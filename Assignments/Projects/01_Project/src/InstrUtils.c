#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Instr.h"
#include "InstrUtils.h"
#include "Utils.h"

void PrintInstruction(FILE * outfile, Instruction * instr)
{
	if (!outfile) {
		ERROR("File error\n");
		exit(EXIT_FAILURE);
	}
	if (instr) {
		switch (instr->opcode) {
		case LOAD:
			fprintf(outfile, "LOAD r%d %c\n", instr->field1,
				instr->field2);
			break;
		case LOADI:
			fprintf(outfile, "LOADI r%d #%d\n", instr->field1,
				instr->field2);
			break;
		case STORE:
			fprintf(outfile, "STORE %c r%d\n", instr->field1,
				instr->field2);
			break;
		case ADD:
			fprintf(outfile, "ADD r%d r%d r%d\n", instr->field1,
				instr->field2, instr->field3);
			break;
		case SUB:
			fprintf(outfile, "SUB r%d r%d r%d\n", instr->field1,
				instr->field2, instr->field3);
			break;
		case MUL:
			fprintf(outfile, "MUL r%d r%d r%d\n", instr->field1,
				instr->field2, instr->field3);
			break;
		case READ:
			fprintf(outfile, "READ %c\n", instr->field1);
			break;
		case WRITE:
			fprintf(outfile, "WRITE %c\n", instr->field1);
			break;
		default:
			ERROR("Illegal instructions\n");
		}
	}
}

void PrintInstructionList(FILE * outfile, Instruction * instr)
{
	/* YOUR CODE GOES HERE */
	/* TO STUDENTS - BEGIN */
	if (!outfile) {
		ERROR("File error\n");
		exit(EXIT_FAILURE);
	}
	if (!instr) {
		ERROR("No instructions\n");
		exit(EXIT_FAILURE);
	}
	while (instr) {
		PrintInstruction(outfile, instr);
		instr = instr->next;
	}
	/* TO STUDENTS - END */
}

Instruction *ReadInstruction(FILE * infile)
{
	static char InstrBuffer[100];
	Instruction *instr = NULL;
	char dummy;

	if (!infile) {
		ERROR("File error\n");
		exit(EXIT_FAILURE);
	}
	instr = (Instruction *) calloc(1, sizeof(Instruction));
	if (!instr) {
		ERROR("Calloc failed\n");
		exit(EXIT_FAILURE);
	}
	instr->prev = NULL;
	instr->next = NULL;
	fscanf(infile, "%99s", InstrBuffer);
	if (strnlen(InstrBuffer, sizeof(InstrBuffer)) == 0) {
		free(instr);
		return NULL;
	}
	if (!strcmp(InstrBuffer, "LOAD")) {
		instr->opcode = LOAD;
		/* get first operand: register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field1));
		/* get second operand: variable */
		fscanf(infile, "%s", InstrBuffer);
		instr->field2 = *InstrBuffer;
	} else if (!strcmp(InstrBuffer, "LOADI")) {
		instr->opcode = LOADI;
		/* get first operand: register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field1));
		/* get second operand: immediate */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field2));
	} else if (!strcmp(InstrBuffer, "STORE")) {
		instr->opcode = STORE;
		/* get first operand: variable */
		fscanf(infile, "%s", InstrBuffer);
		instr->field1 = *InstrBuffer;
		/* get second operand: register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field2));
	} else if (!strcmp(InstrBuffer, "ADD")) {
		instr->opcode = ADD;
		/* get first operand: target register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field1));
		/* get second operand: register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field2));
		/* get third operand: register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field3));
	} else if (!strcmp(InstrBuffer, "SUB")) {
		instr->opcode = SUB;
		/* get first operand: target register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field1));
		/* get second operand: register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field2));
		/* get third operand: register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field3));
	} else if (!strcmp(InstrBuffer, "MUL")) {
		instr->opcode = MUL;
		/* get first operand: target register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field1));
		/* get second operand: register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field2));
		/* get third operand: register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field3));
	} else if (!strcmp(InstrBuffer, "READ")) {
		instr->opcode = READ;
		/* get operand: variable */
		fscanf(infile, "%s", InstrBuffer);
		instr->field1 = *InstrBuffer;
	} else if (!strcmp(InstrBuffer, "WRITE")) {
		instr->opcode = WRITE;
		/* get operand: variable */
		fscanf(infile, "%s", InstrBuffer);
		instr->field1 = *InstrBuffer;
	} else {
		free(instr);
		return NULL;
	}
	return instr;
}

Instruction *ReadInstructionList(FILE * infile)
{
	Instruction *instr, *head, *tail;

	if (!infile) {
		ERROR("File error\n");
		exit(EXIT_FAILURE);
	}
	head = tail = NULL;
	while ((instr = ReadInstruction(infile))) {
		if (!head) {
			head = tail = instr;
			continue;
		}
		instr->prev = tail;
		instr->next = NULL;
		tail->next = instr;
		tail = instr;
	}
	return head;
}

Instruction *LastInstruction(Instruction * instr)
{
	if (!instr) {
		ERROR("No instructions\n");
		exit(EXIT_FAILURE);
	}
	while (instr->next)
		instr = instr->next;
	return instr;
}

void DestroyInstructionList(Instruction * instr)
{
	Instruction *i;

	if (!instr)
		return;
	while (instr) {
		i = instr;
		instr = instr->next;
		free(i);
	}
}
