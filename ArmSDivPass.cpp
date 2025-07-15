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
        return I->getOperand(1);                                                                            errs() << "Found -1 in icmp" <<                                                  "\n";

    auto *secondCI = dyn_cast<ConstantInt>(I->getOperand(1));
    if (secondCI && secondCI->isMinusOne())
        return I->getOperand(0);
                                                                                                            errs() << "-1 not found in icmp" <<                                              "\n";
    return nullptr;
}

// Function for finding sub 0, %1 pattern
// Input
//          True branch block
// Returns
//          BinOp if true
//          nullpt if pattern is not found in block
Instruction* FindSubZeroPattern(BasicBlock *BB, Instruction *SDivInstr)
{
                                                                                                            errs() << "Starting FindSubZeroPattern with basic block" <<                     "\n";
    for (auto &I : *BB) {
        if (auto *BinOp = dyn_cast<BinaryOperator>(&I)) {
            if (BinOp->getOpcode() == Instruction::Sub) {                                                   errs() << "Got sub instruction in true branch" <<                               "\n";
                if (auto *firstOp = dyn_cast<ConstantInt>(BinOp->getOperand(0))) {                          errs() << "First operand " << *firstOp <<                                       "\n";
                    if (firstOp->isZero() && BinOp->getOperand(1) == SDivInstr->getOperand(0)) {            errs() << "Found sub 0, %{smth} pattern!" <<                                    "\n";
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
Instruction* FindSDiv(BasicBlock *BB)
{
    for (auto &I : *BB) {
        if (auto *BinOp = dyn_cast<BinaryOperator>(&I)) {
            if (BinOp->getOpcode() == Instruction::SDiv) {                                                  errs() << "Found SDivInstruction!" <<                                           "\n";
                    return &I;
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
BasicBlock* FindPhiPattern(BasicBlock *firstBranch, BasicBlock *SecondBranch, Instruction *SDivPattern, Instruction *SubZeroPattern)
{
    int current = 0;
    PHINode *FinalBlockPhi = nullptr;
    ReturnInst *FinalBlockRet = nullptr;

    // Checking if false and true successors have the same successor
    if (!(SecondBranch->getSingleSuccessor() == firstBranch->getSingleSuccessor()))
        return nullptr;
                                                                                                            errs() << "Branches have the same successor!" <<                            "\n";
    auto *FinalBlock = SecondBranch->getSingleSuccessor();
    // Checking if final block contains phi and ret instructions
    for (Instruction &I : *FinalBlock) {
        if (I.getOpcode() == Instruction::PHI) {                                                            errs() << "Phi instruction found in final block" <<                         "\n";
                FinalBlockPhi = cast<PHINode>(&I);
                FinalBlockPhi->replaceAllUsesWith(SDivPattern);
                break;
        }
    }

    if (FinalBlockPhi) {
        if (isPhiPatternValid(FinalBlockPhi, SubZeroPattern, SDivPattern)) {                                errs() << "Phi node pattern is valid!" <<                                   "\n";
                return FinalBlock;
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
    if (!M) {                                                                                               errs() << "Function is not a part of a module!" <<                              "\n";
        return PreservedAnalyses::none();
    }

    // Here we get current architecture.
    // This pass is specialized for arm architecture.
    // if the architectures don't match, the pass won't go further
    StringRef TargetTriple = M->getTargetTriple().str();                                                    errs() << "TargetTriple:  " << TargetTriple <<                                  "\n";
    bool isARM = TargetTriple.contains("arm") || TargetTriple.contains("aarch64");

    if (!isARM) {                                                                                           errs() << "Architecture doesт't match the pass" <<                              "\n";
        return PreservedAnalyses::none();
    }

    for (auto &BB : F)
    {
        auto *SDivInstr = FindSDiv(&BB);
        if (!SDivInstr)
            continue;
        auto *firstOperand = SDivInstr->getOperand(0);
        auto *secondOperand = SDivInstr->getOperand(1);

        for (auto *BP : predecessors(&BB))
        {
            auto *Term = BP->getTerminator();
            auto *BI = dyn_cast<BranchInst>(Term);
            if (!BI || !BI->isConditional()) {
                return PreservedAnalyses::none();
            }
                                                                                                                errs() << "SDiv's parent block is conditional!" <<                              "\n";
            Value *Cond = BI->getCondition();

            auto * ICmp = dyn_cast<ICmpInst>(Cond);
            if (!ICmp) {                                                                                        errs() << "Conditional instruction is not icmp" <<                              "\n";
                return PreservedAnalyses::none();
            }

            // Here we check if there is -1 in comparsion and
            // register in icmp equals to divisor in sdiv instruction
            ICmpOp = CheckMinusOneOperands(ICmp);
            if (!ICmpOp && (ICmpOp != secondOperand)) {
                return PreservedAnalyses::none();
            }

            // Getting other branch of the diamond
            if (BI->getNumSuccessors() != 2)
                return PreservedAnalyses::none();
            auto *OtherBranch = BI->getSuccessor(1) == &BB ? BI->getSuccessor(0) : BI->getSuccessor(1);
            auto *SubZeroInstr = FindSubZeroPattern(OtherBranch, SDivInstr);
            if (!SubZeroInstr)
                return PreservedAnalyses::none();

            // Finding phi node pattern
            if (FindPhiPattern(OtherBranch, &BB, SDivInstr, SubZeroInstr))
                return PreservedAnalyses::all();
        } // end of basic block's predecessors cycle
    } // end of basic blocks cycle
}

