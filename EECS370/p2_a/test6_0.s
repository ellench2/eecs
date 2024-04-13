        lw      0       1       One
One     add     0       1       2
        sw      2       3       Three
        lw      0       1       One
        lw      0       1       loc
        sw      0       2       Four
loc     add     0       1       2
Three   lw      0       1       One
        sw      0       1       Three
        lw      6       7       Four
        .fill   Two
Four    .fill   Stack
Two     .fill   1
Five    .fill   Three
        .fill   Two
        .fill   Five
        .fill   One
