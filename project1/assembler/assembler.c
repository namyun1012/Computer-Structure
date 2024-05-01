/* Assembler code fragment for LC-2K */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

struct label {
	int addr;
	char name[7]; // maximum is 6
	char fill[MAXLINELENGTH]; // if fill 
	int filled;
};


struct label ** label_list;
int list_size = 0;

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int registerCheck(char *reg);
struct label * findLabel(char * find);

int main(int argc, char *argv[]) 
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], 
			 arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

	// added code
	label_list = (struct label **) malloc(sizeof(struct label *) * MAXLINELENGTH);


	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
				argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	/* here is an example for how to use readAndParse to read a line from
		 inFilePtr */
	
	// if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		/* reached end of file */
		
	//}
	
	// make labeling
	/* TODO: Phase-1 label calculation */
	// Error checking : duplicated labeling, 
	
	int cur_line = 0;		
	int i = 0;

	while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		
		// Not label
		if(!strcmp(label, "")){ 
			// 
			cur_line++;
			continue;
		}
		// label size > 6
		if(strlen(label) > 6) {
			printf("Size over 6\n");
			exit(0);
		}
		
		char * temp = &label[0];
		if(isNumber(temp)) {
			printf("first letter is number");
			exit(0);
		}
		
		// duplicated label;
		for(i = 0; i < list_size; i++) {
			if(!strcmp(label, label_list[i]->name)) {
				printf("Duplicated Label detected\n");
				exit(0);
			}
		}

		// add label
		// make label

		struct label * new_label = (struct label *) malloc(sizeof(struct label));
		strcpy(new_label->name, label);
		new_label->addr = cur_line; // line is address 
		new_label->filled = 0;

		// put in label list
		label_list[list_size] = new_label;
		list_size++;
		
		// .fill 일시 fill 사용
		if(!strcmp(opcode, ".fill")) {
			
			// 숫자인 경우 그대로 집어넣기
			// 숫자 검사 isNumber String 받는다.
			if(isNumber(arg0)) {
				
				// int 로는 표현 못함
				long long test = atoll(arg0);
				if(test < -2147483648 || test > 2147483647) {
					printf("fill number value is overflowed!");
					exit(0);
				}
				
			}
			
			strcpy(new_label->fill, arg0);
			new_label->filled = 1;
		}

		// increase cur_line
		cur_line++;
	}
	
	
	// End of Labeling
	/* this is how to rewind the file ptr so that you start reading from the
		 beginning of the file */
	// 파일 시작 부터 읽도록 초기화
	rewind(inFilePtr);
	// label check

	for(int i = 0; i < list_size; i++)
		printf("label : %s addr : %d, fill : %s \n", label_list[i]->name, label_list[i]->addr, label_list[i]->fill);

	/* TODO: Phase-2 generate machine codes to outfile */

	/* after doing a readAndParse, you may want to do the following to test the
		 opcode */
	
	// opcode 확인하기
	// I type check 확인 위해서
	cur_line = 0;
	while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		int result = 0;

		// R type
		// 000000 + opcode + regA + regB + unused + destReg 
		if(!strcmp(opcode, "add") || !strcmp(opcode, "nor")) {
			
			if(!strcmp(opcode, "add")) {
				result |= 0 << 24;
				result |= 0 << 23;
				result |= 0 << 22;
			}
			// nor
			else {
				result |= 0 << 24;
				result |= 0 << 23;
				result |= 1 << 22;
			}
			// register check
			// arg0 
			if(!registerCheck(arg0)) {
				printf("arg0 is unvalid\n");
				exit(0);
			}
			// arg1
			if(!registerCheck(arg1)) {
				printf("arg1 is unvalid\n");
				exit(0);
			}

			// arg2
			if(!registerCheck(arg2)) {
				printf("arg2 is unvalid\n");
				exit(0);
			}
			// arg0 = reg A arg1 = regB arg2 = destReg
			result |= (atoi(arg0)) << 19;
			result |= (atoi(arg1)) << 16;
			result |= atoi(arg2);

		}

		// I-type
		
		else if(!strcmp(opcode, "lw") || !strcmp(opcode, "sw") || !strcmp(opcode, "beq")) {
			
			// opcode
			if(!strcmp(opcode, "lw")) {
				result |= 0 << 24;
				result |= 1 << 23;
				result |= 0 << 22;
			}
			// nor
			else if(!strcmp(opcode, "sw")) {
				result |= 0 << 24;
				result |= 1 << 23;
				result |= 1 << 22;
			}
			
			else {
				result |= 1 << 24;
				result |= 0 << 23;
				result |= 0 << 22;
			}

			if(!registerCheck(arg0)) {
				printf("arg0 is unvalid\n");
				exit(0);
			}
			// arg1
			if(!registerCheck(arg1)) {
				printf("arg0 is unvalid\n");
				exit(0);
			}
			
			result |= (atoi(arg0)) << 19;
			result |= (atoi(arg1)) << 16;

			// offset field
			// string 일 경우 label 주소값 사용
			int offset = 0;
			
			// register check
			// first arg2 is Number
			if(isNumber(arg2)) {
				offset = atoi(arg2);

				// offset Test
				if( offset > 32767 || offset < -32768 ) {
					printf("offset is overed!\n");
					exit(0);
				}
			}

			// label 일 때 어떻게 할지 결정
			else  {
				struct label * finded_label = findLabel(arg2);
				
				if(!finded_label) {
					printf("not existed label!\n");
					exit(0);
				}


				// beq 면 상대값
				// second beq and not arg2 is label
				if(!strcmp(opcode, "beq")) {
					int temp = finded_label->addr - cur_line - 1;
					
					// temp로 검사
					if(temp > 32767 | temp < -32768) {
						printf("offset is overed!\n");
						exit(0);
					}
					
					// 
					temp &= 0xffff;
					
					offset |= temp;
				}

				// 아닐시 addr 그대로
				// third, lw st and arg2 is label
				else {
					offset |= finded_label->addr;

					// offset Test
					if( offset > 32767 || offset < -32768 ) {
						printf("offset is overed!\n");
						exit(0);
					}
				}
			}

			// 결과
			result |= offset;
		}

		// J - type
		else if(!strcmp(opcode, "jalr")) {
			result |= 1 << 24;
			result |= 0 << 23;
			result |= 1 << 22;

			if(!registerCheck(arg0)) {
				printf("arg0 is unvalid\n");
				exit(0);
			}
			// arg1
			if(!registerCheck(arg1)) {
				printf("arg0 is unvalid\n");
				exit(0);
			}
			
			result |= (atoi(arg0)) << 19;
			result |= (atoi(arg1)) << 16;

		}

		// O - type
		else if(!strcmp(opcode, "halt") || !strcmp(opcode, "noop")) {
			
			if(!strcmp(opcode, "halt")) {
				result |= 1 << 24;
				result |= 1 << 23;
				result |= 0 << 22;
			}


			// not thing
			else if(!strcmp(opcode, "noop")) {
				result |= 1 << 24;
				result |= 1 << 23;
				result |= 1 << 22;
			}
		}

		// Not opcode
		// Maybe a label

		else if(!strcmp(opcode, ".fill")){
			
			if(isNumber(arg0)) {
				result |= atoi(arg0);
			}
			// 주소 값
			else {
				struct label * temp = findLabel(arg0);
				if(!temp) {
					printf("not existed label\n");
					exit(0);
				}

				result |= temp->addr;
			}
		
		}

		else {
			printf("It is not offical opcode\n");
			exit(0);
		}
		
		cur_line++;
		printf("%d\n", result);
		fprintf(outFilePtr, "%d\n", result);
	}

	if (inFilePtr) {
		fclose(inFilePtr);
	}
	if (outFilePtr) {
		fclose(outFilePtr);
	}

	// label 할당 해제할 것
	return(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
		char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* reached end of file */
		return(0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL) {
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label)) {
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%"
			"[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
	return(1);
}

int isNumber(char *string)
{
	/* return 1 if string is a number */
	int i;
	return( (sscanf(string, "%d", &i)) == 1);
}

int registerCheck(char *reg) {
	
	if(!isNumber(reg)) return 0;
	
	int temp = atoi(reg);

	
	if(temp < 0 || temp > 7) return 0;


	return 1;
}

struct label * findLabel(char * find) {
	int i = 0;
	for(i = 0; i < list_size; i++) {
		if(!strcmp(find, label_list[i]->name))
			return label_list[i];
	}

	// find failed
	return 0;
}