#include "TableGenBackends.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/Support/Debug.h"
#include "llvm/TableGen/Error.h"
#include "llvm/TableGen/Record.h"

using namespace llvm;

namespace {
class EnumEmitter {
  RecordKeeper &Records;

public:
  EnumEmitter(RecordKeeper &R) : Records(R) {}

  void run(raw_ostream &OS) const;
  void emitEnum(Record *Enum, RecordKeeper &RK, raw_ostream &OS) const;
};
} // End anonymous namespace

void EnumEmitter::run(raw_ostream &OS) const {
  auto *EnumClass = Records.getClass("Enum");
  assert(EnumClass && "Enum class definition missing");

  SmallPtrSet<Record *, 16> Enums;
  for (const auto &D : Records.getClasses()) {
    if (D.second->isSubClassOf(EnumClass)) {
      Enums.insert(D.second.get());
    }
  }

  for (auto *R : Enums) {
    emitEnum(R, Records, OS);
  }
}

static StringRef getEnumMemberStr(Record *EM) {
  return (EM->isValueUnset("String") ? EM->getName()
                                     : EM->getValueAsString("String"));
}

void EnumEmitter::emitEnum(Record *Enum, RecordKeeper &RK,
                           raw_ostream &OS) const {
  StringRef EnumName = Enum->getName();
  const std::vector<Record *> &Members = RK.getAllDerivedDefinitions(EnumName);

  using EnumValPair = std::pair<Record *, int>;
  std::vector<EnumValPair> EnumMembers;
  bool HasVal = Enum->getValueInit("Val")->isComplete();
  bool EmitPrintFn = !Enum->isValueUnset("PrintMethod");
  bool EmitMatchFn = !Enum->isValueUnset("MatchMethod");

  for (auto *M : Members) {
    if (!M->isSubClassOf("Enum")) {
      PrintFatalError(M->getLoc(), "Found enumeration member clash: '" +
                                       M->getName() + "' derives from '" +
                                       EnumName +
                                       "' but does not derive from Enum");
    }
    int Val = 0;
    if (!M->isValueUnset("Val"))
      Val = M->getValueAsInt("Val");
    else if (HasVal)
      PrintFatalError(M->getLoc(),
                      "Class has value but enumeration member doesn't!");
    EnumMembers.emplace_back(std::make_pair(M, Val));
  }

  // Sort the members by value
  if (HasVal) {
    std::sort(EnumMembers.begin(), EnumMembers.end(),
              [](const EnumValPair &A, const EnumValPair &B) {
                return A.second < B.second;
              });
  }

  bool IsClass = Enum->getValueAsBit("IsClass");

  OS << "enum" << (IsClass ? " class " : " ") << EnumName << " {\n";
  if (!HasVal) {
    for (const auto &MP : EnumMembers) {
      OS.indent(2) << MP.first->getName() << ",\n";
    }
  } else {
    for (const auto &MP : EnumMembers) {
      OS.indent(2) << MP.first->getName() << " = " << MP.second << ",\n";
    }
  }
  OS << "};\n\n";

  if (EmitPrintFn) {
    const char *Param = "E";
    OS << "inline static const char *" << Enum->getValueAsString("PrintMethod")
       << "(const " << EnumName << " " << Param << ") {\n";
    OS.indent(2) << "switch (" << Param << ") {\n";
    for (const auto &MP : EnumMembers) {
      Record *EM = MP.first;
      Twine QualName(EnumName + "::" + EM->getName());
      OS.indent(2) << "case " << QualName << ":\n";
      OS.indent(4) << "return \"" << getEnumMemberStr(EM) << "\";\n";
    }
    OS.indent(2) << "}\n";
    OS << "}\n\n";
  }

  if (EmitMatchFn) {
    SmallPtrSet<Record *, 8> SeenStrings;
    Twine RetTy("Optional<" + EnumName + ">");
    OS << "inline static " << RetTy << " "
       << Enum->getValueAsString("MatchMethod") << "(const char *Str) {\n";
    OS.indent(2) << "return StringSwitch<" << RetTy << ">(Str)\n";
    for (auto &MP : EnumMembers) {
      Record *EM = MP.first;
      if (!SeenStrings.insert(EM).second)
        PrintFatalError(EM->getLoc(), "Found duplicate string");
      Twine QualName(EnumName + "::" + EM->getName());
      OS.indent(6) << ".Case(\"" << getEnumMemberStr(EM) << "\", " << QualName
                   << ")\n";
    }
    OS.indent(6) << ".Default(None);\n";
    OS << "}\n\n";
  }
}

void llvm::EmitEnums(RecordKeeper &RK, raw_ostream &OS) {
  EnumEmitter(RK).run(OS);
}
