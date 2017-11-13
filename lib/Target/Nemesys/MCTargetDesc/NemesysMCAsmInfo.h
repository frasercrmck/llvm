#ifndef LLVM_LIB_TARGET_NEMESYS_MCTARGETDESC_NEMESYSMCASMINFO_H
#define LLVM_LIB_TARGET_NEMESYS_MCTARGETDESC_NEMESYSMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class NemesysMCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit NemesysMCAsmInfo(const Triple &TheTriple);
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_NEMESYS_MCTARGETDESC_NEMESYSMCASMINFO_H
