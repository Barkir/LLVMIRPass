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

#define DEBUG_TYPE "collapse-nodes"

#ifdef DEBUG
#define ON_DEBUG(code) code
#else
#define ON_DEBUG(code)
#endif

using namespace llvm;

bool endsWithConditionalBranch(BasicBlock *BB) {
    auto *term = BB->getTerminator();
    if (auto *BI = dyn_cast<BranchInst>(term)) {
        return BI->isConditional();
    }

    return false;
}

// bool compareConditions(BasicBlock *BB) {
//     auto *leftBB = BB->getSuccessor(0);     // getting left branch
//     auto *rightBB = BB->getSuccessor(1);    // getting right branch
//
//     for (auto *instruction : leftBB) {
//
//     }
// }

using valueHashTab = std::unordered_map<Value*, int32_t>;

int CountValueRecursively(Value *val, valueHashTab& tab) {
    int retVal = 0;

    auto value = tab[val];
    if (tab[val] != tab.end()) {
        return value;
    }
    if (auto intVal = dyn_cast<ConstantInt>(val)) {
        return intVal->getSExtValue();
    }
    int numOperands = val->getNumOperands();
    for (int i = 0; i < numOperands; i++) {
        auto *instr = val->getOperand(i)
        switch (instr->getOpcode()) {
            case Instruction::Add:
                retVal = CountValueRecursively(instr->getOpcode(0), tab) + CountValueRecursively(instr->getOpcode(1), tab);
                break;

            case Instruction::Sub:
                retVal = CountValueRecursively(instr->getOpcode(0), tab) - CountValueRecursively(instr->getOpcode(1), tab);
                break;

            case Instruction::Mul:
                retVal = CountValueRecursively(instr->getOpcode(0), tab) * CountValueRecursively(instr->getOpcode(1), tab);
                break;

            case Instruction::Div:
                retVal = CountValueRecursively(instr->getOpcode(0), tab) / CountValueRecursively(instr->getOpcode(1), tab);
                break;
            }
    }
    return retVal;
}

int GetConstantFromPhiNode(Instruction *phi) {
    auto *phiNode = dyn_cast<PHINode>(phi);
    if (phiNode) {
        int numValues = phiInstr->getNumIncomingValues();
        for (int i = 0; i < numValues; i++) {
            if (auto *constantVal = dyn_cast<ConstantInt>(phiInstr->getIncomingValue(i))) {
                return constantVal->getSExtValue();
            }
        }
    }
}

Instruction* PhiNodeWithConstant(BasicBlock& BB) {
    for (auto &I : BB) {
        if (auto *phiInstr = dyn_cast<PHINode>(&I)) {
            int numValues = phiInstr->getNumIncomingValues();
            for (int i = 0; i < numValues; i++) {
                if (auto *constantVal = dyn_cast<ConstantInt>(phiInstr->getIncomingValue(i))) {
                    return &I;
                }
            }
        }
    }
}

valueHashTab& collectICmp(Function &F) {
    valueHashTab instrMap;
    for (auto &BB : F) {
        for (auto &I : BB) {
            if (I.getOpcode() == Instruction::ICmp) {
                auto *castI = dyn_cast<ICmpInst>(&I);
                if(castI->getPredicate() == CmpInst::ICMP_EQ) {
                    auto *firstOperand  = I.getOperand(0);
                    auto *secondOperand = I.getOperand(1);
                    if (auto *secondOperandConst = dyn_cast<ConstantInt>(secondOperand)) {
                        const int32_t sextVal = secondOperandConst->getSExtValue();
                        instrMap[firstOperand] = sextVal;
                    }
                }
            }
        }
    }
    return instrMap;
}

PreservedAnalyses CollapseIdenticalNodesPass::run(Function &F, FunctionAnalysisManager &AM) {

    for (auto &BB : F) {
        if (auto *phiNode = PhiNodeWithConstant(BB)) {
            valueHashTab tab = collectICmp(F);
            auto value = CountValueRecursively(phiNode, tab);
            if (value == GetConstantFromPhiNode(phiNode)) {
                DeprecateConstantFromPhiNode(phiNode);
                return PreservedAnalyses::all();
            }
        }
    }
    return PreservedAnalyses::none();
}


