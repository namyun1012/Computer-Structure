        lw      0       6       four
        lw      0       7       one
        lw      0       2       ten
        lw      0       3       adloop
loop    add     1       7       1       reg1 + reg7 (1 + 1) = reg1   
        beq     1       2       exit
        noop
        jalr    3       5
exit    sw      7       1       0
done    halt                            end of program
hello    .fill   hello
four    .fill   4
one     .fill   1
ten     .fill   10
adloop  .fill   loop
