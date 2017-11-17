#include "NemesysMCAsmInfo.h"
#include "llvm/ADT/Triple.h"

using namespace llvm;

void NemesysMCAsmInfo::anchor() {}

NemesysMCAsmInfo::NemesysMCAsmInfo(const Triple & /*TheTriple*/) {
  IsLittleEndian = true;
  CommentString = ";";
  UseIntegratedAssembler = true;
}
