include "llvm/Target/Target.td"
include "NemesysRegisterInfo.td"
include "NemesysInstrInfo.td"

def NemesysInstrInfo : InstrInfo;

def NemesysAsmParser : AsmParser {
  bit ReportMultipleNearMisses = 1;
}

def Nemesys : Target {
  let AssemblyParsers = [NemesysAsmParser];
  let InstructionSet = NemesysInstrInfo;
}
