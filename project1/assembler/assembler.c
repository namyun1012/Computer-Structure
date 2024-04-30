/* Assembler code fragment for LC-2K */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

struct label {
	int addr;
	char name[7]; // maximum is 6
	char stored[MAXLINELENGTH]; // if fill 
};


struct label ** label_list;
int list_size = 0;

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);


struct label_list * label_list;

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

	while(!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		
		// Not label
		if(!strcmp(label, "")) continue;

		// label size > 6
		if(strlen(label) > 6) {
			printf("Size over 6\n");
			exit(0);
		}
		
		if(isNumber(label[0])) {
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

		// put in label list
		label_list[list_size] = new_label;
		list_size++;

		// .fill 일시 stored 사용
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

			strcpy(new_label->stored, arg0);
		}

		// increase cur_line
		cur_line++;
	}

	// End of Labeling
	/* this is how to rewind the file ptr so that you start reading from the
		 beginning of the file */
	// 파일 시작 부터 읽도록 초기화
	rewind(inFilePtr);

	/* TODO: Phase-2 generate machine codes to outfile */

	/* after doing a readAndParse, you may want to do the following to test the
		 opcode */
	
	// opcode 확인하기
	while(!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		int result = 0;

		// R type
		// 000000 + opcode + regA + regB + unused + destReg 
		if(!strcmp(opcode, "add") || !strcmp(opcode, "nor")) {

		}

		// I-type
		else if(!strcmp(opcode, "lw") || !strcmp(opcode, "sw") || !strcmp(opcode, "beq")) {

		}

		// J - type
		else if(!strcmp(opcode, "jalr")) {

		}

		// O - type
		else if(!strcmp(opcode, "halt") || !strcmp(opcode, "noop")) {

		}

		// Not opcode
		else {
			printf("It is not offical opcode\n");
			exit(0);
		}
	}

	if (inFilePtr) {
		fclose(inFilePtr);
	}
	if (outFilePtr) {
		fclose(outFilePtr);
	}
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