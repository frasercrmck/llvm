#include "NemesysFixupKinds.h"
#include "NemesysMCTargetDesc.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/Support/raw_ostream.h"
#include <cstdint>

using namespace llvm;

namespace {
class NemesysMCCodeEmitter : public MCCodeEmitter {
  const MCContext &CTX;

public:
  NemesysMCCodeEmitter(const MCInstrInfo &MCII, MCContext &C) : CTX(C) {}

  unsigned getMachineOpValue(const MCInst &, const MCOperand &,
                             SmallVectorImpl<MCFixup> &,
                             const MCSubtargetInfo &) const;
  unsigned getPCRel16Value(const MCInst &, const unsigned,
                           SmallVectorImpl<MCFixup> &,
                           const MCSubtargetInfo &) const;
  unsigned getNegatablePredValue(const MCInst &, const unsigned,
                                 SmallVectorImpl<MCFixup> &,
                                 const MCSubtargetInfo &) const;
  uint64_t getBinaryCodeForInstr(const MCInst &, SmallVectorImpl<MCFixup> &,
                                 const MCSubtargetInfo &) const;
  void encodeInstruction(const MCInst &, raw_ostream &,
                         SmallVectorImpl<MCFixup> &,
                         const MCSubtargetInfo &) const override;
};
} // namespace

unsigned
NemesysMCCodeEmitter::getMachineOpValue(const MCInst &, const MCOperand &MO,
                                        SmallVectorImpl<MCFixup> &,
                                        const MCSubtargetInfo &) const {
  if (MO.isReg()) {
    unsigned Reg = MO.getReg();
    return CTX.getRegisterInfo()->getEncodingValue(Reg);
  } else if (MO.isImm())
    return static_cast<unsigned>(MO.getImm());
  llvm_unreachable("Unknown operand to encode");
}

void NemesysMCCodeEmitter::encodeInstruction(const MCInst &MI, raw_ostream &OS,
                                             SmallVectorImpl<MCFixup> &Fixups,
                                             const MCSubtargetInfo &STI) const {
  auto Val = getBinaryCodeForInstr(MI, Fixups, STI);

  for (unsigned i = 0; i < 4; i++)
    OS << (unsigned char)((Val >> (i * 8)) & 0xFF);
}

unsigned NemesysMCCodeEmitter::getPCRel16Value(const MCInst &MI,
                                               const unsigned OpIdx,
                                               SmallVectorImpl<MCFixup> &Fixups,
                                               const MCSubtargetInfo &) const {
  const MCOperand &MO = MI.getOperand(OpIdx);
  if (MO.isImm())
    return MO.getImm();

  assert(MO.isExpr() && "Unexpected branch target type");
  const MCExpr *Expr = MO.getExpr();
  MCFixupKind Kind = MCFixupKind(Nemesys::fixup_nemesys_pcrel16);
  Fixups.push_back(MCFixup::create(0, Expr, Kind, MI.getLoc()));

  return 0;
}

unsigned NemesysMCCodeEmitter::getNegatablePredValue(
    const MCInst &MI, const unsigned OpIdx, SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &) const {
  const MCOperand &RegMO = MI.getOperand(OpIdx);
  const MCOperand &ImmMO = MI.getOperand(OpIdx + 1);

  assert(RegMO.isReg() && ImmMO.isImm() && "Invalid negatable predicate");
  unsigned RegVal = CTX.getRegisterInfo()->getEncodingValue(RegMO.getReg());
  return ((ImmMO.getImm() & 0x1) << 3) | (RegVal & 0x7);
}

#include "NemesysGenMCCodeEmitter.inc"

llvm::MCCodeEmitter *
llvm::createNemesysMCCodeEmitter(const MCInstrInfo &InstrInfo,
                                 const MCRegisterInfo &, MCContext &Context) {
  return new NemesysMCCodeEmitter(InstrInfo, Context);
}
