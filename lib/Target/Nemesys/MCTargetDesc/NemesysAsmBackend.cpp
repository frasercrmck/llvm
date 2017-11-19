#include "MCTargetDesc/NemesysFixupKinds.h"
#include "MCTargetDesc/NemesysMCTargetDesc.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCRegisterInfo.h"

namespace llvm {
class NemesysAsmBackend : public MCAsmBackend {
public:
  NemesysAsmBackend() : MCAsmBackend() {}

  unsigned getNumFixupKinds() const override {
    return Nemesys::NumTargetFixupKinds;
  }

  const MCFixupKindInfo &getFixupKindInfo(MCFixupKind) const override;

  void applyFixup(const MCAssembler &, const MCFixup &, const MCValue &,
                  MutableArrayRef<char>, uint64_t, bool) const override;

  bool mayNeedRelaxation(const MCInst &) const override { return false; }

  bool fixupNeedsRelaxation(const MCFixup &, uint64_t,
                            const MCRelaxableFragment *,
                            const MCAsmLayout &) const override {
    return false;
  }

  void relaxInstruction(const MCInst &, const MCSubtargetInfo &,
                        MCInst &) const override {
    llvm_unreachable("Not able to relax instruction");
  }

  bool writeNopData(uint64_t, MCObjectWriter *) const override;

  std::unique_ptr<MCObjectWriter>
  createObjectWriter(raw_pwrite_stream &) const override;
};

const MCFixupKindInfo &
NemesysAsmBackend::getFixupKindInfo(MCFixupKind Kind) const {
  if (Kind < FirstTargetFixupKind)
    return MCAsmBackend::getFixupKindInfo(Kind);
  assert(unsigned(Kind - FirstTargetFixupKind) < getNumFixupKinds() &&
         "Invalid kind!");
  return Nemesys::FixupInfos[Kind - FirstTargetFixupKind];
}

void NemesysAsmBackend::applyFixup(const MCAssembler &, const MCFixup &Fixup,
                                   const MCValue &, MutableArrayRef<char> Data,
                                   uint64_t Value, bool IsResolved) const {
  const unsigned NumBytes = 2;
  const unsigned Offset = Fixup.getOffset();
  for (unsigned i = 0; i < NumBytes; ++i)
    Data[Offset + i] |= uint8_t((Value >> (i * 8)) & 0xFF);
}

bool NemesysAsmBackend::writeNopData(uint64_t Count, MCObjectWriter *OW) const {
  // TODO: This is garbage
  for (uint64_t I = 0; I != Count; ++I)
    OW->write8(7);
  return true;
}

std::unique_ptr<MCObjectWriter>
NemesysAsmBackend::createObjectWriter(raw_pwrite_stream &OS) const {
  return createNemesysELFObjectWriter(OS, ELF::ELFOSABI_NONE);
}

} // namespace llvm

llvm::MCAsmBackend *llvm::createNemesysAsmBackend(const Target &,
                                                  const MCRegisterInfo &,
                                                  const Triple &, StringRef,
                                                  const MCTargetOptions &) {
  return new NemesysAsmBackend();
}
