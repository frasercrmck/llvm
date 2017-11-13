#ifndef LLVM_LIB_TARGET_NEMESYS_NEMESYSTARGETMACHINE_H
#define LLVM_LIB_TARGET_NEMESYS_NEMESYSTARGETMACHINE_H

#include "llvm/Target/TargetMachine.h"

namespace llvm {
class NemesysTargetMachine : public LLVMTargetMachine {
public:
  NemesysTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                       StringRef FS, const TargetOptions &Options,
                       Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                       CodeGenOpt::Level OL, bool JIT);

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_NEMESYS_NEMESYSTARGETMACHINE_H
