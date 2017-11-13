#ifndef LLVM_LIB_TARGET_NEMESYS_NEMESYSFRAMELOWERING_H
#define LLVM_LIB_TARGET_NEMESYS_NEMESYSFRAMELOWERING_H

#include "Nemesys.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {

class BitVector;
class NemesysSubtarget;

class NemesysFrameLowering : public TargetFrameLowering {
protected:
  const NemesysSubtarget &STI;

public:
  explicit NemesysFrameLowering(const NemesysSubtarget &Subtarget)
      : TargetFrameLowering(StackGrowsDown,
                            /*StackAlignment=*/4,
                            /*LocalAreaOffset=*/0),
        STI(Subtarget) {}
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_NEMESYS_NEMESYSFRAMELOWERING_H
