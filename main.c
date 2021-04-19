#include "assembler.h"
label *labelTable;
extern int countLabel;
int allocLabel = 1, countLabel = 0;
char* fileName;
/* Main method. Get name of the files open them end call to the methods for parse them */
int main(int argc, char* argv[]) {
	int i;
	char inputFile[FILENAME_MAX];
	FILE *file;
	labelTable = malloc(sizeof(label));
	if (argc == 1) {
		printf("No files to compile\n");
		return 0;
	}
	for (i = 1; i < argc; i++) {
		fileName = argv[i];
		strcpy(inputFile, argv[i]);
		strcat(inputFile, ".as");
		file = fopen(inputFile, "r");
		if (!file){
			printf(" Can't open the file %s \n", fileName);
			continue;
		}
		else if (parserFile(file))
			if (secondRead(file)) {
				createObjectFile();
				createExternFile(labelTable, countLabel);
				createEntryFile(labelTable, countLabel);
				}
	initializeAndClose(file,labelTable);

	} /*end of the for*/
	free(labelTable);
	return 0;
}
