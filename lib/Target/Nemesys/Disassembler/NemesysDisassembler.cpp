#include "MCTargetDesc/NemesysMCTargetDesc.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCDisassembler/MCDisassembler.h"
#include "llvm/MC/MCFixedLenDisassembler.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/Endian.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "nemesys-disassembler"

typedef MCDisassembler::DecodeStatus DecodeStatus;

namespace {
class NemesysDisassembler : public MCDisassembler {

public:
  NemesysDisassembler(const MCSubtargetInfo &STI, MCContext &Ctx)
      : MCDisassembler(STI, Ctx) {}

  DecodeStatus getInstruction(MCInst &, uint64_t &, ArrayRef<uint8_t>, uint64_t,
                              raw_ostream &, raw_ostream &) const override;
};
} // end anonymous namespace

static MCDisassembler *createNemesysDisassembler(const Target &T,
                                                 const MCSubtargetInfo &STI,
                                                 MCContext &Ctx) {
  return new NemesysDisassembler(STI, Ctx);
}

extern "C" void LLVMInitializeNemesysDisassembler() {
  TargetRegistry::RegisterMCDisassembler(getTheNemesysTarget(),
                                         createNemesysDisassembler);
}

static const unsigned RCDecoderTable[] = {
    Nemesys::R0,  Nemesys::R1,  Nemesys::R2,  Nemesys::R3,  Nemesys::R4,
    Nemesys::R5,  Nemesys::R6,  Nemesys::R7,  Nemesys::R8,  Nemesys::R9,
    Nemesys::R10, Nemesys::R11, Nemesys::R12, Nemesys::R13, Nemesys::R14,
    Nemesys::R15, Nemesys::R16, Nemesys::R17, Nemesys::R18, Nemesys::R19,
    Nemesys::R20, Nemesys::R21, Nemesys::R22, Nemesys::R23, Nemesys::R24,
    Nemesys::R25, Nemesys::R26, Nemesys::R27, Nemesys::R28, Nemesys::R29,
    Nemesys::R30, Nemesys::R31};

static const unsigned PCDecoderTable[] = {Nemesys::P0, Nemesys::P1, Nemesys::P2,
                                          Nemesys::P3, Nemesys::P4, Nemesys::P5,
                                          Nemesys::P6, Nemesys::R7};

static DecodeStatus DecodeRegister(MCInst &MI, uint64_t RegNo,
                                   const unsigned *RegTable,
                                   size_t RegTableSize) {
  if (RegNo > RegTableSize)
    return MCDisassembler::Fail;
  MI.addOperand(MCOperand::createReg(RegTable[RegNo]));
  return MCDisassembler::Success;
}

static DecodeStatus DecodeRCRegisterClass(MCInst &MI, uint64_t RegNo,
                                          uint64_t Address,
                                          const void *Decoder) {
  return DecodeRegister(MI, RegNo, RCDecoderTable, sizeof(RCDecoderTable));
}

static DecodeStatus DecodePCRegisterClass(MCInst &MI, uint64_t RegNo,
                                          uint64_t Address,
                                          const void *Decoder) {
  return DecodeRegister(MI, RegNo, PCDecoderTable, sizeof(PCDecoderTable));
}

#include "NemesysGenDisassemblerTables.inc"

DecodeStatus NemesysDisassembler::getInstruction(MCInst &MI, uint64_t &Size,
                                                 ArrayRef<uint8_t> Bytes,
                                                 uint64_t Address,
                                                 raw_ostream &OS,
                                                 raw_ostream &CS) const {
  Size = 4;
  if (Bytes.size() < 4) {
    Size = 0;
    return MCDisassembler::Fail;
  }

  uint32_t Inst = support::endian::read32le(Bytes.data());
  return decodeInstruction(DecoderTable32, MI, Inst, Address, this, STI);
}
