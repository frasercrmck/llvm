#ifndef LLVM_LIB_TARGET_NEMESYS_NEMESYSREGISTERINFO_H
#define LLVM_LIB_TARGET_NEMESYS_NEMESYSREGISTERINFO_H

#include "Nemesys.h"
#include "llvm/Target/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "NemesysGenRegisterInfo.inc"

namespace llvm {
class NemesysRegisterInfo : public NemesysGenRegisterInfo {
public:
  NemesysRegisterInfo();
};

} // end namespace llvm
#endif // LLVM_LIB_TARGET_NEMESYS_NEMESYSREGISTERINFO_H
