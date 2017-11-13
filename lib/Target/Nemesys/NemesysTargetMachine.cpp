#include "NemesysTargetMachine.h"
#include "Nemesys.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

extern "C" void LLVMInitializeNemesysTarget() {
  RegisterTargetMachine<NemesysTargetMachine> X(getTheNemesysTarget());
}

static std::string computeDataLayout() {
  return "e-p:32:32:32-i8:32:32-i16:32:32-n32";
}

static Reloc::Model getEffectiveRelocModel(Optional<Reloc::Model> RM) {
  return RM.hasValue() ? *RM : Reloc::Static;
}

NemesysTargetMachine::NemesysTargetMachine(const Target &T, const Triple &TT,
                                           StringRef CPU, StringRef FS,
                                           const TargetOptions &Options,
                                           Optional<Reloc::Model> RM,
                                           Optional<CodeModel::Model> CM,
                                           CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(), TT, CPU, FS, Options,
                        getEffectiveRelocModel(RM), *CM, OL) {}

namespace {
/// Nemesys Code Generator Pass Configuration Options.
class NemesysPassConfig : public TargetPassConfig {
public:
  NemesysPassConfig(NemesysTargetMachine &TM, PassManagerBase *PM)
      : TargetPassConfig(TM, *PM) {}

  NemesysTargetMachine &getNemesysTargetMachine() const {
    return getTM<NemesysTargetMachine>();
  }
};
} // namespace

TargetPassConfig *NemesysTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new NemesysPassConfig(*this, &PM);
}
