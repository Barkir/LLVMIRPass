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

void collectICmp(BasicBlock *BB) {
    std::unordered_map<Value*, const int32_t> instrMap;
    for (auto &I : *BB) {
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

PreservedAnalyses CollapseIdenticalNodesPass::run(Function &F, FunctionAnalysisManager &AM) {

    for (auto &BB : F) {
        if (endsWithConditionalBranch(&BB)) {
            collectICmp(&BB);
            // if (compareConditions(BB) == true) {
                // collapseCondition(BB);
            // }
        }
    }
    return PreservedAnalyses::none();
}


