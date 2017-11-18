; RUN: llvm-mc -arch=nemesys < %s | FileCheck %s

; CHECK: br p0, #foo
br p0, #foo

; CHECK: br p0, #24
br p0, #24
