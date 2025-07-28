#include "llvm/Transforms/Utils/DivToIntrinsic.h"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/CFG.h"
#include "llvm/Support/raw_ostream.h"
#include "limits.h"

#define DEBUG_BARKIR

#ifdef DEBUG_BARKIR
#define BARK_DEBUG(code) code
#else
#define BARK_DEBUG(code)
#endif

extern llvm::cl::opt<bool> DisableDivToIntrinsicPass;

llvm::cl::opt<bool> DisableDivToIntrinsicPass(
    "disable-divtointrinsic",
    llvm::cl::desc("Argument for disabling running DivToIntrinsicPass"),
    llvm::cl::init(false)
);

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

bool ContainsInOperand(Instruction * I, const int value)
{
    int numOper = I->getNumOperands();
    for (int i = 0; i < numOper; i++) {
        auto *oper = dyn_cast<ConstantInt>(I->getOperand(i));
        if (oper) {
            errs() << "oper #" << i << ": " << *oper << " ";
            errs() << "comaped to value " << value << ":" << (oper->getSExtValue() == value) << "\n";
        }
        if (oper && oper->getSExtValue() == value) {
            errs() << "returning true" << "\n";
            return true;
        }
    }

    return false;
}

Value* CheckMinusOneOperands(Instruction *I)
{
    if (I->getOpcode() == Instruction::ICmp) {
        auto *firstCI  = dyn_cast<ConstantInt>(I->getOperand(0));
        if (firstCI  && firstCI->isMinusOne()) {
            return I->getOperand(1);                                                                                                                                                    BARK_DEBUG(errs() << "Found -1 in icmp" << "\n");
        }
        auto *secondCI = dyn_cast<ConstantInt>(I->getOperand(1));
        if (secondCI && secondCI->isMinusOne()) {
            return I->getOperand(0);
        }
    }

                                                                                                                                                                                    BARK_DEBUG(errs() << "-1 not found in icmp" << "\n");
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
                                                                                                                                                                                    BARK_DEBUG(errs() << "Starting FindSubZeroPattern with basic block" << "\n");
    for (auto &I : *BB) {
        if (auto *BinOp = dyn_cast<BinaryOperator>(&I)) {
            if (BinOp->getOpcode() == Instruction::Sub) {                                                                                                                           BARK_DEBUG(errs() << "Got sub instruction in true branch" << "\n");
                if (BinOp->getOperand(1) == SDivInstr->getOperand(0)) {                                                                                                             BARK_DEBUG(errs() << "Found sub %{smth}, %{smth} pattern!" << "\n");
                           return &I;
                    }

            }
        }
    }
    return nullptr;                                                                                                                                                                 BARK_DEBUG(errs() << "Pattern not found" << "\n");
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
            if (BinOp->getOpcode() == Instruction::SDiv) {                                                                                                                          BARK_DEBUG(errs() << "Found SDivInstruction!" << "\n");
                    return &I;
            }
        }
    }
    return nullptr;
}

