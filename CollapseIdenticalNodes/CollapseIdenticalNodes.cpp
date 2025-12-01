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
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"
// #include "llvm/Support/CFG.h"

#define DEBUG_TYPE "collapse-nodes"

#ifdef DEBUG
#define ON_DEBUG(code) code
#else
#define ON_DEBUG(code)
#endif

using namespace llvm;

using valueHashTab = std::unordered_map<Value*, int32_t>;


// table for collecting icmp (needed to proof && links between conditions)
static std::vector<Instruction*> icmpTable;
using icmpIterator = std::vector<Instruction*>::iterator;

llvm::raw_fd_ostream& operator<<(llvm::raw_fd_ostream& os, valueHashTab& tab) {

    for (size_t i = 0; i < tab.size(); i++) {
    os << "═════";
    }
    os << "\n";

    for (const auto &pair : tab) {
        os << "║ ";
        if (pair.first) {
            pair.first->printAsOperand(os, false);
            os << "\t\t";
        } else {
            os << "null";
        }
        os << "║ " << "\t" << pair.second << "║\n";
    }
    for (size_t i = 0; i < tab.size(); i++) {
    os << "═════";
    }
    os << "\n";
    return os;
}

int CountValueRecursively(Value *val, valueHashTab& tab) {
    errs() << UCYN "CountValueRecursively of " << *val << RESET << "\n";
    int retVal = 0;
    auto value = tab.find(val);
    if (value != tab.end()) {
        errs() << "Found value in hashtable " << BYEL << value->second << RESET "\n";
        return value->second;
    }
    if (auto intVal = dyn_cast<ConstantInt>(val)) {
        errs() << "The value from instruction is " << BYEL << *intVal << RESET "\n";
        return intVal->getSExtValue();
    }

    auto *instr = dyn_cast<BinaryOperator>(val);
    if (instr) {
        switch (instr->getOpcode()) {
            case Instruction::Add:
                errs() << YEL "ADD " << *instr << RESET "\n";
                retVal = CountValueRecursively(instr->getOperand(0), tab) + CountValueRecursively(instr->getOperand(1), tab);
                break;

            case Instruction::Sub:
                errs() << YEL "SUB " << *instr <<  RESET "\n";
                retVal = CountValueRecursively(instr->getOperand(0), tab) - CountValueRecursively(instr->getOperand(1), tab);
                break;

            case Instruction::Mul:
                errs() << YEL "MUL " << *instr << RESET "\n";
                retVal = CountValueRecursively(instr->getOperand(0), tab) * CountValueRecursively(instr->getOperand(1), tab);
                break;

            case Instruction::SDiv:
                errs() << YEL "DIV " << *instr << RESET "\n";
                retVal = CountValueRecursively(instr->getOperand(0), tab) / CountValueRecursively(instr->getOperand(1), tab);
                break;
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

Value *getPznOrConstant(Value *val, valueHashTab& tab) {
    errs() << UCYN "getPznOrConstant" RESET << "\n";
    LLVMContext &ctx = val->getContext();

    // finding values in hash table (depth = 2);
    auto hfind_val = tab.find(val);

    // explained on this example:
    // %val = {instr} i32 %val1, %val2;
    // {instr} is '+', '-', '*', '/'
    if (hfind_val != tab.end()) { // a case where %val is already in a hashtable (return: tab[%val])
        errs() << BYEL "Value " RESET << hfind_val->first << BYEL " is in hash table - " << BWHT << hfind_val->second << RESET "\n";
        ConstantInt* ret = ConstantInt::get(Type::getInt32Ty(ctx), hfind_val->second, true);
        return ret;
    }
    if (auto* instrVal = dyn_cast<Instruction>(val)) {
        auto hfind_fop = tab.find(instrVal->getOperand(0));
        if (hfind_fop != tab.end()) { // a case where %val1 is in a hashtable and %val2 is a constant (return: tab[%val1] {instr} %val2)
            errs() << BYEL "Value " RESET << *hfind_fop->first << BYEL " is in hash table - " << BWHT << hfind_fop->second << RESET "\n";
            auto *foundInstr = dyn_cast<BinaryOperator>(instrVal);
            if (foundInstr) {
                auto *secondOperandConst = dyn_cast<ConstantInt>(foundInstr->getOperand(1));

                if (secondOperandConst) {
                    errs() << BYEL "second operand is a ConstantInt : first operand is an Instruction" << "RESET" << "\n";
                    int32_t firstConst = hfind_fop->second;
                    auto secondConst = secondOperandConst->getSExtValue();

                    int32_t retInt = 0;
                    ConstantInt *ret = nullptr;

                    switch (foundInstr->getOpcode()) {
                        case Instruction::Add:
                            retInt = firstConst + secondConst;
                            errs() << GRNHB "ADD = " << retInt << "\n";
                            ret = ConstantInt::get(Type::getInt32Ty(ctx), retInt, true);
                            errs() << UCYN "END OF FUNC" RESET << "\n";
                            return ret;

                        case Instruction::Sub:
                            retInt = firstConst - secondConst;
                            errs() << GRNHB "SUB = " << retInt << "\n";
                            ret = ConstantInt::get(Type::getInt32Ty(ctx), retInt, true);
                            errs() << UCYN "END OF FUNC" RESET << "\n";
                            return ret;

                        case Instruction::Mul:
                            retInt = firstConst * secondConst;
                            errs() << GRNHB "MUL = " << retInt << "\n";
                            ret = ConstantInt::get(Type::getInt32Ty(ctx), retInt, true);
                            errs() << UCYN "END OF FUNC" RESET << "\n";
                            return ret;

                        case Instruction::SDiv:
                            retInt = firstConst / secondConst;
                            errs() << GRNHB "SDiv = " << retInt << "\n";
                            ret = ConstantInt::get(Type::getInt32Ty(ctx), retInt, true);
                            errs() << UCYN "END OF FUNC" RESET << "\n";
                            return ret;
                }
            }
        }
    }

    }
    Value *pzn = PoisonValue::get(Type::getInt32Ty(ctx));
    errs() << UCYN "END OF FUNC" RESET << "\n";
    return pzn;
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
                    auto *secondFromTab = getPznOrConstant(secondOperand, instrMap);
                    if (auto *secondOperandConst = dyn_cast<ConstantInt>(secondOperand)) {
                        errs() << BYEL << "second op is num! ";
                        const int32_t sextVal = secondOperandConst->getSExtValue();
                        errs() << CYNHB "sextVal is " << sextVal << RESET "\n";
                        instrMap[firstOperand] = sextVal;
                        icmpTable.push_back(castI);
                    } else if (!isa<PoisonValue>(secondFromTab)) {
                        auto *secondFromTabConst = dyn_cast<ConstantInt>(secondFromTab);
                        errs() << BYEL << "secondOperand is a complexed value!" << RESET << "\n";
                        const int32_t sextVal = secondFromTabConst->getSExtValue();
                        errs() << CYNHB "sextVal is " << sextVal << RESET "\n";
                        instrMap[firstOperand] = sextVal;
                        icmpTable.push_back(castI);
                    } else if (auto *firstOperandConst = dyn_cast<ConstantInt>(firstOperand)) {
                        errs() << BYEL << "first op is num! ";
                        const int32_t sextVal = firstOperandConst->getSExtValue();
                        errs() << CYNHB "sextVal is " << sextVal << RESET "\n";
                        instrMap[secondOperand] = sextVal;
                        icmpTable.push_back(castI);
                    }

                }
            }
        }
    }

    errs() << instrMap;
    errs() << UCYN "END OF FUNC" RESET << "\n";
    return instrMap;
}

