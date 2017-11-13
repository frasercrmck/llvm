#include "NemesysInstPrinter.h"
#include "Nemesys.h"
#include "llvm/MC/MCInst.h"

using namespace llvm;

#include "NemesysGenAsmWriter.inc"

void NemesysInstPrinter::printInst(const MCInst *MI, raw_ostream &OS,
                                   StringRef Annotation,
                                   const MCSubtargetInfo & /*STI*/) {
  printInstruction(MI, OS);
}

void NemesysInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                      raw_ostream &OS, const char *Modifier) {
  assert((Modifier == 0 || Modifier[0] == 0) && "No modifiers supported");
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg())
    OS << getRegisterName(Op.getReg());
  else
    llvm_unreachable("Can only print Nemesys registers");
}
