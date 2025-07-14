#include "llvm/Transforms/Utils/DivToIntrinsic.h"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/CFG.h"
#include "llvm/Support/raw_ostream.h"

/*
    WARNING!!! ACHTUNG!!! 警告!!! تحذير!!!

    Pass is written only for arm machines. Sorry x86 :(
    Pass wird nur für Arm-Computer geschrieben. Entschuldigung x86 :(
    Pass 仅针对 Arm 计算机写入。对不起 x86 :(
    التمرير مكتوب فقط لآلات الذراع. آسف x86 :(
*/

using namespace llvm;

// TODO: LLVM_DEBUG
// opt -debug-only=DEBUG_TYPE <- macro


// Function for checking if any of operands of the instruction is -1
// Returns
//          nullptr           if false
//          comparing operand if true
Value* CheckMinusOneOperands(Instruction *I)
{
    auto *firstCI  = dyn_cast<ConstantInt>(I->getOperand(0));
    if (firstCI  && firstCI->isMinusOne())
        return I->getOperand(1);                                                                            errs() << "Found -1 in cmp" <<                                                  "\n";

    auto *secondCI = dyn_cast<ConstantInt>(I->getOperand(1));
    if (secondCI && secondCI->isMinusOne())
        return I->getOperand(0);

    return nullptr;
}

// Function for finding sub 0, %1 pattern
// Input
//          True branch block
// Returns
//          BinOp if true
//          nullpt if pattern is not found in block
Instruction* FindSubZeroPattern(BasicBlock *BB)
{
                                                                                                            errs() << "Starting FindSubZeroPattern with basic block" <<                     "\n";
    for (auto &I : *BB) {
        if (auto *BinOp = dyn_cast<BinaryOperator>(&I)) {
            if (BinOp->getOpcode() == Instruction::Sub) {                                                   errs() << "Got sub instruction in true branch" <<                               "\n";
                if (auto *firstOp = dyn_cast<ConstantInt>(BinOp->getOperand(0))) {                          errs() << "First operand " << *firstOp <<                                       "\n";
                    if (firstOp->isZero()) {                                                                errs() << "Found sub 0, %{smth} pattern!" <<                                    "\n";
                           return &I;
                    }
                }
            }
        }
    }
    return nullptr;                                                                                         errs() << "Pattern not found" <<                                                "\n";
}

// Function for finding sub %0, %1 pattern
// Input
//      BasicBlock (false branch in example)
//      firstOp (compared with %0)
//      secondOp (compated with %1)
// Returns
//      true if pattern is found in block
//      false if pattern is not found in block
Instruction* FindSDivPattern(BasicBlock *BB, Value *firstOp, Value *secondOp)
{
    for (auto &I : *BB) {
        if (auto *BinOp = dyn_cast<BinaryOperator>(&I)) {
            if (BinOp->getOpcode() == Instruction::SDiv) {
                if (BinOp->getOperand(0) == firstOp && BinOp->getOperand(1) == secondOp) {                  errs() << "Found sdiv %{smth}, %{smth} pattern!" <<                              "\n";
                    return &I;
                }
            }
        }
    }
    return nullptr;
}

bool isPhiPatternValid(PHINode *Phi, Instruction *fNode, Instruction *sNode)
{
    return (Phi->getIncomingValue(0) == fNode && Phi->getIncomingValue(1) == sNode) ||
           (Phi->getIncomingValue(1) == fNode && Phi->getIncomingValue(0) == sNode);
}

// Function for finding pattern:
//      %{smth} = phi [%{SDivPattern}, %br1], [%{SubZeroPattern}, %br2]
//      ret i32 %{smth}
// Input
//      True and False Branch successors, SDiv and SubZero Pattern
// Returns
//      true if pattern is found
//      false if pattern is not found
BasicBlock* FindPhiPattern(BasicBlock *TrueSucc, BasicBlock *FalseSucc, Instruction *SDivPattern, Instruction *SubZeroPattern)
{
    int current = 0;
    PHINode *FinalBlockPhi = nullptr;
    ReturnInst *FinalBlockRet = nullptr;

    // Checking if false and true successors have the same successor
    if (!(FalseSucc->getSingleSuccessor() == TrueSucc->getSingleSuccessor()))
        return nullptr;

    auto *FinalBlock = FalseSucc->getSingleSuccessor();
    // Checking if final block contains phi and ret instructions
    for (Instruction &I : *FinalBlock) {
        if (current == 0 && I.getOpcode() == Instruction::PHI)
                FinalBlockPhi = cast<PHINode>(&I);
        else if (current == 1 && I.getOpcode() == Instruction::Ret)
            FinalBlockRet = cast<ReturnInst>(&I);
        current++;
    }

    // Seems like a non-scalable solution, weak point.
    if (current == 2 && FinalBlockPhi && FinalBlockRet) {
        if (isPhiPatternValid(FinalBlockPhi, SubZeroPattern, SDivPattern)) {
            if (FinalBlockRet->getReturnValue() == FinalBlockPhi) {
                return FinalBlock;
            }
        }
    }
    return nullptr;
}

