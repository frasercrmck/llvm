include "llvm/Target/Target.td"
include "NemesysRegisterInfo.td"
include "NemesysInstrInfo.td"

def NemesysInstrInfo : InstrInfo;

def NemesysAsmParser : AsmParser {
  bit ReportMultipleNearMisses = 1;
}

def NemesysAsmParserVariant : AsmParserVariant {
  string SeparatorCharacters = " \t,.";
}

def Nemesys : Target {
  let AssemblyParsers = [NemesysAsmParser];
  let AssemblyParserVariants = [NemesysAsmParserVariant];
  let InstructionSet = NemesysInstrInfo;
}
