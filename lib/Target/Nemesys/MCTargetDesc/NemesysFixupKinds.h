#ifndef LLVM_LIB_TARGET_NEMESYS_MCTARGETDESC_NEMESYSFIXUPKINDS_H
#define LLVM_LIB_TARGET_NEMESYS_MCTARGETDESC_NEMESYSFIXUPKINDS_H

#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCFixupKindInfo.h"

namespace llvm {
namespace Nemesys {
enum Fixups {
  fixup_nemesys_pcrel16 = FirstTargetFixupKind,
  // Marker
  LastTargetFixupKind,
  NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
};

const static MCFixupKindInfo FixupInfos[Nemesys::NumTargetFixupKinds] = {
    // This table *must* be in the order that the fixup_* kinds are defined
    // Name                      Offset (bits) Size (bits)     Flags
    {"fixup_nemesys_pcrel16", 0, 16, MCFixupKindInfo::FKF_IsPCRel}};
} // namespace Nemesys
} // namespace llvm

#endif // LLVM_LIB_TARGET_NEMESYS_MCTARGETDESC_NEMESYSFIXUPKINDS_H