void clearBasicBlock(BasicBlock *BB)
{
    while (!BB->empty()) {
        Instruction *I = &(BB->back());
        I->eraseFromParent();
    }
}

PreservedAnalyses llvm::DivToIntrinsicPass::run(Function &F,
                                          FunctionAnalysisManager &AM)
{
    Value *ICmpOp = nullptr;

    Module *M = F.getParent();
    if (!M) {                                                                                               errs() << "Function is not a part of a module!" <<                                          "\n";
        return PreservedAnalyses::none();
    }

    // Here we get current architecture.
    // This pass is specialized for arm architecture.
    // if the architectures don't match, the pass won't go further
    StringRef TargetTriple = M->getTargetTriple().str();                                                    errs() << "TargetTriple:  " << TargetTriple <<                                              "\n";
    bool isARM = TargetTriple.contains("arm") || TargetTriple.contains("aarch64");

    if (!isARM) {                                                                                           errs() << "Architecture doesт't match the pass" <<                                          "\n";
        return PreservedAnalyses::none();
    }

    for (auto &BB : F) {

        auto *Term = BB.getTerminator();
        if (auto *BI = dyn_cast<BranchInst>(Term)) {
            if (BI->isConditional()) {

                Value *Cond = BI->getCondition();
                if (auto *ICmp = dyn_cast<ICmpInst>(Cond)) {                                                errs() << "Found icmp branch condition" <<                                                  "\n";
                    if (!(ICmpOp = CheckMinusOneOperands(ICmp)))
                        return PreservedAnalyses::none();
                }
                BasicBlock *TrueSucc  = BI->getSuccessor(0);                                                errs() << "True successor: "  << *TrueSucc  <<                                              "\n";

                // Trying to find sub 0, %{smth} pattern.
                auto *SubZeroPattern = FindSubZeroPattern(TrueSucc);
                if (!SubZeroPattern)
                    return PreservedAnalyses::none();
                                                                                                            errs() << "Subtrahend value - " << *SubZeroPattern->getOperand(1) <<                        "\n";
                BasicBlock *FalseSucc = BI->getSuccessor(1);                                                errs() << "False successor: " << *FalseSucc <<                                              "\n";
                                                                                                            errs() << "ICmpOp: " << *ICmpOp << " Subtrahend: " << *SubZeroPattern->getOperand(1) <<     "\n";
                // Trying to find sdiv %{SubZero->getOperand(1)}, %{ICmpOp} pattern
                // ICmpOp is a pointer to the operand, which was compared to -1
                // SubZeroPattern->getOperand(1) is a pointer to a null-deductible value
                auto SDivPattern = FindSDivPattern(FalseSucc, SubZeroPattern->getOperand(1), ICmpOp);
                if (!SDivPattern)
                    return PreservedAnalyses::none();

                // Trying to find phi pattern, explained in function definition.
                auto *FinalBlock = FindPhiPattern(TrueSucc, FalseSucc, SDivPattern, SubZeroPattern);
                if (FinalBlock) {                                                                           errs() << "Found pattern! Transforming..." <<                                               "\n";

                    clearBasicBlock(&BB);
                    BB.dropAllReferences();
                    Instruction * SDivCopy = SDivPattern->clone();
                    SDivCopy->insertInto(&BB, BB.end());

                    ReturnInst::Create(
                        BB.getContext(),
                        SDivCopy,
                        BB.end()
                    );


                    FinalBlock->eraseFromParent();
                    TrueSucc->eraseFromParent();
                    FalseSucc->eraseFromParent();

                    errs() << "Transforming done: " << BB << "\n";
                    return PreservedAnalyses::all();
                }
            }
        } // end of checking BranchInst basic block
    } // end of basic blocks cycle
    return PreservedAnalyses::all();
}

