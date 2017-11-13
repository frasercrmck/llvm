add_llvm_library(LLVMNemesysAsmParser
  NemesysAsmParser.cpp
)

add_dependencies( LLVMNemesysAsmParser NemesysCommonTableGen )
