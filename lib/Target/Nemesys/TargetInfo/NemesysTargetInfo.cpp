#include "Nemesys.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheNemesysTarget() {
  static Target TheNemesysTarget;
  return TheNemesysTarget;
}

extern "C" void LLVMInitializeNemesysTargetInfo() {
  RegisterTarget<Triple::nemesys> X(getTheNemesysTarget(), "nemesys", "Nemesys");
}
