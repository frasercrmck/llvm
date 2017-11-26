#ifndef LLVM_LIB_TARGET_NEMESYS_MCTARGETDESC_NEMESYSBASEINFO_H
#define LLVM_LIB_TARGET_NEMESYS_MCTARGETDESC_NEMESYSBASEINFO_H

#include <map>

enum CondCode {
  EQ  = 0x0,
  NE  = 0x1,
  LT  = 0x2,
  LE  = 0x3,
  ULT = 0x4,
  ULE = 0x5,
  GE  = 0xA,
  GT  = 0xB,
  UGE = 0xC,
  UGT = 0xD
};

static const std::map<CondCode, const char*> CondCodeStrs = {
  { CondCode::EQ, "eq" },
  { CondCode::NE, "ne" },
  { CondCode::LT, "lt" },
  { CondCode::LE, "le" },
  { CondCode::ULT, "ult" },
  { CondCode::ULE, "ule" },
  { CondCode::GE,  "ge" },
  { CondCode::GT,  "gt" },
  { CondCode::UGE, "uge" },
  { CondCode::UGT, "ugt" }
};

#endif // LLVM_LIB_TARGET_NEMESYS_MCTARGETDESC_NEMESYSBASEINFO_H
