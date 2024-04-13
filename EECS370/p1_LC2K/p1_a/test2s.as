        lw      0       1       filler
        add     0       1       2
        sw      0       2       0
        noop
        add     2       1       2
        beq     2       3       0
        lw      0       5       regA
        jalr    5       3
        halt
filler  .fill   5
regA    .fill   8
