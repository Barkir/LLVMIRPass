//===- CollapseIdenticalNodes.cpp ----------------------------------------------------===//
//
//  Pass by Barkir
//  description: collapsing identical nodes
//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Utils/CollapseIdenticalNodes.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/Instructions.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/CFG.h"

#define DEBUG_TYPE "collapse-nodes"

#ifdef DEBUG
#define ON_DEBUG(code) code
#else
#define ON_DEBUG(code)
#endif

using namespace llvm;

using valueHashTab = std::unordered_map<Value*, int32_t>;

int CountValueRecursively(Value *val, valueHashTab& tab) {
    errs() << UCYN "CountValueRecursively" RESET << "\n";
    int retVal = 0;

    auto value = tab.find(val);
    if (value != tab.end()) {
        errs() << "Found value in hashtable " << BYEL << value->second << RESET "\n";
        return value->second;
    }
    if (auto intVal = dyn_cast<ConstantInt>(val)) {
        errs() << "The from instruction is " << BYEL << value->second << RESET "\n";
        return intVal->getSExtValue();
    }

    for (auto User : val->users()) {
        errs() << "val: " << *val << "||| User is " << *User << "\n";
        if (auto *instr = dyn_cast<BinaryOperator>(User)) {
            switch (instr->getOpcode()) {
                case Instruction::Add:
                    errs() << YEL "ADD" RESET "\n";
                    retVal = CountValueRecursively(instr->getOperand(0), tab) + CountValueRecursively(instr->getOperand(1), tab);
                    break;

                case Instruction::Sub:
                    errs() << YEL "SUB" RESET "\n";
                    retVal = CountValueRecursively(instr->getOperand(0), tab) - CountValueRecursively(instr->getOperand(1), tab);
                    break;

                case Instruction::Mul:
                    errs() << YEL "MUL" RESET "\n";
                    retVal = CountValueRecursively(instr->getOperand(0), tab) * CountValueRecursively(instr->getOperand(1), tab);
                    break;

                case Instruction::SDiv:
                    errs() << YEL "DIV" RESET "\n";
                    retVal = CountValueRecursively(instr->getOperand(0), tab) / CountValueRecursively(instr->getOperand(1), tab);
                    break;
            }
        }
    }

    return retVal;
}

int GetConstantFromPhiNode(Instruction *phi) {
    errs() << UCYN "GetConstantFromPhiNode" RESET << "\n";
    auto *phiNode = dyn_cast<PHINode>(phi);
    if (phiNode) {
        int numValues = phiNode->getNumIncomingValues();
        for (int i = 0; i < numValues; i++) {
            if (auto *constantVal = dyn_cast<ConstantInt>(phiNode->getIncomingValue(i))) {
                return constantVal->getSExtValue();
            }
        }
    }
}

Instruction* PhiNodeWithConstant(BasicBlock& BB) {
    errs() << UCYN "PhiNodeWithConstant" RESET << "\n";
    for (auto &I : BB) {
        errs() << BYEL "Got instruction " << I <<  RESET "\n";
        if (auto *phiInstr = dyn_cast<PHINode>(&I)) {
            errs() << "Found phi node " << *phiInstr << "\n";
            int numValues = phiInstr->getNumIncomingValues();
            for (int i = 0; i < numValues; i++) {
                if (auto *constantVal = dyn_cast<ConstantInt>(phiInstr->getIncomingValue(i))) {
                    errs() << "Phi node has a constant. Returning it as an Instruction*" << "\n";
                    errs() << UCYN "END OF FUNC" RESET << "\n";
                    return &I;
                }
            }
        }
    }
    errs() << UCYN "END OF FUNC" RESET << "\n";
    return nullptr;
}

valueHashTab collectICmp(Function &F) {
    errs() << UCYN "collectICmp" RESET << "\n";
    valueHashTab instrMap;
    for (auto &BB : F) {
        for (auto &I : BB) {
            if (I.getOpcode() == Instruction::ICmp) {
                errs() << BYEL "Found icmp! " << I << RESET "\n";
                auto *castI = dyn_cast<ICmpInst>(&I);
                if(castI->getPredicate() == CmpInst::ICMP_EQ) {
                    errs() << "That is icmp eq!" << "\n";
                    auto *firstOperand  = I.getOperand(0);
                    auto *secondOperand = I.getOperand(1);
                    if (auto *secondOperandConst = dyn_cast<ConstantInt>(secondOperand)) {
                        errs() << BYEL << "second op is num! ";
                        const int32_t sextVal = secondOperandConst->getSExtValue();
                        errs() << CYNHB "sextVal is " << sextVal << RESET "\n";
                        instrMap[firstOperand] = sextVal;
                    } else if (auto *firstOperandConst = dyn_cast<ConstantInt>(firstOperand)) {
                        errs() << BYEL << "first op is num! ";
                        const int32_t sextVal = firstOperandConst->getSExtValue();
                        errs() << CYNHB "sextVal is " << sextVal << RESET "\n";
                        instrMap[secondOperand] = sextVal;
                    }
                }
            }
        }
    }
    errs() << UCYN "END OF FUNC" RESET << "\n";
    return instrMap;
}

PreservedAnalyses CollapseIdenticalNodesPass::run(Function &F, FunctionAnalysisManager &AM) {

    errs() << "Starting CollapseIdenticalNodesPass..." << "\n";

    for (auto &BB : F) {
        if (auto *phiNode = PhiNodeWithConstant(BB)) {
            valueHashTab tab = collectICmp(F);
            errs() << "collected icmp and got hashtab!" << "\n";
            // auto *phiVal = dyn_cast<Value>(phiNode);
            auto *phi = dyn_cast<PHINode>(phiNode);
            auto value = CountValueRecursively(phi->getIncomingValue(1), tab);
            errs() << MAGB "Total Value is " << value << RESET "\n";
            if (value == GetConstantFromPhiNode(phiNode)) {
                // DeprecateConstantFromPhiNode(phiNode);
                errs() << "WOW" << "\n";
                return PreservedAnalyses::all();
            }
        }
    }
    return PreservedAnalyses::none();
}


