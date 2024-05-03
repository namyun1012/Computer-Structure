        lw      0       2       four
        lw      0       7       one
loop    add     1       2       1
        add     1       2       1
        add     1       2       1
        add     1       2       1
        lw      1       3       0       
        beq     1       5       exit
        noop
        jalr    0       2
exit    sw      0       1       1    load reg1 with 7 (symbolic address)
done    halt                            end of program
seven    .fill   7
negse    .fill   -7
eleven   .fill   11
hello    .fill   hello
four    .fill   4
one     .fill   1
