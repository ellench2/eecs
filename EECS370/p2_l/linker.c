/**
 * Project 2
 * LC-2K Linker
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXSIZE 300
#define MAXLINELENGTH 1000
#define MAXFILES 6

typedef struct FileData FileData;
typedef struct SymbolTableEntry SymbolTableEntry;
typedef struct RelocationTableEntry RelocationTableEntry;
typedef struct CombinedFiles CombinedFiles;

struct SymbolTableEntry {
	char label[7];
	char location;
	int offset;
};

struct RelocationTableEntry {
	int offset;
	char inst[7];
	char label[7];
	int file;
};

struct FileData {
	int textSize;
	int dataSize;
	int symbolTableSize;
	int relocationTableSize;
	int textStartingLine; // in final executable
	int dataStartingLine; // in final executable
	int text[MAXSIZE];
	int data[MAXSIZE];
	SymbolTableEntry symbolTable[MAXSIZE];
	RelocationTableEntry relocTable[MAXSIZE];
};

struct CombinedFiles {
	int text[MAXSIZE];
	int data[MAXSIZE];
	SymbolTableEntry     symTable[MAXSIZE];
	RelocationTableEntry relocTable[MAXSIZE];
	int textSize;
	int dataSize;
	int symTableSize;
	int relocTableSize;
};

static inline int convertNum(int num)
{
    return num - ( (num & (1<<15)) ? 1<<16 : 0 );
}

int main(int argc, char *argv[])
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr; 
	int i, j;

	if (argc <= 2) {
		printf("error: usage: %s <obj file> ... <output-exe-file>\n",
				argv[0]);
		exit(1);
	}

	outFileString = argv[argc - 1];

	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	FileData files[MAXFILES];

  // read in all files and combine into a "master" file
	for (i = 0; i < argc - 2; i++) {
		inFileString = argv[i+1];

		inFilePtr = fopen(inFileString, "r");
		printf("opening %s\n", inFileString);

		if (inFilePtr == NULL) {
			printf("error in opening %s\n", inFileString);
			exit(1);
		}

		char line[MAXLINELENGTH];
		int sizeText, sizeData, sizeSymbol, sizeReloc;

		// parse first line of file
		fgets(line, MAXSIZE, inFilePtr);
		sscanf(line, "%d %d %d %d",
				&sizeText, &sizeData, &sizeSymbol, &sizeReloc);

		files[i].textSize = sizeText;
		files[i].dataSize = sizeData;
		files[i].symbolTableSize = sizeSymbol;
		files[i].relocationTableSize = sizeReloc;

		// read in text section
		int instr;
		for (j = 0; j < sizeText; j++) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			instr = atoi(line);
			files[i].text[j] = instr;
		}

		// read in data section
		int data;
		for (j = 0; j < sizeData; j++) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			data = atoi(line);
			files[i].data[j] = data;
		}

		// read in the symbol table
		char label[7];
		char type;
		int addr;
		for (j = 0; j < sizeSymbol; j++) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			sscanf(line, "%s %c %d",
					label, &type, &addr);
			files[i].symbolTable[j].offset = addr;
			strcpy(files[i].symbolTable[j].label, label);
			files[i].symbolTable[j].location = type;
		}

		// read in relocation table
		char opcode[7];
		for (j = 0; j < sizeReloc; j++) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			sscanf(line, "%d %s %s",
					&addr, opcode, label);
			files[i].relocTable[j].offset = addr;
			strcpy(files[i].relocTable[j].inst, opcode);
			strcpy(files[i].relocTable[j].label, label);
			files[i].relocTable[j].file	= i;
		}
		fclose(inFilePtr);
	} // end reading files

	// *** INSERT YOUR CODE BELOW ***
	//    Begin the linking process
	//    Happy coding!!!
    
    //scan all symbol tables, add to big symbol table, check each time if already added
    //if so, oopsie
    //also check if defining Stack label
    //if so, oopsie
    CombinedFiles result;
    result.symTableSize = 0;
    for (int i = 0; i < argc - 2; i++) {
        for (int j = 0; j < files[i].symbolTableSize; j++) {
            if (files[i].symbolTable[j].location != 'U' &&
                !strcmp(files[i].symbolTable[j].label, "Stack")) {
                exit(1);
            }
            for (int k = 0; k < result.symTableSize; k++) {
                if (!strcmp(result.symTable[k].label, files[i].symbolTable[j].label) &&
                    files[i].symbolTable[j].location != 'U') {
                    exit(1);
                }
            }
            if (files[i].symbolTable[j].label[0] < 91 && files[i].symbolTable[j].location != 'U') {
                for (int l = 0; l < 7; l++) {
                    if (files[i].symbolTable[j].label[l] != '\0') {
                        result.symTable[result.symTableSize].label[l] = files[i].symbolTable[j].label[l];
                    }
                    else {
                        result.symTable[result.symTableSize].label[l] = files[i].symbolTable[j].label[l];
                        break;
                    }
                }
                result.symTableSize++;
            }
        }
    }
    
    for (int i = 0; i < argc - 2; i++) {
        for (int j = 0; j < files[i].relocationTableSize; j++) {
            if (files[i].relocTable[j].label[0] < 91 && strcmp(files[i].relocTable[j].label, "Stack")) {
                int found = -1;
                for (int k = 0; k < result.symTableSize; k++) {
                    if (!strcmp(result.symTable[k].label, files[i].relocTable[j].label)) {
                        found = k;
                    }
                }
                if (found == -1) {
                    exit(1);
                }
            }
        }
    }
    //after that, scan all relocation tables, if global label NOT FOUND in big symbol table, oopsie
    
    
    

    int currtextline = 0;
    int currdataline = 0;
    for (int i = 0; i < argc - 2; i++) { // loop through files[]
        //add text to combined files text
        for (int j = currtextline; j < files[i].textSize + currtextline; j++) {
            result.text[j] = files[i].text[j-currtextline];
        }
        currtextline += files[i].textSize;
        //add data to combined files data
        for (int j = currdataline; j < files[i].dataSize + currdataline; j++) {
            result.data[j] = files[i].data[j-currdataline];
        }
        currdataline += files[i].dataSize;
        
        for (int k = 0; k < files[i].relocationTableSize; k++) { // loop through relocation table
            if (!strcmp(files[i].relocTable[k].label, "Stack")) {
                int sum = 0;
                for (int l = 0; l < argc - 2; l++) {
                    sum += files[l].dataSize;
                    sum += files[l].textSize;
                }
                if (!strcmp(files[i].relocTable[k].inst, "lw") ||
                    !strcmp(files[i].relocTable[k].inst, "sw")) {
                    int add = 0;
                    for (int l = 0; l < i; l++) {
                        add += files[l].textSize;
                    }
                    result.text[add + files[i].relocTable[k].offset] += sum;
                }
                else {
                    int prevdata = 0;
                    for (int l = 0; l < i; l++) {
                        prevdata += files[l].dataSize;
                    }
                    result.data[prevdata + files[i].relocTable[k].offset] += sum;
                }
            }
            else if (files[i].relocTable[k].label[0] < 91) { // global label
                int labelloc = 0;
                int foundlabel = 0;
                int foundlocal = 0;
                for (int l = 0; l < argc - 2; l++) { // loop through files[]
                    for (int m = 0; m < files[l].symbolTableSize; m++) { // loop through symbol table
                        // compare files[l].symbolTable[m].label
                        // make sure label is being defined there
                        // if in data:
                            // add up length of all texts and length of all datas that came before
                            // the one where definition is happening, plus offset from symbol table
                        // if in text:
                            // add up length of all texts that came before the one
                            // where definition is happening, plus offset from symbol table
                        if (!strcmp(files[l].symbolTable[m].label, files[i].relocTable[k].label)
                            && files[l].symbolTable[m].location != 'U') {
                            if (files[l].symbolTable[m].location == 'D') {
                                if (l == i) {
                                    foundlocal = 1;
                                    //just like local label
                                    if (!strcmp(files[i].relocTable[k].inst, "lw") ||
                                        !strcmp(files[i].relocTable[k].inst, "sw")) {
                                        int curroffsetfield = convertNum(files[i].text[files[i].relocTable[k].offset] & 65535);
                                        // CHECK IF THIS EXTRACTS CORRECTLY!!! it should tho
                                        if (curroffsetfield < files[i].textSize) {
                                            int add = 0;
                                            for (int n = 0; n < i; n++) {
                                                add += files[n].textSize;
                                            }
                                            result.text[currtextline+files[i].relocTable[k].offset  - files[i].textSize] += add;
                                        }
                                        else {
                                            int add = 0;
                                            for (int n = i + 1; n < argc - 2; n++) {
                                                add += files[n].textSize;
                                            }
                                            int prevtext = 0;
                                            for (int n = 0; n < i; n++) {
                                                prevtext += files[n].textSize;
                                            }
                                            int adddata = 0;
                                            for (int n = 0; n < i; n++) {
                                                adddata += files[n].dataSize;
                                            }
                                            result.text[prevtext+files[i].relocTable[k].offset] += (prevtext + add + adddata);
                                        }
                                    }
                                    else if (!strcmp(files[i].relocTable[k].inst, ".fill")) {
                                        int curroffsetfield = convertNum(files[i].data[files[i].relocTable[k].offset]);
                                        if (curroffsetfield < files[i].textSize) {
                                            int addtext = 0;
                                            for (int n = 0; n < i; n++) {
                                                addtext += files[n].textSize;
                                            }
                                            int adddata = 0;
                                            for (int n = 0; n < i; n++) {
                                                adddata += files[n].dataSize;
                                            }
                                            result.data[adddata + files[i].relocTable[k].offset] += addtext;
                                        }
                                        else {
                                            int prevtext = 0;
                                            for (int n = 0; n < i; n++) { //CHANGED
                                                prevtext += files[n].textSize;
                                            }
                                            int posttext = 0;
                                            for (int n = i+1; n < argc - 2; n++) { //CHANGED
                                                posttext += files[n].textSize;
                                            }
                                            int prevdata = 0;
                                            for (int n = 0; n < i; n++) {
                                                prevdata += files[n].dataSize;
                                            }
                                            result.data[prevdata + files[i].relocTable[k].offset] += (prevtext + posttext + prevdata);
                                        }
                                    }
                                    else {
                                        //somethings fucked up
                                    }
                                    break;
                                }
                                else {
                                    for (int n = 0; n < argc - 2; n++) {
                                        labelloc += files[n].textSize;
                                    }
                                    for (int n = 0; n < l; n++) {
                                        labelloc += files[n].dataSize;
                                    }
                                    labelloc += files[l].symbolTable[m].offset;
                                    foundlabel = 1;
                                    break;
                                }
                            }
                            else if (files[l].symbolTable[m].location == 'T') {
                                if (l == i) {
                                    //just like local label
                                    foundlocal = 1;
                                    if (!strcmp(files[i].relocTable[k].inst, "lw") ||
                                        !strcmp(files[i].relocTable[k].inst, "sw")) {
                                        int curroffsetfield = convertNum(files[i].text[files[i].relocTable[k].offset] & 65535);
                                        // CHECK IF THIS EXTRACTS CORRECTLY!!! it should tho
                                        if (curroffsetfield < files[i].textSize) {
                                            int add = 0;
                                            for (int n = 0; n < i; n++) {
                                                add += files[n].textSize;
                                            }
                                            result.text[currtextline+files[i].relocTable[k].offset - files[i].textSize] += add;
                                        }
                                        else {
                                            int add = 0;
                                            for (int n = i + 1; n < argc - 2; n++) {
                                                add += files[n].textSize;
                                            }
                                            int prevtext = 0;
                                            for (int n = 0; n < i; n++) {
                                                prevtext += files[n].textSize;
                                            }
                                            int adddata = 0;
                                            for (int n = 0; n < i; n++) {
                                                adddata += files[n].dataSize;
                                            }
                                            result.text[prevtext+files[i].relocTable[k].offset] += (prevtext + add + adddata);
                                        }
                                    }
                                    else if (!strcmp(files[i].relocTable[k].inst, ".fill")) {
                                        int curroffsetfield = convertNum(files[i].data[files[i].relocTable[k].offset]);
                                        if (curroffsetfield < files[i].textSize) {
                                            int addtext = 0;
                                            for (int n = 0; n < i; n++) {
                                                addtext += files[n].textSize;
                                            }
                                            int adddata = 0;
                                            for (int n = 0; n < i; n++) {
                                                adddata += files[n].dataSize;
                                            }
                                            result.data[adddata + files[i].relocTable[k].offset] += addtext;
                                        }
                                        else {
                                            int addtext = 0;
                                            for (int n = 0; n < i; n++) {
                                                addtext += files[n].textSize;
                                            }
                                            int adddata = 0;
                                            for (int n = 0; n < i; n++) {
                                                adddata += files[n].dataSize;
                                            }
                                            result.data[adddata + files[i].relocTable[k].offset] += (addtext + adddata);
                                        }
                                    }
                                    else {
                                        //somethings fucked up
                                    }
                                    break;
                                }
                                else {
                                    for (int n = 0; n < l; n++) {
                                        labelloc += files[n].textSize;
                                    }
                                    labelloc += files[l].symbolTable[m].offset;
                                    foundlabel = 1;
                                    break;
                                }
                            }
                            else {
                                //somethings fucked up
                            }
                        }
                    }
                    if (foundlabel == 1) {
                        break;
                    }
                }
                //here, we know what the correct location of the label is!
                //now we think about where we have to look to change the offset to that label location
                if (foundlocal == 0) {
                    if (!strcmp(files[i].relocTable[k].inst, "lw") ||
                        !strcmp(files[i].relocTable[k].inst, "sw")) {
                        int fixloc = 0;
                        for (int l = 0; l < i; l++) {
                            fixloc += files[l].textSize;
                        }
                        fixloc += files[i].relocTable[k].offset;
                        result.text[fixloc] += labelloc;
                    }
                    else {
                        int fixloc = 0;
                        for (int l = 0; l < i; l++) {
                            fixloc += files[l].dataSize;
                        }
                        fixloc += files[i].relocTable[k].offset;
                        result.data[fixloc] += labelloc;
                    }
                }
            }
            else { // local label
                // if in text
                    // go to machine code, extract offsetfield, if >= length of text in
                    // current file, defined in data; else in text
                    // if defined in text
                        // add to current offsetfield length of texts that came before this one
                    // if defined in data
                        // add up length of rest of texts, add up length of data's that
                        // come before current data section, plus offset from machine code
                if (!strcmp(files[i].relocTable[k].inst, "lw") ||
                    !strcmp(files[i].relocTable[k].inst, "sw")) {
                    int curroffsetfield = convertNum(files[i].text[files[i].relocTable[k].offset] & 65535);
                    // CHECK IF THIS EXTRACTS CORRECTLY!!! it should tho
                    if (curroffsetfield < files[i].textSize) {
                        int add = 0;
                        for (int n = 0; n < i; n++) {
                            add += files[n].textSize;
                        }
                        result.text[currtextline+files[i].relocTable[k].offset - files[i].textSize] += add;
                    }
                    else {
                        int add = 0;
                        for (int n = i + 1; n < argc - 2; n++) {
                            add += files[n].textSize;
                        }
                        int prevtext = 0;
                        for (int n = 0; n < i; n++) {
                            prevtext += files[n].textSize;
                        }
                        int adddata = 0;
                        for (int n = 0; n < i; n++) {
                            adddata += files[n].dataSize;
                        }
                        result.text[prevtext+files[i].relocTable[k].offset] += (prevtext + add + adddata);
                    }
                }
                // if in data
                    // go to machine code, extract offsetfield, if >= length of text in
                    // current file, defined in data; else in text
                    // if defined in text
                        // add up lengths of texts that come before this one, add to offset from machine code
                    // if defined in data
                        // add up lengths of texts and datas that came before this one, add to offset
                        // from machine code
                else if (!strcmp(files[i].relocTable[k].inst, ".fill")) {
                    int curroffsetfield = convertNum(files[i].data[files[i].relocTable[k].offset]);
                    if (curroffsetfield < files[i].textSize) {
                        int addtext = 0;
                        for (int n = 0; n < i; n++) {
                            addtext += files[n].textSize;
                        }
                        int adddata = 0;
                        for (int n = 0; n < i; n++) {
                            adddata += files[n].dataSize;
                        }
                        result.data[adddata + files[i].relocTable[k].offset] += addtext;
                    }
                    else {
                        int addtext = 0;
                        for (int n = 0; n < i; n++) {
                            addtext += files[n].textSize;
                        }
                        int adddata = 0;
                        for (int n = 0; n < i; n++) {
                            adddata += files[n].dataSize;
                        }
                        result.data[adddata + files[i].relocTable[k].offset] += (addtext + adddata);
                    }
                }
                else {
                    //somethings fucked up
                }
            }
        }
        
    }
    //now want to output corrected text and data to specified machine code file
    int lentext = 0;
    for (int i = 0; i < argc - 2; i++) {
        lentext += files[i].textSize;
    }
    int lendata = 0;
    for (int i = 0; i < argc - 2; i++) {
        lendata += files[i].dataSize;
    }
    for (int i = 0; i < lentext; i++) {
        fprintf(outFilePtr, "%i", result.text[i]);
        fprintf(outFilePtr, "\n");
    }
    for (int i = 0; i < lendata; i++) {
        fprintf(outFilePtr, "%i", result.data[i]);
        fprintf(outFilePtr, "\n");
    }
    exit(0);
} // main
