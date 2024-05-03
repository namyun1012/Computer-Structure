        lw      0       1       seven    load reg1 with 7 (symbolic address)
        lw      0       2       negse       load reg2 with -7 (numeric address)
add1    add     1       2       1       reg 1 will be 0
loop    add     2       3       5   
        beq     0       1       2       goto end of program when reg1==0
        beq     0       0       loop   go back to the beginning of the loop
        noop
        jalr    3       1
        sw      0       1       100    store reg1 with 7 (symbolic address)
        sw      2       3       -52
        sw      5       6       13
done    halt                            end of program
seven    .fill   7
negse    .fill   -7
eleven    .fill   11
hello    .fill   hello
