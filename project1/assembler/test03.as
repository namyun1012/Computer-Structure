        lw      0       1       three    load reg1 with 7 (symbolic address)
        lw      0       6       adloop
        lw      0       5       minus
loop    beq     0       1       L1
        add     1       2       2
        add     1       5       1         
        jalr    6       7
L1      sw      2       4       0
        add     2       1       2
        sw      2       5       3
done    halt                            end of program
seven    .fill   7
negse    .fill   -7
eleven    .fill   11
three   .fill   3
minus   .fill   -1
adloop  .fill   loop