bool checkLinkThruTerminator(icmpIterator pred, icmpIterator post) {
    errs() << UCYN "checkLinkThruTerminator" RESET << "\n";
    auto predBBTerminator = (*pred)->getParent()->getTerminator();
    auto postBB = (*post)->getParent();

    errs() << *(predBBTerminator->getOperand(2)) << *postBB << "\n";
    if (predBBTerminator->getOperand(2) == postBB) {
        errs() << CYNHB "EQUAL!" << RESET "\n";
        errs() << UCYN "END OF FUNC" RESET << "\n";
        return true;
    }
    errs() << UCYN "END OF FUNC" RESET << "\n";
    return false;
}

bool checkLinkThruSelect(icmpIterator pred, icmpIterator post) {
    errs() << UCYN "checkLinkThruSelect" RESET << "\n";
    if ((*pred)->getParent() == (*post)->getParent()) { // instructions are in the same basic block
        for (auto user: (*pred)->users()) {
            errs() << "Got User " << *user << "\n";
            if (auto userToInst = dyn_cast<Instruction>(user)) {
                if (userToInst->getOpcode() == Instruction::Select) {
                    errs() << "user is select!" << "\n";
                    if (userToInst->getOperand(0) == *pred && userToInst->getOperand(1) == *post) {
                        *post = userToInst; // updating because select result will be used in future BB's, not the old post
                        return true;
                    }
                }
            }
        }

    }
    errs() << UCYN "END OF FUNC" RESET << "\n";
    return false;
}

