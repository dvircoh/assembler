#ifndef ASEMBLER_H_
#define ASEMBLER_H_
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>



#define MAX_LABEL           	31
#define MAX_LINE            	80
#define FALSE			0
#define TRUE			1
#define MAX_MEMORY          	1024
#define NUM_OF_CMD          	16
#define LENGTH_OF_CMD       	5
#define NUM_OF_SAVE_WORDS   	28
#define LENGTH_OF_SAVE_WORDS	7
#define NUM_OF_REGISTERS    	8
#define EXTERNAL_ENCODE    	1
#define RELOCATABLE_ENCODE    	2
#define FIRST_ADDRESS		100
typedef unsigned int bool; /* Get TRUE or FALSE values */
typedef struct {
	char label[MAX_LABEL];
	int address;
	bool cmd;
	bool ext;
	bool ent;
} label;

/*functions of parser*/
bool parserFile(FILE *file);
char *removeWhiteSpace(char *line);
bool addNumToData(int num, unsigned int *DC, int lineCounter);
bool insertData(char *line, unsigned int *DC, int lineCounter);
bool insertString(char *line, unsigned int *DC, int lineCounter);
void insertToTable(char *labelName, int address, bool cmd, bool ext);
void insertCmdToArr(char *operand1, char *operand2, unsigned int *IC,
		int numOfCmd);
void printError(int lineCounter, char *str);
bool cmdParse(unsigned int *IC, int numOfCmd, int countLine);
bool externInsert(char *label, int countLine) ;
char *removeWhiteSpaceEnd(char *line);

/*functions of checks*/
bool legalData(char *line, int lineCounter);
bool legalString(char *line, int lineCounter);
bool errorLabel(char *labelName, int countLine, label *labelTable,
		int countLabel);
bool checkRegister(char *reg, int countLine);
bool errorCmdG1(char *operand1, char *operand2, int countLine);
bool errorCmdG2(char *operand1, char *operand2, int countLine);
bool errorCmdG3(char *operand1, char *operand2, int countLine);
bool errorCmdG4(char *operand1, char *operand2, int countLine);
bool errorCmdG5(char *operand1, char *operand2, int countLine);
bool errorCmdG6(char *operand1, char *operand2, int countLine);
int checkTypeOp(char *operand);

/*functions of secondRead*/
bool secondRead(FILE *file);
bool operandsParse(unsigned int *IC, int countLine);
bool operandEncode(char *operand, unsigned int *IC, int operandNumber,
		int countLine);
int addressLabel(char *label, int countLine,int IC);
bool entryInsert(char *label, int countLine);
void twoRegistersEncode(char *reg1, char *reg2, unsigned int *IC);
bool twoOperandsParse(char *op1,char *op2,unsigned int *IC, int countLine);
bool oneOperandsParse(char *op1,unsigned int *IC, int countLine);

/*functions of output*/
const char *encodeToBase64(int data, char *result);
void createObjectFile();
void createExternFile(label *labelTable, int countLabel);
void createEntryFile(label *labelTable, int countLabel);
bool chackEntry(label *labelTable, int countLabel);
bool chackExtern(label *labelTable, int countLabel);

/*functions of intialization*/
void initializeAndClose(FILE *file,label *labelTable);
