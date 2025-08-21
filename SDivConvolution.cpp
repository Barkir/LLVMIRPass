// #include "llvm/Transforms/Utils/DivToIntrinsic.h"


#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/CFG.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IntrinsicsAArch64.h"
#include "limits.h"


#include "aarch64_sdiv_convolution.h"
#include "llvm_ark_interface.h"
#include "transforms/transform_utils.h"

#define DEBUG_BARKIR

#ifdef DEBUG_BARKIR
#define BARK_DEBUG(code) code
#else
#define BARK_DEBUG(code)
#endif

using namespace llvm;

namespace ark::llvmbackend::passes {

SDivConvolution::SDivConvolution() = default;

/// Function for replacing sdiv to aarch64_sdiv intrinsic
bool ReplaceAArch64SDiv(Instruction *SDivInstr, Function *F) {
    if (!SDivInstr || !F)
        return false;

    auto SDivIntrinsicOpcode = llvm::Intrinsic::AARCH64Intrinsics::aarch64_sdiv;
    auto SDivIntrinsicFunc = Intrinsic::getDeclaration(F->getParent(), SDivIntrinsicOpcode, {SDivInstr->getOperand(0)->getType()});
    auto SDivIntrinsicInstr = CallInst::Create(SDivIntrinsicFunc, {SDivInstr->getOperand(0), SDivInstr->getOperand(1)}, llvm::None);
    ReplaceInstWithInst(SDivInstr, SDivIntrinsicInstr);

    return true;
}

void printArgumentTypes(llvm::Function *F) {
    for (llvm::Argument& Arg : F->args()) {
        llvm::Type *ArgType = Arg.getType();
        errs() << Arg << " type is: " << *ArgType << "\n";
    }
}

/// Function for checking operands of the instruction
/// Has two modes: checking only integer or checking both integer and value
/// Mode one is turned on when variable == nullptr (be careful!)
///
/// Returns
///     true if all args contain in instruction
///     false otherwise
bool ContainsInOperand(Instruction *I, Value *variable, const int value) {
    int numOper = I->getNumOperands();
    bool valueCond = false;
    bool operCond = false;

    for (int i = 0; i < numOper; i++) {
        auto *oper  = I->getOperand(i);
        auto *oper2int = dyn_cast<ConstantInt>(oper);
        if (oper2int && oper2int->getSExtValue() == value) {
            BARK_DEBUG(errs() << "oper #" << i << ": " << *oper << " ");
            BARK_DEBUG(errs() << "compared to value " << value << ":" << oper2int->getSExtValue() << "\n");
            operCond = true;
        }
        else if (variable && oper == variable){
            BARK_DEBUG(errs() << "oper equals to 1st arg" << *variable << "\n");
            valueCond = true;
        }

    }

    return (!value && operCond) || (valueCond && operCond);
}

/// Function for checking if PHINode contains 'value'
///
/// Returns
///     true if value contains in phi
///     false otherwise
bool ContainsInPhi(PHINode *Phi, const int value) {
    int numValues = Phi->getNumIncomingValues();
    for (int i = 0; i < numValues; i++) {
        auto *phi_value = Phi->getIncomingValue(i);
        auto *phi_value2int = dyn_cast<ConstantInt>(phi_value);
        if (phi_value2int && phi_value2int->getSExtValue() == value) {
            BARK_DEBUG(errs() << "value #" << i << ": " << *phi_value << " ");
            BARK_DEBUG(errs() << "compared to value " << value << ":" << phi_value2int->getSExtValue() << "\n");
            return true;
        }
    }
    return false;
}

/// Function for finding SDiv in a basicblock
Instruction* FindSDiv(BasicBlock *BB) {
    for (auto &I : *BB) {
        if (auto *BinOp = dyn_cast<BinaryOperator>(&I)) {
            if (BinOp->getOpcode() == Instruction::SDiv) {
                BARK_DEBUG(errs() << "Found SDivInstruction!" << "\n");
                return &I;
            }
        }
    }
    return nullptr;
}

/// Help function for clearingBasicBlock
void clearBasicBlock(BasicBlock *BB) {
    while (!BB->empty()) {
        Instruction *I = &(BB->back());
        I->eraseFromParent();
    }
}

/// Function for finding PhiNode in users of the other instruction
PHINode* FindPhiInUses(Instruction *Instr) {

    if (Instr->getNumUses() == 1)
    {
        for (auto *User : Instr->users()) {
            auto *PhiInstr = dyn_cast<Instruction>(User);
            if (PhiInstr && PhiInstr->getOpcode() == Instruction::PHI) {
                auto *PhiInstrCasted = dyn_cast<PHINode>(PhiInstr);
                return PhiInstrCasted;
            }
        }
    }
    return nullptr;
}

/// Function for getting sub instructionf from phi instruction
Instruction* GetSubInstrFromPhi(Instruction *PhiInstr) {
    PHINode *PhiInstrCasted = dyn_cast<PHINode>(PhiInstr);
    uint64_t numVal = PhiInstrCasted->getNumIncomingValues();

    for (uint64_t i = 0; i < numVal; i++) {
        auto *PotSub = PhiInstrCasted->getIncomingValue(i);
        auto *PotSubCasted = dyn_cast<Instruction>(PotSub);
        if (PotSubCasted && PotSubCasted->getOpcode() == Instruction::Sub)
            return PotSubCasted;
    }

    return nullptr;
}

/// Function for checking correct operands in sub and sdiv instruction
bool SubInstrAppropriate(Instruction *SubInstr, Instruction *SDivInstr) {
    return (SubInstr->getOperand(1) == SDivInstr->getOperand(0) || SubInstr->getOperand(1) == SDivInstr->getOperand(1));
}

/// Function for transforming our pattern
/// Pipeline:
///     find phi as a user of sdiv ->
///         -> get sub or int_min from it ->
///             -> replace phi with sdiv
///                 -> replace sdiv to aarch64
bool FinalTransform(Instruction *SDivInstr, Function *F, BasicBlock *BB) {
    auto *PhiInstr = FindPhiInUses(SDivInstr);
    if (PhiInstr) {
        BARK_DEBUG(errs() << "Found Phi in uses of sdiv " << *PhiInstr << "\n");

        auto *SubInstr = GetSubInstrFromPhi(PhiInstr);
        if ((!SubInstr && ContainsInPhi(PhiInstr, INT_MIN)) ||
        (SubInstr && SubInstrAppropriate(SubInstr, SDivInstr))) {
            auto *clonedSDiv = SDivInstr->clone();
            ReplaceInstWithInst(PhiInstr, clonedSDiv);
            BARK_DEBUG(errs() << "replaced phi instruction with sdiv" << *(BB->getSingleSuccessor()) << "\n");

            ReplaceAArch64SDiv(SDivInstr, F);
            BARK_DEBUG(errs() << "replaced sdiv with aarch64_sdiv" << BB << "\n");

            errs() << "SDivConvolution PASSED! >__< :: function -> " << F->getName() << "\n";
            return true;
        }
    }
    return false;
}

llvm::PreservedAnalyses SDivConvolution::run(Function &F,
                                          FunctionAnalysisManager &AM) {

    Module *M = F.getParent();
    if (!M) {
        BARK_DEBUG(errs() << "Function is not a part of a module!" << "\n");
        return PreservedAnalyses::none();
    }

    /// This pass is specialized for arm architecture.
    /// if the architectures don't match, the pass won't go further
    StringRef TargetTriple = M->getTargetTriple();
    bool isARM = TargetTriple.contains("arm") || TargetTriple.contains("aarch64");
    if (!isARM) {
        BARK_DEBUG(errs() << "Architecture doesn't match the pass" << "\n");
        return PreservedAnalyses::none();
    }

    for (auto &BB : F) {
        BARK_DEBUG(errs() << "Running cycle... the block is -> -> ->" << BB << "\n");

        auto *SDivInstr = FindSDiv(&BB);
        if (!SDivInstr)
            continue;
        BARK_DEBUG(errs() << "Found SDiv! " << *SDivInstr << "\n");

        auto *firstOperand = SDivInstr->getOperand(0);
        auto *secondOperand = SDivInstr->getOperand(1);
        auto *BP = BB.getSinglePredecessor();
        if (!BP)
            return PreservedAnalyses::none();
        BARK_DEBUG(errs() << "SDiv got predecessor " << *BP << "\n");

        auto *Term = BP->getTerminator();
        auto *BI = dyn_cast<BranchInst>(Term);
        if (!BI || !BI->isConditional()) {
            return PreservedAnalyses::none();
        }
        BARK_DEBUG(errs() << "SDiv's parent block is conditional!" << "\n");

        Value *Cond = BI->getCondition();
        BARK_DEBUG(errs() << "Condition is " << *Cond << "\n");

        auto * TermInstr = dyn_cast<Instruction>(Cond);
        if (TermInstr && TermInstr->getOpcode() == Instruction::ICmp) {
            // Check if there is -1 in comparsion and
            // register in icmp equals to divisor in sdiv instruction
            if (ContainsInOperand(TermInstr, secondOperand, -1))
            {
                BARK_DEBUG(errs() << "-1 and " << *secondOperand << "containing in " << *TermInstr << "\n");
                if (FinalTransform(SDivInstr, &F, &BB))
                    return PreservedAnalyses::all();
            }

        } else if (TermInstr && TermInstr->getOpcode() == Instruction::And) {
            BARK_DEBUG(errs() << "Got and instruction!" << "\n");

            auto *firstCI =  dyn_cast<ICmpInst>(TermInstr->getOperand(0));
            auto *secondCI = dyn_cast<ICmpInst>(TermInstr->getOperand(1));

            if (firstCI && secondCI) {

                BARK_DEBUG(errs() << "first operand is"  << *firstCI  << "\n");
                BARK_DEBUG(errs() << "second operand is" << *secondCI  << "\n");

                if ((ContainsInOperand(firstCI, secondOperand, -1) && ContainsInOperand(secondCI, firstOperand, INT_MIN)) ||
                (ContainsInOperand(firstCI, firstOperand, INT_MIN) && ContainsInOperand(secondCI, secondOperand, -1))) {
                    if (FinalTransform(SDivInstr, &F, &BB))
                        return PreservedAnalyses::all();
                }
            }
        }  // end of condition with And instruction
    } // end of basic blocks cycle

    return PreservedAnalyses::none();
}


}