bool isPhiPatternValid(PHINode *Phi, Instruction *SubZero, Instruction *SDiv)
{
    int numVal = Phi->getNumIncomingValues();
                                                                                                                                                                                    BARK_DEBUG(errs() << "Got " << numVal << " values in phi-node" << "\n");

    bool isSubZero = false;
    bool isSDiv = false;

    for (int i = 0; i < numVal; i++) {
        auto *income = Phi->getIncomingValue(i);
                                                                                                                                                                                    BARK_DEBUG(errs() << "incoming value: " << *income << "\n");
        if (income == SubZero) {
            isSubZero = true;
                                                                                                                                                                                    BARK_DEBUG(errs() << "isSubZero is true!" << "\n");
        }
        else if (income == SDiv) {
                                                                                                                                                                                    BARK_DEBUG(errs() << "isSDiv is true!" << "\n");
            isSDiv = true;
        }
        else {
            for (auto *User : SDiv->users()) {
                                                                                                                                                                                    BARK_DEBUG(errs() << "user: " << *User << "\n");
                if (User == income) {
                                                                                                                                                                                    BARK_DEBUG(errs() << "isSDiv is true as a user!" << "\n");
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
                                                                                                                                                                                BARK_DEBUG(errs() << "Branches have the same successor!" << "\n");
    auto *FinalBlock = SDivBranch->getSingleSuccessor();
    // Checking if final block contains phi and ret instructions
    for (Instruction &I : *FinalBlock) {
        if (I.getOpcode() == Instruction::PHI) {
                FinalInstrPhi = cast<PHINode>(&I);
                break;
        }
    }
    if (FinalInstrPhi) {                                                                                                                                                        BARK_DEBUG(errs() << "Phi instruction found in final block" << *FinalInstrPhi << "\n");
        if (isPhiPatternValid(FinalInstrPhi, SubZeroPattern, SDivPattern)) {                                                                                                    BARK_DEBUG(errs() << "Phi node pattern is valid!" << "\n");
                const uint32_t numValues = FinalInstrPhi->getNumIncomingValues();
                for (int i = 0; i < numValues; i++) {
                    if (FinalInstrPhi->getIncomingValue(i) == SubZeroPattern) {                                                                                                 BARK_DEBUG(errs() << "Incoming value with number: " << i << *SubZeroPattern << "\n");
                        FinalInstrPhi->removeIncomingValue(i);                                                                                                                  BARK_DEBUG(errs() << "removed " << *SubZeroPattern << " from phinode, it is now -> " << *FinalInstrPhi << "\n");
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
                                                                                                                                                                                BARK_DEBUG(errs() << "Checking branch pattern..." << "\n");
                                                                                                                                                                                BARK_DEBUG(errs() << "instr: " << *BI << "\n");
                                                                                                                                                                                BARK_DEBUG(errs() << "first op" << *(BI->getOperand(1)) << BI->getOperand(0) << "\n");
                                                                                                                                                                                BARK_DEBUG(errs() << "second op" << *(BI->getOperand(2)) << BI->getOperand(1) << "\n");

    return (BI->getOperand(1) == SubZeroBlock && BI->getOperand(2) == SDivBlock) ||
           (BI->getOperand(2) == SubZeroBlock && BI->getOperand(1) == SDivBlock);
}

// Function for transforming and removing icmp instruction, including it's users
// Input
//      instruction, basic block with sdiv, basic block with sub
bool TransformICmpAndUsers(Instruction *ICmp, BasicBlock *SDivBlock, BasicBlock *SubZeroBlock)
{
                                                                                                                                                                            BARK_DEBUG(errs() << "Got instruciton " << *ICmp << "\n");
                                                                                                                                                                            BARK_DEBUG(errs() << "Getting it's users" << "\n");

    for (auto *User : ICmp->users()) {
                                                                                                                                                                            BARK_DEBUG(errs() << ">> " << *User << "\n");
        if (auto *BI = dyn_cast<BranchInst>(User)) {
            auto * BranchBlock = BI->getParent();                                                                                                                           BARK_DEBUG(errs() << "Instruction is located in basic block " << *BranchBlock << "\n");
            if (isBranchPatternValid(BI, SubZeroBlock, SDivBlock)){
                                                                                                                                                                            BARK_DEBUG(errs() << "SubZeroBlock and SDivBlock is in condition!" << "\n");
                BranchInst::Create(SDivBlock, BranchBlock->end());
                BI->eraseFromParent();
                                                                                                                                                                            BARK_DEBUG(errs() << "Removed old branch instruction" << "\n");
                                                                                                                                                                            BARK_DEBUG(errs() << "Now block looks like:" << *BranchBlock << "\n");
                SubZeroBlock->dropAllReferences();
                SubZeroBlock->eraseFromParent();
                                                                                                                                                                            BARK_DEBUG(errs() << "Erased SubZeroBlock..." << "\n");
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
    // Handling -disable-divtointrinsic cl flag
    if (DisableDivToIntrinsicPass) {
        errs() << "DivToIntrinsicPass disabled" << "\n";
        return PreservedAnalyses::all();
    }
    Value *ICmpOp = nullptr;

    Module *M = F.getParent();
    if (!M) {                                                                                                                                                           BARK_DEBUG(errs() << "Function is not a part of a module!" << "\n");
        return PreservedAnalyses::none();
    }

    // Here we get current architecture.
    // This pass is specialized for arm architecture.
    // if the architectures don't match, the pass won't go further
    StringRef TargetTriple = M->getTargetTriple().str();                                                                                                                //BARK_DEBUG(errs() << "TargetTriple:  " << TargetTriple << "\n");
    bool isARM = TargetTriple.contains("arm") || TargetTriple.contains("aarch64");

    if (!isARM) {                                                                                                                                                       BARK_DEBUG(errs() << "Architecture doesт't match the pass" << "\n");
        return PreservedAnalyses::none();
    }

    for (auto &BB : F)
    {
        auto *SDivInstr = FindSDiv(&BB);
        if (!SDivInstr)
            continue;
        auto *firstOperand  = SDivInstr->getOperand(0);
        auto *secondOperand = SDivInstr->getOperand(1);

        for (auto *BP : predecessors(&BB))
        {
            auto *Term = BP->getTerminator();
            auto *BI = dyn_cast<BranchInst>(Term);
            if (!BI || !BI->isConditional()) {
                return PreservedAnalyses::none();
            }
                                                                                                                                                                        BARK_DEBUG(errs() << "SDiv's parent block is conditional!" << "\n");
            Value *Cond = BI->getCondition();                                                                                                                           BARK_DEBUG(errs() << "ICmp is " << *Cond << "\n");

            auto * TermInstr = dyn_cast<BinaryOperator>(Cond);
            if (TermInstr && TermInstr->getOpcode() == Instruction::ICmp) {

                // Here we check if there is -1 in comparsion and
                // register in icmp equals to divisor in sdiv instruction
                ICmpOp = CheckMinusOneOperands(TermInstr);
                if (!ICmpOp || (ICmpOp != secondOperand)) {
                    return PreservedAnalyses::none();
                }

                // Getting other branch of the diamond
                if (BI->getNumSuccessors() != 2)
                    return PreservedAnalyses::none();

                // Here we need to get branch without sdiv instruction
                auto *SubZeroBlock = BI->getSuccessor(1) == &BB ? BI->getSuccessor(0) : BI->getSuccessor(1);
                auto *SubZeroInstr = FindSubZeroPattern(SubZeroBlock, SDivInstr);
                if (!SubZeroInstr)
                    return PreservedAnalyses::none();

                // Finding phi node pattern
                if (FindAndTransformPhiPattern(SubZeroBlock, &BB, SDivInstr, SubZeroInstr))
                {
                                                                                                                                                                        BARK_DEBUG(errs() << "Now we need to find all ICmp users in our function and transform them" << "\n");
                    TransformICmpAndUsers(TermInstr, &BB, SubZeroBlock);
                    return PreservedAnalyses::all();
                }
            } // end of condition with ICmp instruction

            else if (TermInstr && TermInstr->getOpcode() == Instruction::And) {
                errs() << "Got and instruction!" << "\n";
                auto *firstCI =  dyn_cast<ICmpInst>(TermInstr->getOperand(0));   errs() << "first operand is"  << *firstCI  << "\n";
                auto *secondCI = dyn_cast<ICmpInst>(TermInstr->getOperand(1));   errs() << "second operand is" << *secondCI  << "\n";

                if (firstCI && secondCI) {
                    if ((ContainsInOperand(firstCI, -1) && ContainsInOperand(secondCI, INT_MIN)) || ContainsInOperand(firstCI, INT_MIN) && ContainsInOperand(secondCI, -1)) {
                        errs() << "Found pattern with -1 and INT_MIN" << "\n";
                        // Next we need to check if sdiv block and entry block have the same successor
                        // Next we check if there is a phi-node and delete the INT_MIN value from it
                        errs() << "BB successor is " << *BB.getSingleSuccessor() << "\n";
                        errs() << "BI successor is " << *BI->getSuccessor(0) << "\n";
                        if (BB.getSingleSuccessor() == BI->getSuccessor(0)) {
                            errs() << "sdiv block and entry block have the same successor" << "\n";
                            auto *Succ = BB.getSingleSuccessor();
                            for (auto &I : *Succ) {
                                if (I.getOpcode() == Instruction::PHI) {
                                    errs() << "Got phi node in successor " << I << "\n";
                                    auto *FinalInstrPhi = cast<PHINode>(&I);
                                    const uint32_t numValues = FinalInstrPhi->getNumIncomingValues();
                                    for (int i = 0; i < numValues; i++) {
                                        errs() << *FinalInstrPhi->getIncomingBlock(i) << "\n";
                                        if (FinalInstrPhi->getIncomingBlock(i) == BP) {
                                            errs() << "EntryBlock in phi node (need to erase it from here)" << "\n";
                                            FinalInstrPhi->removeIncomingValue(i);
                                            errs() << "Removed it... Now phi instruction is looking like that" << *FinalInstrPhi << "\n";
                                            BP->dropAllReferences();
                                            BP->eraseFromParent();
                                        }
                                    }

                                }
                            }
                        }

                        return PreservedAnalyses::none();

                    }
                }

            }  // end of condition with And instruction
        } // end of basic block's predecessors cycle
    } // end of basic blocks cycle

    return PreservedAnalyses::none();
}

