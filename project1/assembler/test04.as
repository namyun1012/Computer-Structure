        lw      0       6       min4    // reg6 = -4
        add     6       7       7       // reg7 -= 4
        lw      0       1       five    // reg1 = 5
        lw      0       6       one     // reg6 = 1
        lw      0       5       loopad
        sw      0       1       0       // memory[0] = reg1
        add     0       0       2       // reg2 = 0
        add     0       0       4       // reg4 = 0
loop    beq     1       2       done    // if reg1 == reg2 goto done
        add     4       2       4
        add     1       4       4
        add     2       6       2
        jalr    5       7
done    sw      1       4       0    
        halt                            end of program
min4    .fill   -4
five    .fill   5
one     .fill   1
loopad  .fill   loop
