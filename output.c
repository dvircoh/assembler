#include "assembler.h"

extern unsigned int instructionArr[1025];
extern unsigned int dataArr[1025];
extern char* fileName;
/*Creates a file with the labels declared "entry".*/
void createEntryFile(label *labelTable, int countLabel) {
	char entryFileName[FILENAME_MAX];
	FILE* f;
	int i;
	bool flugEntry = chackEntry(labelTable, countLabel);
	strcpy(entryFileName, fileName);
	strcat(entryFileName, ".ent");
	if (flugEntry)
		f = fopen(entryFileName, "w");
	for (i = 0; i < countLabel; i++)
		if (labelTable[i].ent)
			fprintf(f, "\n%s      %d", labelTable[i].label,
					labelTable[i].address);
}
/*Creates a file with the labels declared "extern".*/
void createExternFile(label *labelTable, int countLabel) {
	char externFileName[FILENAME_MAX];
	FILE* f;
	int i;
	bool flugExtern = chackExtern(labelTable, countLabel);
	strcpy(externFileName, fileName);
	strcat(externFileName, ".ext");
	if (flugExtern)
		f = fopen(externFileName, "w");
	for (i = 0; i < countLabel; i++)
		if (labelTable[i].ext&&labelTable[i].address)
			fprintf(f, "\n%s      %d", labelTable[i].label,
					labelTable[i].address+FIRST_ADDRESS);
}
/*Creates a file containing the information that the assembler converted in base 64*/
void createObjectFile() {
	unsigned int *IC = &instructionArr[1024], *DC = &dataArr[1024];
	char objectFileName[FILENAME_MAX];
	FILE* f;
	int i;
	char result[3];
	strcpy(objectFileName, fileName);
	strcat(objectFileName, ".ob");
	f = fopen(objectFileName, "w");
	fprintf(f, "%d      %d", *IC, *DC);
	for (i = 0; i < *IC; i++)
		fprintf(f, "\n%s", encodeToBase64(instructionArr[i], result));
	for (i = 0; i < *DC; i++)
		fprintf(f, "\n%s", encodeToBase64(dataArr[i], result));
}
/*Converts the information to 64 base*/
const char *encodeToBase64(int data, char *result) {
	static char base64map[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
			'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
			'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
			'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
			'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8',
			'9', '+', '/' };
	int mask = 1, i;
	for (i = 1; i < 6; i++) {
		mask = mask << 1;
		mask += 1;
	}
	result[1] = base64map[mask & data];
	data = data >> 6;
	result[0] = base64map[mask & data];
	result[2] = '\0';
	return result;
}
/*Checks if have been labels that declared "extern"*/
bool chackExtern(label *labelTable, int countLabel) {
	int i;
	for (i = 0; i < countLabel; i++)
		if (labelTable[i].ext)
			return TRUE;
	return FALSE;
}
/*Checks if have been labels that declared "entry"*/
bool chackEntry(label *labelTable, int countLabel) {
	{
		int i;
		for (i = 0; i < countLabel; i++)
			if (labelTable[i].ent)
				return TRUE;
		return FALSE;
	}
}
