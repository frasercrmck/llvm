#include "MCTargetDesc/NemesysFixupKinds.h"
#include "MCTargetDesc/NemesysMCTargetDesc.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCValue.h"

using namespace llvm;

namespace {
class NemesysELFObjectWriter : public MCELFObjectTargetWriter {
public:
  NemesysELFObjectWriter(uint8_t);

  unsigned getRelocType(MCContext &, const MCValue &, const MCFixup &,
                        bool) const override;
};
} // namespace

NemesysELFObjectWriter::NemesysELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(/* Is64Bit */ false, OSABI, ELF::EM_NEMESYS,
                              /* HasRelocationAddend */ false) {}

unsigned NemesysELFObjectWriter::getRelocType(MCContext &Ctx,
                                              const MCValue &Target,
                                              const MCFixup &Fixup,
                                              bool IsPCRel) const {
  return (unsigned)ELF::R_NEMESYS_PC16;
}

std::unique_ptr<MCObjectWriter>
llvm::createNemesysELFObjectWriter(raw_pwrite_stream &OS, uint8_t OSABI) {
  return createELFObjectWriter(llvm::make_unique<NemesysELFObjectWriter>(OSABI),
                               OS, /*IsLittleEndian*/ true);
}
