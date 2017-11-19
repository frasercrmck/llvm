#ifndef LLVM_LIB_TARGET_NEMESYS_MCTARGETDESC_NEMESYSMCTARGETDESC_H
#define LLVM_LIB_TARGET_NEMESYS_MCTARGETDESC_NEMESYSMCTARGETDESC_H

#include "llvm/ADT/StringRef.h"

namespace llvm {
class Target;
class Triple;
class MCRegisterInfo;
class MCAsmBackend;
class MCTargetOptions;

Target &getTheNemesysTarget();

MCAsmBackend *createNemesysAsmBackend(const Target &, const MCRegisterInfo &,
                                      const Triple &, StringRef,
                                      const MCTargetOptions &);

} // namespace llvm

#define GET_REGINFO_ENUM
#include "NemesysGenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "NemesysGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "NemesysGenSubtargetInfo.inc"

#endif
