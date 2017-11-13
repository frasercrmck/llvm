#include "NemesysRegisterInfo.h"
#include "Nemesys.h"
#include "NemesysFrameLowering.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Target/TargetSubtargetInfo.h"

#define GET_REGINFO_TARGET_DESC
#include "NemesysGenRegisterInfo.inc"

using namespace llvm;

NemesysRegisterInfo::NemesysRegisterInfo()
    : NemesysGenRegisterInfo(Nemesys::R31) {}
