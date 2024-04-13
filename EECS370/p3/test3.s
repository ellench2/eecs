        lw      0       1       data
        lw      1       1       2
        add     1       2       3
        add     3       3       2
        sw      0       3       0
        lw      0       2       0
        add     1       2       5
        beq     0       1       done
        nor     5       5       5
        beq     5       5       done
done    halt
data    .fill   1
        
