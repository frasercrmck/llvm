#include "NemesysMCTargetDesc.h"
#include "InstPrinter/NemesysInstPrinter.h"
#include "NemesysMCAsmInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "NemesysGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "NemesysGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "NemesysGenRegisterInfo.inc"

static MCInstrInfo *createNemesysMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitNemesysMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createNemesysMCRegisterInfo(const Triple & /*TT*/) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitNemesysMCRegisterInfo(X, Nemesys::R31);
  return X;
}

static MCInstPrinter *createNemesysMCInstPrinter(const Triple & /*T*/,
                                                 unsigned SyntaxVariant,
                                                 const MCAsmInfo &MAI,
                                                 const MCInstrInfo &MII,
                                                 const MCRegisterInfo &MRI) {
  return new NemesysInstPrinter(MAI, MII, MRI);
}

static MCSubtargetInfo *
createNemesysMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  return createNemesysMCSubtargetInfoImpl(TT, CPU, FS);
}

extern "C" void LLVMInitializeNemesysTargetMC() {
  RegisterMCAsmInfo<NemesysMCAsmInfo> X(getTheNemesysTarget());

  TargetRegistry::RegisterMCInstrInfo(getTheNemesysTarget(),
                                      createNemesysMCInstrInfo);

  TargetRegistry::RegisterMCRegInfo(getTheNemesysTarget(),
                                    createNemesysMCRegisterInfo);

  TargetRegistry::RegisterMCInstPrinter(getTheNemesysTarget(),
                                        createNemesysMCInstPrinter);

  TargetRegistry::RegisterMCCodeEmitter(getTheNemesysTarget(),
                                        createNemesysMCCodeEmitter);

  TargetRegistry::RegisterMCAsmBackend(getTheNemesysTarget(),
                                       createNemesysAsmBackend);

  TargetRegistry::RegisterMCSubtargetInfo(getTheNemesysTarget(),
                                          createNemesysMCSubtargetInfo);
}
