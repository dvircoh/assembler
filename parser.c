#include "assembler.h"

extern char* fileName;
unsigned int instructionArr[1025]; /*Array for the instruction*/
unsigned int dataArr[1025]; /*Array for the data*/
extern label *labelTable; /*Array for the labels informations*/
extern int allocLabel, countLabel;
const char cmdArr[NUM_OF_CMD][LENGTH_OF_CMD] = { "mov", "cmp", "add", "sub",
		"not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr",
		"rts", "stop" };

/*print the error with line number and file name*/
void printError(int lineCounter, char *str) {
	printf("\n file %s error in line %d %s\n", fileName, lineCounter, str);
}
/*parse the file line after line, insert labels to array of labels and create row of commands*/
bool parserFile(FILE *file) {
	int i;
	unsigned int *IC = &instructionArr[1024], *DC = &dataArr[1024];
	int errorsCounter = 0;
	char delim[3] = { " 	"};
	char str[MAX_LINE + 2];
	char *line;
	char *token1;
	char labelName[MAX_LABEL + 1];
	char temp;
	bool haveLabel;
	int countLine = 0;


	if (!labelTable) {
		printf("\nhave not memory for this program");
		exit(0);
	}
	/*for empty file return 0 and go to next file*/
	if (feof(file))
		return 0;
	while (fgets(str, MAX_LINE + 2, file)) {

		line = str;
		haveLabel = FALSE;
		countLine++;
		if (strlen(line)>MAX_LINE+1)
		{
			printError(countLine, "the line is longer");
			errorsCounter++;
			/*loop that passing on the file to new line or end of the file*/
			do {
				temp = fgetc(file);
			} while (temp != '\n' && temp != EOF);
			continue;
		}
		if (*line == ';')
			continue;
		token1 = strtok(line, delim);
		if (token1[strlen(token1) - 1] == ':') {
			strcpy(labelName, token1);
			labelName[strlen(labelName) - 1] = 0;/*trim the ':' from the end of the label*/
			for (i = 0; i < countLabel; i++) {
				if (!strcmp(labelName, labelTable[i].label)) {
					printError(countLine, "the label is declared twice");
					errorsCounter++;
					continue;
				}
			}
			token1 = strtok(NULL, delim);
			if (token1 == NULL) {
				printError(countLine, "label empty");
				errorsCounter++;
				continue;
			}
			if (errorLabel(labelName, countLine, labelTable, countLabel))
				haveLabel = TRUE;
			else {
				errorsCounter++;
				continue;

			}
		}
		line = removeWhiteSpace(line);
		if (*line==0)
			continue;
		if(token1[strlen(token1)-1]=='\n')
			token1[strlen(token1)-1]='\0';

		if (!strcmp(token1, ".data")) {
			if (haveLabel)
				insertToTable(labelName, *DC, FALSE, FALSE);
			if(!insertData(line, DC, countLine))
				errorsCounter++;
			continue;
		}
		if (!strcmp(token1, ".string")) {
			if (haveLabel)
				insertToTable(labelName, *DC, FALSE, FALSE);
			if(!insertString(line, DC, countLine))
				errorsCounter++;
			continue;
		}
		if (!strcmp(token1, ".entry")) {
			continue;
		}
		if (!strcmp(token1, ".extern")) {
			char *label = strtok(NULL, " 	\n");
			if (!externInsert(label, countLine))
				errorsCounter++;
			continue;
		}
		for (i = 0; i < 16; i++)
			if (!strcmp(token1, cmdArr[i])) {
 				if (haveLabel) 
					insertToTable(labelName, *IC, TRUE, FALSE);
				if (!cmdParse(IC, i, countLine)) {
					errorsCounter++;
					goto END_OF_LOOP;
				} else goto END_OF_LOOP;
						
			}

		printError(countLine, "unidentified line");
		errorsCounter++;
		END_OF_LOOP: ;

	}
	for (i = 0; i < countLabel; i++)
		if (labelTable[i].ext==FALSE){
			if(labelTable[i].cmd == FALSE) 
				labelTable[i].address = labelTable[i].address + *IC +FIRST_ADDRESS;
			else labelTable[i].address = labelTable[i].address +FIRST_ADDRESS;
		}
	if (errorsCounter)
		return FALSE;
	else
		return TRUE;

}
/*parse the command line and insert commands to array of instruction */
bool cmdParse(unsigned int *IC, int numOfCmd, int countLine) {
	char *operand1 = strtok(NULL, ",\n");
	char *operand2 = strtok(NULL, " 	\n");
	operand1 =removeWhiteSpace(operand1);
	operand2 =removeWhiteSpace(operand2);
	switch (numOfCmd) {
	case 0:
	case 2:
	case 3:
		if (!errorCmdG1(operand1, operand2, countLine))
			return FALSE;
		else
			insertCmdToArr(operand1, operand2, IC, numOfCmd);
		break;
	case 1:
		if (!errorCmdG2(operand1, operand2, countLine))
			return FALSE;
		else
			insertCmdToArr(operand1, operand2, IC, numOfCmd);
		break;
	case 6:
		if (!errorCmdG3(operand1, operand2, countLine))
			return FALSE;
		else
			insertCmdToArr(operand1, operand2, IC, numOfCmd);
		break;
	case 4:
	case 5:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 13:
		if (!errorCmdG4(operand1, operand2, countLine))
			return FALSE;
		else
			insertCmdToArr(operand1, operand2, IC, numOfCmd);
		break;
	case 12:
		if (!errorCmdG5(operand1, operand2, countLine))
			return FALSE;
		else
			insertCmdToArr(operand1, operand2, IC, numOfCmd);
		break;

	case 14:
	case 15:
		if (!errorCmdG6(operand1, operand2, countLine))
			return FALSE;
		else
			insertCmdToArr(operand1, operand2, IC, numOfCmd);
		break;
	}
	return TRUE;
}
/*insert commands line to array of instruction*/
void insertCmdToArr(char *operand1, char *operand2, unsigned int *IC,
		int numOfCmd) {
	int numOfOp1 = checkTypeOp(operand1);
	int numOfOp2 = checkTypeOp(operand2);
	if(!numOfOp2){
		numOfOp2=numOfOp1;
		numOfOp1=0;
	}
	numOfOp1 <<= 9;
	numOfOp2 <<= 2;
	numOfCmd <<= 5;
	instructionArr[*IC] = numOfOp1 + numOfOp2 + numOfCmd;
	if (checkTypeOp(operand1) == 0) {
		(*IC)++;
		return;
	}
	if (checkTypeOp(operand2) == 0 || (checkTypeOp(operand1) == 5 && checkTypeOp(operand2) == 5)) {
		*IC += 2;
		return;
	} else
		*IC += 3;
}
/*create label and insert to the table*/
void insertToTable(char *labelName, int address, bool cmd, bool ext) {
	if (allocLabel==countLabel)
	 {
	 allocLabel=allocLabel*2;
	 labelTable=realloc(labelTable,allocLabel*sizeof(label));
	 if (!labelTable)
	 {
	 printf("\nhave not memory for this program");
	 exit(0);
	 }
	 }
	 strcpy(labelTable[countLabel].label,labelName);
	 labelTable[countLabel].address=address;
	 labelTable[countLabel].cmd=cmd;
	 labelTable[countLabel].ext=ext;
	 labelTable[countLabel].ent=FALSE;
	 countLabel++;
}
/*insert string to array of data*/
bool insertString(char *line, unsigned int *DC, int lineCounter) {
	char *str =strtok(NULL,"\n");
	if (!legalString(str, lineCounter))
		return FALSE;
	str = removeWhiteSpace(str);
	str++;
	while (*str != '"') {
		if (!addNumToData((int) *str, DC, lineCounter))
			return FALSE;
		str++;
	}
	if (!addNumToData(0, DC, lineCounter))
		return FALSE;
	return TRUE;
}
/*insert numbers to array of data*/
bool insertData(char *line, unsigned int *DC, int lineCounter) {
	char *param =strtok(NULL,"\n");
	int num;
	if (!legalData(param, lineCounter))
		return FALSE;
	num = strtol(param, &param, 10);
	addNumToData(num, DC, lineCounter);
	param = removeWhiteSpace(param);
	while (*param) {
		param++;
		num = strtol(param, &param, 10);
		if (!addNumToData(num, DC, lineCounter)){
			return FALSE;}
		param = removeWhiteSpace(param);
	}
	return TRUE;
}
/*insert data to the array. return false if finished the place in the computer*/
bool addNumToData(int num, unsigned int *DC, int lineCounter) {
	if (*DC < 1024) {
		dataArr[*DC] = num;
		(*DC)++;
		return TRUE;
	} else {
		printError(lineCounter, "have not memory to this file");
		return FALSE;
	}
}
bool externInsert(char *label, int countLine) {
	int i;
	if(!label){
		printError(countLine, "extern empty");
		return FALSE;
	}
	if(strtok(NULL," 	\n")){
		printError(countLine, "too many parameters");
		return FALSE;
	}
	label=removeWhiteSpace(label);
	for (i = 0; i < countLabel; i++) {
		if (!strcmp(label, labelTable[i].label)) {
			printError(countLine, "the label is declared twice");
			return FALSE;
		}
	}
	if(errorLabel(label,countLine,labelTable,countLabel)){
		insertToTable(label,0, FALSE, TRUE);
		return TRUE;
	}
	else return FALSE;
}
/*remove white spaces from the head of the string
 * and return pointer to the first character*/
char *removeWhiteSpace(char *line) {
	if (line == NULL)
		return NULL;
	while ((isspace(*line)) && (strlen(line) > 0))
		line++;

	return line;
}
char *removeWhiteSpaceEnd(char *line) {
	char *end ;
	if (line == NULL)
		return NULL;
	end= line+strlen(line) - 1;
	while (isspace(*end))
		end--;
	end++;
	*end = '\0';
	return line;
}
