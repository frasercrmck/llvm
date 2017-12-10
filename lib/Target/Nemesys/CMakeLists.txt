set(LLVM_TARGET_DEFINITIONS Nemesys.td)

tablegen(LLVM NemesysGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM NemesysGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM NemesysGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM NemesysGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM NemesysGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM NemesysGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM NemesysGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM NemesysGenEnums.inc -gen-enums)

add_public_tablegen_target(NemesysCommonTableGen)

add_llvm_target(NemesysCodeGen
  NemesysRegisterInfo.cpp
  NemesysTargetMachine.cpp
)

add_subdirectory(AsmParser)
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
add_subdirectory(InstPrinter)
add_subdirectory(Disassembler)
