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
// TODO: test-suite, bootstrap build
// opt -debug-only=DEBUG_TYPE <- macro


// Function for checking if any of operands of the instruction is -1
// Returns
//          nullptr           if false
//          comparing operand if true
Value* CheckMinusOneOperands(Instruction *I)
{
    auto *firstCI  = dyn_cast<ConstantInt>(I->getOperand(0));
    if (firstCI  && firstCI->isMinusOne())
        return I->getOperand(1);                                                                            errs() << "Found -1 in icmp" << "\n";

    auto *secondCI = dyn_cast<ConstantInt>(I->getOperand(1));
    if (secondCI && secondCI->isMinusOne())
        return I->getOperand(0);
                                                                                                            errs() << "-1 not found in icmp" << "\n";
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
                                                                                                            errs() << "Starting FindSubZeroPattern with basic block" << "\n";
    for (auto &I : *BB) {
        if (auto *BinOp = dyn_cast<BinaryOperator>(&I)) {
            if (BinOp->getOpcode() == Instruction::Sub) {                                                   errs() << "Got sub instruction in true branch" << "\n";
                if (BinOp->getOperand(1) == SDivInstr->getOperand(0)) {                                     errs() << "Found sub %{smth}, %{smth} pattern!" << "\n";
                           return &I;
                    }

            }
        }
    }
    return nullptr;                                                                                         errs() << "Pattern not found" << "\n";
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
            if (BinOp->getOpcode() == Instruction::SDiv) {                                                  errs() << "Found SDivInstruction!" << "\n";
                    return &I;
            }
        }
    }
    return nullptr;
}

bool isPhiPatternValid(PHINode *Phi, Instruction *SubZero, Instruction *SDiv)
{
    int numVal = Phi->getNumIncomingValues();
    errs() << "Got " << numVal << " values in phi-node" << "\n";

    bool isSubZero = false;
    bool isSDiv = false;

    for (int i = 0; i < numVal; i++) {
        auto *income = Phi->getIncomingValue(i);
        errs() << "incoming value: " << *income << "\n";
        if (income == SubZero) {
            isSubZero = true;
            errs() << "isSubZero is true!" << "\n";
        }
        else if (income == SDiv) {
            errs() << "isSDiv is true!" << "\n";
            isSDiv = true;
        }
        else {
            for (auto *User : SDiv->users()) {
                errs() << "user: " << *User << "\n";
                if (User == income) {
                    errs() << "isSDiv is true as a user!" << "\n";
                    isSDiv = true;
                    break;
                }
            }
        }
    }
    return isSubZero && isSDiv;
}

// Function for finding and transforming pattern:
//      %{smth} = phi [%{SDivPattern}, %br1], [%{SubZeroPattern}, %br2]
//      ret i32 %{smth}
//      it deletes the SubZeroPattern
// Input
//      True and False Branch successors, SDiv and SubZero Pattern
// Returns
//      true if pattern is found
//      false if pattern is not found
BasicBlock* FindAndTransformPhiPattern(BasicBlock *SubZeroBranch, BasicBlock *SDivBranch, Instruction *SDivPattern, Instruction *SubZeroPattern)
{
    int current = 0;
    PHINode *FinalInstrPhi = nullptr;
    ReturnInst *FinalBlockRet = nullptr;

    // Checking if false and true successors have the same successor
    if (!(SDivBranch->getSingleSuccessor() == SubZeroBranch->getSingleSuccessor()))
        return nullptr;
                                                                                                            errs() << "Branches have the same successor!" << "\n";
    auto *FinalBlock = SDivBranch->getSingleSuccessor();
    // Checking if final block contains phi and ret instructions
    for (Instruction &I : *FinalBlock) {
        if (I.getOpcode() == Instruction::PHI) {
                FinalInstrPhi = cast<PHINode>(&I);
                break;
        }
    }

    if (FinalInstrPhi) {                                                                                    errs() << "Phi instruction found in final block" << *FinalInstrPhi << "\n";
        if (isPhiPatternValid(FinalInstrPhi, SubZeroPattern, SDivPattern)) {                                errs() << "Phi node pattern is valid!" << "\n";
                const uint32_t numValues = FinalInstrPhi->getNumIncomingValues();
                for (int i = 0; i < numValues; i++) {
                    if (FinalInstrPhi->getIncomingValue(i) == SubZeroPattern) {                             errs() << "Incoming value with number: " << i << *SubZeroPattern << "\n";
                        FinalInstrPhi->removeIncomingValue(i);                                              errs() << "removed " << *SubZeroPattern << " from phinode, it is now -> " << *FinalInstrPhi << "\n";
                        break;
                    }
                }
                return FinalBlock;
            }
        }
    return nullptr;
}

