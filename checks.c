#include "assembler.h"
extern label *labelTable;
extern int countLabel;
const char saveWords[NUM_OF_SAVE_WORDS][LENGTH_OF_SAVE_WORDS] = { "mov", "cmp",
		"add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red",
		"prn", "jsr", "rts", "stop", "entry", "extern", "data", "string", "r0",
		"r1", "r2", "r3", "r4", "r5", "r6", "r7" };
/*The function checks if the register is valid. Returns true if yes and false if not*/
bool checkRegister(char *reg, int countLine) {
	int i;
	if (!isdigit(reg[2])) {
		printError(countLine, "register not legal");
		return FALSE;
	}
	i = atoi(&reg[2]);
	if (i >= NUM_OF_REGISTERS) {
		printError(countLine, "register not legal");
		return FALSE;
	}
	if (!removeWhiteSpace(reg)) {
		printError(countLine, "register not legal");
		return FALSE;
	} else
		return TRUE;
}
/*The function checks the operand and returns the addressing method*/
int checkTypeOp(char *operand) {
	if(operand==NULL)
		return 0;
	if (isdigit(*operand)||*operand=='-'||*operand=='+')
		return 1;
	if (*operand == '@')
		return 5;
	if (isalpha(*operand))
		return 3;
		printError(1, "what happend");
	return 0;
}

bool errorCmdG1(char *operand1, char *operand2, int countLine) {
	bool flug_op1 = FALSE;
	bool flug_op2 = FALSE;

	if (!*operand1) {
		printError(countLine, "There are no command operands");
		return FALSE;
	}
	if (!*operand2) {
		printError(countLine, "only one operand");
		return FALSE;
	}
	if (*operand1 == '@') {
		if (!checkRegister(operand1, countLine))
			return FALSE;
		else if (!*removeWhiteSpace(&operand1[+2])) {
			printError(countLine, "operand not legal");
			return FALSE;
		} else
			flug_op1 = TRUE;
	}
	if (isdigit(*operand1)||((*operand1=='-'||*operand1=='+')&&isdigit(operand1[1]))) {
		char *ptr;
		strtol(operand1, &ptr, 10);
		if (*ptr || *removeWhiteSpace(ptr)) {
			printError(countLine, "operand not legal");
			return FALSE;
		} else
			flug_op1 = TRUE;

	}
	if (isalpha(*operand1)) {
		char *end = &operand1[strlen(operand1) - 1];
		while (isspace(*end))
			end--;
		end++;
		*end = '\0';
		if (!errorLabel(operand1, countLine, labelTable, countLabel))
			return FALSE;
		else
			flug_op1 = TRUE;

	}

	if (*operand2 == '@') {
		if (!checkRegister(operand2, countLine))
			return FALSE;
		else if (!*removeWhiteSpace(&operand2[+2])) {
			printError(countLine, "operand not legal");
			return FALSE;
		} else
			flug_op2 = TRUE;

	}

	if (isalpha(*operand2)) {
		char *end = &operand2[strlen(operand2) - 1];
		while (isspace(*end))
			end--;
		end++;
		end = '\0';
		if (!errorLabel(operand2, countLine, labelTable, countLabel))
			return FALSE;
		else
			flug_op2 = TRUE;
	}
	if (flug_op2 && flug_op1)
		return TRUE;
	else
		return FALSE;
}

bool errorCmdG2(char *operand1, char *operand2, int countLine) {
	bool flug_op1 = FALSE;
	bool flug_op2 = FALSE;
	if (!*operand1) {
		printError(countLine, "There are no command operands");
		return FALSE;
	}
	if (!*operand2) {
		printError(countLine, "only one operand");
		return FALSE;
	}
	operand1 = removeWhiteSpace(operand1);
	if (*operand1 == '@') {
		if (!checkRegister(operand1, countLine))
			return FALSE;
		else if (!*removeWhiteSpace(&operand1[+2])) {
			printError(countLine, "operand not legal");
			return FALSE;
		} else
			flug_op1 = TRUE;

	}
	if (isdigit(*operand1)||((*operand1=='-'||*operand1=='+')&&isdigit(operand1[1]))) {
		char *ptr;
		strtol(operand1, &ptr, 10);
		if (*ptr || *removeWhiteSpace(ptr)) {
			printError(countLine, "operand not legal");
			return FALSE;
		} else
			flug_op1 = TRUE;

	}
	if (isalpha(*operand1)) {
		char *end = &operand1[strlen(operand1) - 1];
		while (isspace(*end))
			end--;
		end++;
		*end = '\0';
		if (!errorLabel(operand1, countLine, labelTable, countLabel))
			return FALSE;
		else
			flug_op1 = TRUE;

	}

	if (*operand2 == '@') {
		if (!checkRegister(operand2, countLine))
			return FALSE;
		else if (!*removeWhiteSpace(&operand2[+2])) {
			printError(countLine, "operand not legal");
			return FALSE;
		} else
			flug_op2 = TRUE;

	}
	if (isdigit(*operand2)||((*operand2=='-'||*operand2=='+')&&isdigit(operand2[1]))){
		char *ptr;
		strtol(operand2, &ptr, 10);
		if (*ptr || *removeWhiteSpace(ptr)) {
			printError(countLine, "operand not legal");
			return FALSE;
		} else
			flug_op2 = TRUE;

	}
	if (isalpha(*operand2)) {
		char *end = &operand2[strlen(operand2) - 1];
		while (isspace(*end))
			end--;
		end++;
		*end = '\0';
		if (!errorLabel(operand2, countLine, labelTable, countLabel))
			return FALSE;
		else
			flug_op2 = TRUE;

	}
	if (flug_op2 && flug_op1)
		return TRUE;
	else
		return FALSE;
}