bool proofConjuctionLinks() {
    errs() << UCYN "proofConjuctionLinks" RESET << "\n";
    for (auto it = icmpTable.begin(), end = icmpTable.end() - 1; it != end; ++it) {
        errs() << **it << **(it + 1) << "\n";
        if (!checkLinkThruTerminator(it, it + 1) && !checkLinkThruSelect(it, it+1)) {
            return false;
        }
    }
    return true;
    errs() << UCYN "END OF FUNC" RESET << "\n";
}

void DeprecateConstantFromPhiNode(PHINode* phi, BasicBlock *target) {
    errs() << UCYN "DeprecateConstantFromPhiNode" RESET << "\n";
    const int32_t numValues = phi->getNumIncomingValues();
    for (int i = 0; i < numValues; ++i) {
        if (auto *incomingValue = dyn_cast<ConstantInt>(phi->getIncomingValue(i))) {
            BasicBlock *blockToDelete = phi->getIncomingBlock(i);
            for (pred_iterator PI = pred_begin(blockToDelete); PI != pred_end(blockToDelete); ++PI) {
                BasicBlock *pred = *PI;

                pred->getTerminator()->replaceSuccessorWith(blockToDelete, target);
            }
            DeleteDeadBlock(blockToDelete);
            break;
        }
    }
    errs() << UCYN "END OF FUNC" RESET << "\n";
}

PreservedAnalyses CollapseIdenticalNodesPass::run(Function &F, FunctionAnalysisManager &AM) {

    errs() << "Starting CollapseIdenticalNodesPass..." << "\n";

    for (auto &BB : F) {
        if (auto *phiNode = PhiNodeWithConstant(BB)) {
            valueHashTab tab = collectICmp(F);
            errs() << "collected icmp and got hashtab!" << "\n";
            bool links = proofConjuctionLinks();
            if (links){
            auto *phi = dyn_cast<PHINode>(phiNode);
            auto *phiToReplace = phi->getIncomingValue(1);
            auto value = CountValueRecursively(phiToReplace, tab);
            errs() << MAGB "Total Value is " << value << RESET "\n";
            if (value == GetConstantFromPhiNode(phiNode)) {
                if (auto *phiToReplaceInstr = dyn_cast<Instruction>(phiToReplace)) {
                    BasicBlock *incomingBlock = phi->getIncomingBlock(1);
                    DeprecateConstantFromPhiNode(phi, incomingBlock);
                    errs() << F << "\n";
                }
                errs() << "VALUES ARE EQUAL" << "\n";
                return PreservedAnalyses::all();
            }
            }
        }
    }
    return PreservedAnalyses::none();
}


