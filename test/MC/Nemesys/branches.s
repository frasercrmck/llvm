; RUN: llvm-mc -arch=nemesys < %s | FileCheck %s

; CHECK: br p7, #3456
br p7, #3456

; CHECK: br !p3, #loop
br !p3, #loop
