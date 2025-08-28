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

// #define DEBUG_BARKIR

#ifdef DEBUG_BARKIR
#define BARK_DEBUG(code) code
#else
#define BARK_DEBUG(code)
#endif

using namespace llvm;

namespace ark::llvmbackend::passes
{

SDivConvolution::SDivConvolution() = default;

/// Function for replacing sdiv to aarch64_sdiv intrinsic
bool ReplaceAArch64SDiv(Instruction *SDivInstr, Function *F) {
    assert(SDivInstr != nullptr);
    assert(F != nullptr);

    auto SDivIntrinsicOpcode = llvm::Intrinsic::AARCH64Intrinsics::aarch64_sdiv;
    auto SDivIntrinsicFunc = Intrinsic::getDeclaration(F->getParent(), SDivIntrinsicOpcode, {SDivInstr->getOperand(0)->getType()});
    auto SDivIntrinsicInstr = CallInst::Create(SDivIntrinsicFunc, {SDivInstr->getOperand(0), SDivInstr->getOperand(1)}, llvm::None);
    ReplaceInstWithInst(SDivInstr, SDivIntrinsicInstr);

    return true;
}

/// Function for checking operands of the instruction
/// Has two modes: checking only integer or checking both integer and value
/// Mode one is turned on when variable == nullptr (be careful!)
///
/// Returns
///     true if all args contain in instruction
///     false otherwise
bool ContainsInOperand(Instruction *I, Value *variable, const int value) {
    assert(I != nullptr);
    assert(variable != nullptr);

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
    assert(Phi != nullptr);

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
/// Finds only first sdiv !!!
/// Function for finding SDiv in a basicblock
std::vector<Instruction*> FindSDiv(BasicBlock *BB) {
    assert(BB != nullptr);

    std::vector<Instruction*> sdiv_vec;
    for (auto &I : *BB) {
        if (auto *BinOp = dyn_cast<BinaryOperator>(&I)) {
            if (BinOp->getOpcode() == Instruction::SDiv) {
                BARK_DEBUG(errs() << "Found SDivInstruction!" << "\n");
                sdiv_vec.push_back(BinOp);
            }
        }
    }
    return sdiv_vec;
}

/// Help function for clearingBasicBlock
void clearBasicBlock(BasicBlock *BB) {
    assert(BB != nullptr);

    while (!BB->empty()) {
        Instruction *I = &(BB->back());
        I->eraseFromParent();
    }
}

/// Function for finding PhiNode in users of the other instruction
PHINode* FindPhiInUses(Instruction *Instr) {
    assert(Instr != nullptr);

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

/// Function for getting sub instruction from phi instruction
Instruction* GetSubInstrFromPhi(Instruction *PhiInstr) {
    assert(PhiInstr != nullptr);

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
    assert(SubInstr != nullptr);
    assert(SDivInstr != nullptr);

    return (SubInstr->getOperand(1) == SDivInstr->getOperand(0) || SubInstr->getOperand(1) == SDivInstr->getOperand(1));
}

/// Function for transforming our pattern
/// Pipeline:
///     find phi as a user of sdiv ->
///         -> get sub or int_min from it ->
///             -> replace phi with sdiv
///                 -> replace sdiv to aarch64
bool FinalTransform(Instruction *SDivInstr, Function *F, BasicBlock *BB) {
    assert(SDivInstr != nullptr);
    assert(F != nullptr);
    assert(BB != nullptr);

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
            ReplaceAArch64SDiv(clonedSDiv, F);
            BARK_DEBUG(errs() << "replaced sdiv with aarch64_sdiv" << BB << "\n");

            BARK_DEBUG(errs() << "SDivConvolution PASSED! >__< :: function -> " << F->getName() << "\n");
            return true;
        }
    }
    return false;
}

void PrintRecursively(const char *word, uint32_t level) {
    assert(word != nullptr);

        for (uint32_t i = 0; i < level; i++)
            errs() << "\t";
        errs() << word;
}

Instruction *getSingleUser(Value *val) {
    assert(val != nullptr);

    int numUses = val->getNumUses();
    BARK_DEBUG(errs() << "Value : " << *val << " got " << numUses << " uses." << "\n");
    if (numUses == 1) {
        for (auto *User : val->users()) {
            auto *castedUser = dyn_cast<Instruction>(User);
            return castedUser;
        }
    }
        return nullptr;
}


Instruction *getUserByNumber(Instruction *StartOp, Value *val, uint32_t num, uint32_t numUses) {
    assert(StartOp != nullptr);
    assert(val != nullptr);

    BARK_DEBUG(errs() << "==================================================================" << "\n");
    BARK_DEBUG(errs() << "\t\t" << "value : " << *val << "\n");
    uint32_t cnt = 0;
    for (auto *User : val->users()) {
        if (cnt == num) {
            BARK_DEBUG(errs() << "\t\tuser " << cnt << " : " << *User << "\n");
            auto *castedUser = dyn_cast<Instruction>(User);
            if (castedUser != StartOp)
                return castedUser;
        } else if (cnt > num)
            break;

        cnt++;
    }

    return nullptr;
}

// std::vector<Value*> findCommonOperands(Instruction *I1, Instruction *I2) {
//     std::vector<Value*> commonOperands;
//
//     std::vector<Value*> operands1;
//     for (uint32_t i = 0; i < I1->getNumOperands(); i++) {
//         operands1.push_back(I1->getOperand(i));
//     }
//
//     std::vector<Value*> operands2;
//     for (uint32_t i = 0; i < I2->getNumOperands(); i++) {
//         operands1.push_back(I2->getOperand(i));
//     }
//
//     std::sort(operands1.begin(), operands1.end());
//     std::sort(operands2.begin(), operands2.end());
//
//     std::set_intersection(operands1.begin(). operands1.end(),
//                           operands2.begin(), operands2.end().
//                           std::back_inserter(commonOperands));
//
//     return commonOperands;
// }


/// WARNING!!!
/// AND INFINITE RECURSION CASE NOT HANDLED
/// THIS IS THE CAS
// WARNING!!!

/// This a function for recursive searching of icmp
/// It goes from the top of the tree (can be `and` instruction or `icmp` as well)
///
///    here's how it looks like
///
///           and
///          /    \
///        and     and
///       /  \    /  \
///     icmp and icmp icmp
///
/// if `and` -> go deeper
/// else if `icmp` -> stop and search for correct pattern
/// else -> do nothing
bool funcRecursiveICmpSearch(Instruction *StartOp, Value *val, const int32_t num, uint32_t level) {
    assert(StartOp != nullptr);
    assert(val != nullptr);

    if (StartOp->getOpcode() == Instruction::ICmp) { // CHECK IF ICMP IN EQ MODE !!!!!!!!
        auto *castStartOp = dyn_cast<ICmpInst>(StartOp);
        if (castStartOp->getPredicate() != CmpInst::ICMP_EQ)
            return false;
        // PrintRecursively("icmp", level);
        PrintRecursively("", level);
        BARK_DEBUG(errs() << "Got icmp in recursion with eq predicate -> " << *StartOp << "\n");
        return ContainsInOperand(StartOp, val, num);
    } else if (StartOp->getOpcode() == Instruction::And || (StartOp->getOpcode() == Instruction::SDiv && level == 0)) {
        // PrintRecursively("and", level);
        PrintRecursively(" ", level);
        BARK_DEBUG(errs() << "Got" << *StartOp << " in recursion" << " ::");
        uint32_t numUses1 = StartOp->getOperand(0)->getNumUses();
        BARK_DEBUG(errs() << *(StartOp->getOperand(0)) << " got " << numUses1 << "     uses" << "\n");
        for (uint32_t cntUses = 0; cntUses < numUses1; cntUses++) {
            auto *userLeft = getUserByNumber(StartOp, StartOp->getOperand(0), cntUses, numUses1);
            if (userLeft && funcRecursiveICmpSearch(userLeft, val, num, level+1))
                return true;
        }

        uint32_t numUses2 = StartOp->getOperand(1)->getNumUses();
        for (uint32_t cntUses = 0; cntUses < numUses2; cntUses++) {
            auto *userRight = getUserByNumber(StartOp, StartOp->getOperand(1), cntUses, numUses1);
            if (userRight && funcRecursiveICmpSearch(userRight, val, num, level+1))
                return true;
        }
    }
    return false;
}
// bool -> contains
// pointer
bool RecursiveICmpSearch(Instruction *StartOp, Value *val, const int32_t num) {
    assert(StartOp != nullptr);
    assert(val != nullptr);

    BARK_DEBUG(errs() << "Starting recursive icmp search!" << "\n");
    BARK_DEBUG(errs() << "StartOp = " << *StartOp << " : Value = " << *val << " : Num = " << num << "\n");

    return funcRecursiveICmpSearch(StartOp, val, num, 0);

}

llvm::PreservedAnalyses SDivConvolution::run(Function &F,
                                        FunctionAnalysisManager &AM) {

    bool changed = false;
    Module *M = F.getParent();
    if (!M) {
        BARK_DEBUG(errs() << "Function is not a part of a module!" << "\n");
        return PreservedAnalyses::none();
    }

    BARK_DEBUG(errs() << F << "\n");

    /// This pass is specialized for arm architecture.
    /// if the architectures don't match, the pass won't go further
    StringRef TargetTriple = M->getTargetTriple();

    // simplify arm check
    // (llvm_ark_interface (LLVMArchToArkArch))
    bool isARM = TargetTriple.contains("arm") || TargetTriple.contains("aarch64");
    if (!isARM) {
        BARK_DEBUG(errs() << "Architecture doesn't match the pass" << "\n");
        return PreservedAnalyses::none();
    }

    for (auto &BB : F) {
        BARK_DEBUG(errs() << "Running cycle... the block is -> -> ->" << BB << "\n");

        auto SDivInstrVec = FindSDiv(&BB);
        BARK_DEBUG(errs() << "Found SDivVector with size " << SDivInstrVec.size() << SDivInstrVec << "\n");

        for (auto *SDivInstr : SDivInstrVec) {
            auto *firstOperand = SDivInstr->getOperand(0);
            auto *secondOperand = SDivInstr->getOperand(1);

            if (RecursiveICmpSearch(SDivInstr, secondOperand, -1)) {
                if (FinalTransform(SDivInstr, &F, &BB)) {
                    changed = true;
                    continue;
                }
            } else if (RecursiveICmpSearch(SDivInstr, firstOperand, INT_MIN) && RecursiveICmpSearch(SDivInstr, secondOperand, -1)) {
                if (FinalTransform(SDivInstr, &F, &BB)) {
                    changed = true;
                    continue;
                }
            }
        } // end of SDivInstrVec cycle
    } // end of basic blocks cycle

    BARK_DEBUG(errs() << "==================================" << "\n");
    BARK_DEBUG(errs() << F << "\n");
    BARK_DEBUG(errs() << "==================================" << "\n");

    return changed ? llvm::PreservedAnalyses::none() : llvm::PreservedAnalyses::all();
}

}


