        lw      0       1       data1
        add     1       1       2
        nor     2       3       0
        beq     0       0       done
        lw      0       1       data1
done    halt
data1	.fill	12345
