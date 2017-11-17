; RUN: llvm-mc -arch=nemesys < %s | FileCheck %s

; CHECK: add r0, #3, r1
add r0, #3, r1
; CHECK: add r0, r20, r6
add r0, r20, r6

; CHECK: sub r5, #4, r1
sub r5, #4, r1
; CHECK: sub r0, r20, r6
sub r0, r20, r6

; CHECK: mpy r1, #19, r1
mpy r1, #19, r1
; CHECK: mpy r0, r20, r6
mpy r0, r20, r6

; CHECK: and r0, #3, r1
and r0, #3, r1
; CHECK: and r0, r20, r6
and r0, r20, r6

; CHECK: or r31, #16, r1
or r31, #0x10, r1
; CHECK: or r0, r20, r6
or r0, r20, r6

; CHECK: xor r0, #3, r1
xor r0, #3, r1
; CHECK: xor r0, r20, r6
xor r0, r20, r6


; CHECK: shl r31, #16, r1
shl r31, #0x10, r1
; CHECK: shl r0, r20, r6
shl r0, r20, r6

; CHECK: sra r0, #3, r1
sra r0, #3, r1
; CHECK: sra r0, r20, r6
sra r0, r20, r6

; CHECK: srl r0, #3, r1
srl r0, #3, r1
; CHECK: srl r0, r20, r6
srl r0, r20, r6
