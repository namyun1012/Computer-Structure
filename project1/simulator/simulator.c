/* LC-2K Instruction-level simulator */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000 
typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int num);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read in the entire machine-code file into memory */
    // 하나씩 Machine code를 읽어나감
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
            state.numMemory++) {

        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);

    }
    
		/* TODO: */
    
    state.pc = 0;
    // halt 나올시 종료함
    int code, opcode, arg0, arg1, arg2;
    int i = 0;
    for(i = 0; i <= 7; i++)
        state.reg[i] = 0;

    i = 0;
    while(1) {
        // i++;
        i++;
        code = state.mem[state.pc];
        state.pc += 1;

        // code parsing 
        opcode = (code >> 22) & 0b111; // opcode
        arg0 = (code >> 19) &0b111; // regA
        arg1 = (code >> 16) & 0b111; // regB 
        arg2 = code & 0xffff; // 16bit

        // printf("current code : opcode : %d arg0 : %d arg1 : %d arg2 : %d\n", opcode, arg0, arg1, arg2);

        // r-type
        if(opcode >= 0 && opcode <= 1) {
            // arg2 는 끝의 3 bit만 사용한다.
            arg2 &= 0b111;

            // add
            if(opcode == 0) {
                state.reg[arg2] = state.reg[arg0] + state.reg[arg1];
            }

            // nor
            else {
                state.reg[arg2] = !(state.reg[arg0] || state.reg[arg1]);
            }
        }

        // I-type
        else if(opcode >= 2 && opcode <= 4) {
           

            int offset = convertNum(arg2);
            int addr;
             // lw
            if(opcode == 2) {
                addr = state.reg[arg0] + offset;
                state.reg[arg1] = state.mem[addr];
            }


            // sw
            else if(opcode == 3) {
                addr = state.reg[arg0] + offset;
                state.mem[addr] = state.reg[arg1];
            }

            // beq
            // address를 따로 처리해야 한다.
            else {
                // 같을 경우에 branch
                short temp = 0;
                temp |= arg2;
                int temp32 = (int) temp;
                
                if(state.reg[arg0] == state.reg[arg1]) {
                    state.pc += temp32;
                    // printf("pc : %d\n", state.pc);
                }
            }


        }

        // J-type
        else if(opcode == 5) {
            state.reg[arg1] = state.pc;

            //regA 와 regB의 값이 다를 경우에만 점프함
            if(state.reg[arg0] != state.reg[arg1]) {
                state.pc =state.reg[arg0];
            }
        }

        // O-type
        else if(opcode >= 6 && opcode <= 7) {
            
            // halt시 while문 종료함
            if(opcode == 6) {
                break;
            }

            // do nothing
            else {

            }

        }

        else {
            printf("not opcode\n");
            exit(1);
        }

        printState(&state);
    }

    printf("machine halted\n");
    printf("total of %d instructions executed\n", i);
    printf("final state of machine:\n");
    


    printState(&state);
    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
	/* convert a 16-bit number into a 32-bit Linux integer */
	if (num & (1 << 15)) {
		num -= (1 << 16);
	}
	return (num);
}

// 어처피 3 bit 만 받아와서 0 이상 7 이하만 표현 가능하다.
int registerCheck(int reg) {	
	if(reg < 0 || reg > 7) return 0;

	return 1;
}