#ifndef LLVM_LIB_TARGET_NEMESYS_MCTARGETDESC_NEMESYSMCTARGETDESC_H
#define LLVM_LIB_TARGET_NEMESYS_MCTARGETDESC_NEMESYSMCTARGETDESC_H

namespace llvm {
class Target;
class Triple;

Target &getTheNemesysTarget();

} // namespace llvm

#define GET_REGINFO_ENUM
#include "NemesysGenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "NemesysGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "NemesysGenSubtargetInfo.inc"

#endif
