; RUN: not llvm-mc -arch=nemesys < %s 2>%t | FileCheck %s
; RUN: FileCheck --check-prefix=CHECK-ERROR %s < %t

; CHECK: cmp.lt r0, r1, p2
cmp.lt r0, r1, p2

; CHECK-ERROR: error: unknown condition code
cmp.zz r0, r1, p2

; CHECK-ERROR: error: unknown operand
cmp.eq r0, r1, p99
