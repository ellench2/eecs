lw      0       1       n
lw      0       2       r
lw      0       4       Caddr
jalr    4       7
halt
start   sw      r5      r1      Stack
lw      r0      r6      One
add     r6      r5      r5
sw      r5      r2      Stack
add     r6      r5      r5
sw      r5      r7      Stack
beq     0       r1      r1
beq     0       r0      r1
beq     r1      r2      r1
lw      r0      r6      neg1
add     r6      r1      r1
lw      r0      r6      start
jalr    r6      r7
add     r0      r3      r4
lw      r0      r6      neg1
add     r6      r2      r2
lw      r0      r6      start
jalr    r6      r7
add     r3      r4      r3
back    lw      r0      r6      neg1
lw      r5      r7      Stack
add     r6      r5      r5
lw      r5      r2      Stack
add     r6      r5      r5
lw      r5      r1      Stack
jalr    r7      r6
r1      lw      r0      r3      one
beq     r0      r0      back
n       .fill   7
r       .fill   3
Caddr   .fill   start
Stack   .fill   0
Stack   .fill   0
Stack   .fill   0
Stack   .fill   0
Stack   .fill   0
Stack   .fill   0
Stack   .fill   0
Stack   .fill   0
Stack   .fill   0
Stack   .fill   0
Stack   .fill   0
Stack   .fill   0
Stack   .fill   0
Stack   .fill   0
Stack   .fill   0
Stack   .fill   0