// Function checks if branch instruction contains two blocks in conditions
bool isBranchPatternValid(BranchInst *BI, BasicBlock *SubZeroBlock, BasicBlock *SDivBlock)
{
    errs() << "Checking branch pattern..." << "\n";
    errs() << "instr: " << *BI << "\n";
    errs() << "first op" << *(BI->getOperand(1)) << BI->getOperand(0) << "\n";
    errs() << "second op" << *(BI->getOperand(2)) << BI->getOperand(1) << "\n";

    return (BI->getOperand(1) == SubZeroBlock && BI->getOperand(2) == SDivBlock) ||
           (BI->getOperand(2) == SubZeroBlock && BI->getOperand(1) == SDivBlock);
}

// Function for transforming and removing icmp instruction, including it's users
// Input
//      instruction, basic block with sdiv, basic block with sub
bool TransformICmpAndUsers(Instruction *ICmp, BasicBlock *SDivBlock, BasicBlock *SubZeroBlock)
{
    errs() << "Got instruciton " << *ICmp << "\n";

    errs() << "Getting it's users" << "\n";

    for (auto *User : ICmp->users()) {
        errs() << ">> " << *User << "\n";
        if (auto *BI = dyn_cast<BranchInst>(User)) {
            auto * BranchBlock = BI->getParent(); errs() << "Instruction is located in basic block " << *BranchBlock << "\n";
            if (isBranchPatternValid(BI, SubZeroBlock, SDivBlock)){
                errs() << "SubZeroBlock and SDivBlock is in condition!" << "\n";
                BranchInst::Create(SDivBlock, BranchBlock->end());
                BI->eraseFromParent();
                errs() << "Removed old branch instruction" << "\n";
                errs() << "Now block looks like:" << *BranchBlock << "\n";
                SubZeroBlock->dropAllReferences();
                SubZeroBlock->eraseFromParent();
                errs() << "Erased SubZeroBlock..." << "\n";
                return true;
            }
        }
    }
    return false;
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
    if (!M) {                                                                                               errs() << "Function is not a part of a module!" << "\n";
        return PreservedAnalyses::none();
    }

    // Here we get current architecture.
    // This pass is specialized for arm architecture.
    // if the architectures don't match, the pass won't go further
    StringRef TargetTriple = M->getTargetTriple().str();                                                    errs() << "TargetTriple:  " << TargetTriple << "\n";
    bool isARM = TargetTriple.contains("arm") || TargetTriple.contains("aarch64");

    if (!isARM) {                                                                                           errs() << "Architecture doesт't match the pass" << "\n";
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
                                                                                                                errs() << "SDiv's parent block is conditional!" << "\n";
            Value *Cond = BI->getCondition();                                                                   errs() << "ICmp is " << *Cond << "\n";

            auto * ICmp = dyn_cast<ICmpInst>(Cond);
            if (!ICmp) {                                                                                        errs() << "Conditional instruction is not icmp" << "\n";
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
            auto *SubZeroBlock = BI->getSuccessor(1) == &BB ? BI->getSuccessor(0) : BI->getSuccessor(1);
            auto *SubZeroInstr = FindSubZeroPattern(SubZeroBlock, SDivInstr);
            if (!SubZeroInstr)
                return PreservedAnalyses::none();

            // Finding phi node pattern
            if (FindAndTransformPhiPattern(SubZeroBlock, &BB, SDivInstr, SubZeroInstr))
            {
                errs() << "Now we need to find all ICmp users in our function and transform them" << "\n";
                TransformICmpAndUsers(ICmp, &BB, SubZeroBlock);
                return PreservedAnalyses::all();
            }
        } // end of basic block's predecessors cycle
    } // end of basic blocks cycle

    return PreservedAnalyses::none();
}

