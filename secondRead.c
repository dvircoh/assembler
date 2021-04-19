#include "assembler.h"
extern unsigned int instructionArr[1025];
extern label *labelTable;
extern int countLabel;


/*Second transition on the file. Handles labels and encoding of operands*/
bool secondRead(FILE *file) {
	unsigned int *IC = &instructionArr[1024];
	char str[82];
	char *token1;
	char *line;
	char delim[3] = { " 	"};
	int countLine = 0;
	bool errorFlug = TRUE;

	*IC = 0;
	rewind(file);
	while (fgets(str, MAX_LINE + 2, file)) {
		countLine++;
		line = str;
		token1 = strtok(line, delim);
		if (token1[strlen(token1) - 1] == ':')
			token1 = strtok(NULL, delim);
		line = removeWhiteSpace(line);
		if (!*line)
			continue;
		if (*line == ';')
			continue;
		if(token1[strlen(token1)-1]=='\n')
			token1[strlen(token1)-1]='\0';
		if (!strcmp(token1, ".entry")) {
			char *label = strtok(NULL," 	\n");
			if (!entryInsert(label, countLine)){
		
				errorFlug = FALSE;
				continue;
			}
			continue;
		}
		if (!strcmp(token1, ".extern")) 
			continue;
		if (!strcmp(token1, ".data")) 
			continue;
		if (!strcmp(token1, ".string")) 
			continue;
		if (!operandsParse(IC, countLine))
				errorFlug = FALSE;
	}
		return errorFlug;
}

bool operandsParse(unsigned int *IC, int countLine) {
	char *op1= strtok(NULL, ",\n");
	char *op2= strtok(NULL, " 	\n");
	(*IC)++;
	if (!op1)
		return TRUE;
	if (!op2)
		return oneOperandsParse(op1,IC,countLine);
	else return twoOperandsParse(op1,op2,IC,countLine);
}

/*parse the operandsof commends with two operands. return false if the the operands are wrong*/
bool twoOperandsParse(char *op1,char *op2,unsigned int *IC, int countLine) {
	op1 = removeWhiteSpace(op1);
	op2 = removeWhiteSpace(op2);
	if (*op1 == '@' && *op2 == '@') {
		twoRegistersEncode(op1, op2, IC);
		return TRUE;
	}
	if (!operandEncode(op1, IC, 1, countLine))
		return FALSE;
	if (!operandEncode(op2, IC, 2, countLine))
		return FALSE;
	return TRUE;

}
/*parse the operandsof commends with one operands. return false if the the operands are wrong*/
bool oneOperandsParse(char *op1,unsigned int *IC, int countLine) {
	op1 = removeWhiteSpace(op1);
	if (!operandEncode(op1, IC, 2, countLine))
		return FALSE;
	return TRUE;

}
/*Encodes the operands and inserts them into the instruction arrey*/
bool operandEncode(char *operand, unsigned int *IC, int operandNumber,
		int countLine) {
	if (isdigit(*operand)||*operand=='-'||*operand=='+') {
		int num = atoi(operand);
		instructionArr[*IC] = num<<2;
		(*IC)++;	
		return TRUE;
	}

	if (*operand == '@') {
		int reg = atoi(operand+2);
		if (operandNumber == 1)
			instructionArr[*IC] = reg << 7;
		if (operandNumber == 2)
			instructionArr[*IC] = reg << 2;
		(*IC)++;
		return TRUE;
	}
	if (isalpha(*operand)) {
		int address;
		operand =removeWhiteSpaceEnd(operand);
		address = addressLabel(operand, countLine,*IC);
		if (address) {
			instructionArr[*IC] = address;
			(*IC)++;
			return TRUE;
		} else
			return FALSE;
	}
	return TRUE;
}
/*return address of label. if the label is not exist return 0*/
int addressLabel(char *label, int countLine,int IC) {

	unsigned int result;
	int i;
	for (i = 0; i < countLabel; i++)
		if (!strcmp(labelTable[i].label, label)) 
		{
			result = labelTable[i].address << 2;
			if (labelTable[i].ext){
				result = result | EXTERNAL_ENCODE;
				insertToTable(labelTable[i].label,IC, FALSE,TRUE);
			}
			else
				result = result | RELOCATABLE_ENCODE;
			return result;

		}
	printError(countLine, "label have not in this file");
	return 0;

}
/*Declares label as "entry". return false if the label is not exist.*/
bool entryInsert(char *label, int countLine) {
	int i;
	if(strtok(NULL, " 	\n")){
		printError(countLine, "too many parameters");
		return FALSE;
	}
	for (i = 0; i < countLabel; i++)
		if (!strcmp(labelTable[i].label, label)) {
			labelTable[i].ent = TRUE;
			return TRUE;
		}
	printError(countLine, "label have not in this file");
	return FALSE;
}

/*Encodes two registers operands into a line of information*/
void twoRegistersEncode(char *reg1, char *reg2, unsigned int *IC) {
	int num;
	num = atoi(reg1+2);
	instructionArr[*IC] = num << 7;
	num = atoi(reg2+2);
	instructionArr[*IC] += num << 2;
	(*IC)++;
}

