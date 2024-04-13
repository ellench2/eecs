top     lw      2       3       number //get bit extractor into r3
        lw      0       4       mcand //get
        nor     3       4       3 // extracted a-th bit of mcand
        beq     0       3       shift // if extracted is zero, don't bother adding to sum
        beq     0       0       back // if else, go to shifter
        lw      0       5       one
back    add     5       2       2
        lw      0       6       fift
        beq     6       2       quit
        beq     0       0       top
shift   add     0       2       7 // make copy of shift# in r7
        lw      0       4       mplier // get the thing we're left shifting, in r4
        beq     0       7       skip
iter    add     4       4       4 // add r4 to itself (times 2)
        lw      0       5       neg1
        add     5       7       7 // decrement copy of shift#
        beq     0       7       skip // if done multiplying by 2, leave loop
        beq     0       0       iter // else loop again
skip    add     1       4       1 // add to running sum
        lw      0       5       one
        beq     0       0       back
quit    halt
number  .fill   -2
        .fill   -3
        .fill   -5
        .fill   -9
        .fill   -17
        .fill   -33
        .fill   -65
        .fill   -129
        .fill   -257
        .fill   -513
        .fill   -1025
        .fill   -2049
        .fill   -4097
        .fill   -8193
        .fill   -16385
fift    .fill   15
neg1    .fill   -1
mcand   .fill   1429
mplier  .fill   6203
one     .fill   1
