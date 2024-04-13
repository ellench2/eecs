        lw      0       1       n
        lw      0       2       r
        lw      0       4       Caddr
        jalr    4       7
        halt
start   sw      5       1       Stack
        lw      0       6       One
        add     6       5       5
        sw      5       2       Stack
        add     6       5       5
        sw      5       7       Stack
        beq     0       1       r1
        beq     0       2       r1
        beq     1       2       r1
        lw      0       6       neg1
        add     6       1       1
        lw      0       6       Caddr
        jalr    6       7
        add     0       3       4
        lw      0       6       neg1
        add     6       2       2
        lw      0       6       Caddr
        jalr    6       7
        add     3       4       3
back    lw      0       6       neg1
        lw      5       7       Stack
        add     6       5       5
        lw      5       2       Stack
        add     6       5       5
        lw      5       1       Stack
        jalr    7       6
r1      lw      0       3       One
        beq     0       0       back
n       .fill   2
r       .fill   1
One     .fill   1
neg1    .fill   -1
Caddr   .fill   start
Stack   .fill   0
        .fill   0
        .fill   0
        .fill   0
        .fill   0
        .fill   0
        .fill   0
        .fill   0
        .fill   0
        .fill   0
        .fill   0
        .fill   0
        .fill   0
        .fill   0
        .fill   0
        .fill   0
        .fill   0
        .fill   0
        .fill   0
