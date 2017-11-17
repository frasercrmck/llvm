; RUN: not llvm-mc -arch=nemesys < %s 2>%t | FileCheck %s
; RUN: FileCheck --check-prefix=CHECK-ERROR %s < %t

; CHECK: add r0, #3, r1
add r0, #3, r1

; CHECK: add r0, #1, r1
add r0, #0x1, r1

; TODO: parse negative immediates
; add r0, #-12, r1

; CHECK-ERROR: note: operand must be 6-bit signed immediate
; CHECK-ERROR-NEXT: add r0, #35, r1
; CHECK-ERROR: note: operand must be a register in range [r0, r31]
; CHECK-ERROR-NEXT: add r0, #35, r1
add r0, #35, r1
