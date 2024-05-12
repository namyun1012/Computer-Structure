        lw      0       4       four    // reg4 = 4 swap function
        lw      0       5       one       // reg5 = 1
        lw      0       6       two    // reg6 = 2
        sw      0       5       0       // memory[0] = 1 
        sw      0       6       1       // memory[1] = 2
        lw      0       1       zero       // reg1 = 0 // addr
        add     1       5       2       // reg2 = 1
        lw      1       3       0       // reg3 = memory[reg1]
        lw      2       7       0       // reg7 = memory[reg2]
        sw      1       7       0
        sw      2       3       0
        jalr    1       1               // not jump
        halt                            end of program
four    .fill   4
five    .fill   5
two     .fill   2
one     .fill   1
zero    .fill   0
