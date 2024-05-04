        lw      0       1       seven    load reg1 with 7 (symbolic address)
        lw      0       2       negse       load reg2 with -7 (numeric address)
add1    add     1       2       1         
        nor     0       1       1       not reg1
        beq     1       0       done   error
        sw      0       1       0    store reg1 with memory1 (symbolic address)
        sw      0       2       3
        lw      0       4       eleven
        sw      1       4       0       
done    halt                            end of program
seven    .fill   7
negse    .fill   -7
eleven    .fill   11
