include "llvm/Target/Target.td"
include "NemesysRegisterInfo.td"
include "NemesysInstrInfo.td"

def NemesysInstrInfo : InstrInfo;

def Nemesys : Target {
  let InstructionSet = NemesysInstrInfo;
}
