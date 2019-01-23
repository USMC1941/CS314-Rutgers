#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "InstrUtils.h"
#include "Utils.h"

#define MAX_REG_NUM 1000

int main(int argc, char *argv[])
{
	FILE *infile;
	Instruction *head, *instr;
	int Memory[5];
	int RegisterFile[MAX_REG_NUM];
	int input;
	int instrCounter = 0;	/* counts number of executed instructions */

	if (argc != 2) {
		fprintf(stderr, "Use of command:\n  run <RISC code file>\n");
		exit(EXIT_FAILURE);
	}
	memset(Memory, '\0', sizeof(Memory));
	infile = fopen(argv[1], "r");
	if (!infile) {
		ERROR("Cannot open input file \"%s\"\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	head = ReadInstructionList(infile);
	fclose(infile);
	instr = head;
	while (instr) {
		switch (instr->opcode) {
		case LOAD:
			/* based on ASCII representation */
			/* of variable names */
			RegisterFile[instr->field1] =
			    Memory[instr->field2 - 'a'];
			break;
		case LOADI:
			RegisterFile[instr->field1] = instr->field2;
			break;
		case STORE:
			Memory[instr->field1 - 'a'] =
			    RegisterFile[instr->field2];
			break;
		case ADD:
			RegisterFile[instr->field1] =
			    RegisterFile[instr->field2] +
			    RegisterFile[instr->field3];
			break;
		case SUB:
			RegisterFile[instr->field1] =
			    RegisterFile[instr->field2] -
			    RegisterFile[instr->field3];
			break;
		case MUL:
			RegisterFile[instr->field1] =
			    RegisterFile[instr->field2] *
			    RegisterFile[instr->field3];
			break;
		case READ:
			printf("tinyL>> enter value for \"%c\": ",
			       instr->field1);
			scanf("%d", &input);
			Memory[instr->field1 - 'a'] = input;
			break;
		case WRITE:
			printf("tinyL>> %c = %d\n", instr->field1,
			       Memory[instr->field1 - 'a']);
			break;
		default:
			ERROR("Illegal instructions\n");
		}
		instrCounter++;
		instr = instr->next;
	}
	DestroyInstructionList(head);
	printf("Executed %d instructions\n", instrCounter);
	return EXIT_SUCCESS;
}