bool errorCmdG3(char *operand1, char *operand2, int countLine) {
	bool flug_op1 = FALSE;
	bool flug_op2 = FALSE;
	if (!*operand1) {
		printError(countLine, "There are no command operands");
		return FALSE;
	}
	if (!*operand2) {
		printError(countLine, "only one operand");
		return FALSE;
	}

	if (isalpha(*operand1)) {
		operand1 = removeWhiteSpaceEnd(operand1);
		if (!errorLabel(operand1, countLine, labelTable, countLabel))
			return FALSE;
		else
			flug_op1 = TRUE;

	}
	operand2 = removeWhiteSpace(operand2);
	if (*operand2 == '@') {
		if (!checkRegister(operand2, countLine))
			return FALSE;
		else if (!*removeWhiteSpace(&operand2[+2])) {
			printError(countLine, "too many operands for command");
			return FALSE;
		} else
			flug_op2 = TRUE;

	}
	if (isalpha(*operand2)) {
		operand2 = removeWhiteSpaceEnd(operand2);
		if (!errorLabel(operand1, countLine, labelTable, countLabel))
			return FALSE;
		else
			flug_op2 = TRUE;

	}
	if (flug_op2 && flug_op1)
		return TRUE;
	else
		return FALSE;
}

bool errorCmdG4(char *operand1, char *operand2, int countLine) {

	if (!*operand1) {
		printError(countLine, "There are no command operands");
		return FALSE;
	}
	if (operand2) {
		printError(countLine, "too many operands for command");
		return FALSE;
	}
	if (*operand1 == '@') {
		if (!checkRegister(operand1, countLine))
			return FALSE;
		else if (!*removeWhiteSpace(&operand1[+2])) {
			printError(countLine, "too many operands for command");
			return FALSE;
		} else
			return TRUE;
	}
	if (isalpha(*operand1)) {
		operand1 = removeWhiteSpaceEnd(operand1);
		if (!errorLabel(operand1, countLine, labelTable, countLabel))
			return FALSE;
		else
			return TRUE;
	}
	return FALSE;
}

bool errorCmdG5(char *operand1, char *operand2, int countLine) {
	if (!*operand1) {
		printError(countLine, "There are no command operands");
		return FALSE;
	}
	if (operand2) {
		printError(countLine, "too many operands for command");
		return FALSE;
	}

	if (*operand1 == '@') {
		if (!checkRegister(operand1, countLine))
			return FALSE;
		else if (!*removeWhiteSpace(&operand1[+2])) {
			printError(countLine, "too many operands for command");
			return FALSE;
		} else
			return TRUE;
	}
	if (isdigit(*operand1)||((*operand1=='-'||*operand1=='+')&&isdigit(operand1[1]))) {
		char *ptr;
		strtol(operand1, &ptr, 10);
		if (!*ptr || !*removeWhiteSpace(ptr))
			return TRUE;
		else {
			printError(countLine, "operand not legal");
			return FALSE;
		}
	}
	if (isalpha(*operand1)) {
		char *end = &operand1[strlen(operand1) - 1];
		while (isspace(*end))
			end--;
		end++;
		*end = '\0';
		if (!errorLabel(operand1, countLine, labelTable, countLabel))
			return FALSE;
		else
			return TRUE;
	}
	return FALSE;
}

bool errorCmdG6(char *operand1, char *operand2, int countLine) {

	if (!operand1)
		return TRUE;
	else {
		printError(countLine, "too many operands for command");
		return FALSE;
	}

}
/*checks that the label write legal. return false if not.*/
bool errorLabel(char *labelName, int countLine, label *labelTable,
		int countLabel) {
	int i;
	char *ptr = labelName;
	if (strlen(labelName) > MAX_LABEL) {
		printError(countLine, "the label longer");
		return FALSE;
	}
	if (!isalpha(*ptr)) {
		printError(countLine, "label must begin with letter");
		return FALSE;
	}
	while (*ptr) {
		if (!isalnum(*ptr)) {
			printError(countLine, "label must consist of letters and numbers");
			return FALSE;
		}
		ptr++;
	}
	for (i = 0; i < NUM_OF_SAVE_WORDS; i++) {
		if (!strcmp(labelName, saveWords[i])) {
			printError(countLine, "label use at save word");
			return FALSE;
		}
	}
	return TRUE;
}
/*checks that the string write legal. return false if not.*/
bool legalString(char *str, int lineCounter) {
	char *end=str;
	str = removeWhiteSpace(str);
	if (!str)
	{
		printError(lineCounter, "no string");
		return FALSE;
	}
	removeWhiteSpaceEnd(str);
	end = str+strlen(str) - 1;
	if (*str != '"' || *end != '"') {
		printError(lineCounter, "string not legal");
		return FALSE;
	}
	str++;
	while (str != end)
		if (!isprint(*str)) {
			printError(lineCounter, "string not legal");
			return FALSE;
		} else
			str++;
	return TRUE;

}
/*checks that the numbers write legal. return false if not.*/
bool legalData(char *param, int lineCounter) {
	
	param = removeWhiteSpace(param);
	if (!param) {
		printError(lineCounter, "have not parameters");
		return FALSE;
	}
	if (*param == '0')
		strtol(param, &param, 10);
	else if (!strtol(param, &param, 10)) {
		printError(lineCounter, "parameter not legal");
		return FALSE;
	}
	param = removeWhiteSpace(param);
	while (*param) {
		if (*param != ',') {
			printError(lineCounter, "parameter not legal");
			return FALSE;
		}
		param++;
		if (*param == '0')
			strtol(param, &param, 10);
		else if (!strtol(param, &param, 10)) {
			printError(lineCounter, "parameter not legal");
			return FALSE;
		}
		param = removeWhiteSpace(param);
	}
	return TRUE;
}
