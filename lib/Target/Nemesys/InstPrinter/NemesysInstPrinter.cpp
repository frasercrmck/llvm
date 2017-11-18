#include "NemesysInstPrinter.h"
#include "MCTargetDesc/NemesysBaseInfo.h"
#include "Nemesys.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/FormattedStream.h"

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
  else if (Op.isImm())
    OS << '#' << Op.getImm();
  else if (Op.isExpr()) {
    OS << '#';
    Op.getExpr()->print(OS, &MAI);
  } else
    llvm_unreachable("Can only print registers, immediates, and expressions");
}

void NemesysInstPrinter::printCondCode(const MCInst *MI, unsigned OpNo,
                                       raw_ostream &OS) const {
  const MCOperand &Op = MI->getOperand(OpNo);
  assert(Op.isImm() && "CondCode operand must be immediate");
  auto Val = static_cast<CondCode>(Op.getImm());
  assert(CondCodeStrs.count(Val) && "Invalid CondCode operand");
  OS << CondCodeStrs.find(Val)->second;
}
