/**
 * Project 1
 * Assembler code fragment for LC-2K
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
static inline int isNumber(char *);
int optype(char*);
void outputop(FILE *, char*, int* line);
void convertregA(char*, int*);
void convertregB(char*, int*);
void convertdest(char*, int*);
void convertIreg(char*, char[MAXLINELENGTH][6], int*, char*, int PC);
void convertfill(char*, char[MAXLINELENGTH][6], int*);
int convertdigits(char*);
int main(int argc, char **argv)
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

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
    
    char labels[MAXLINELENGTH][6] = {{0}};
    char symbols[MAXLINELENGTH][6] = {{0}};
    char symbol_loc[MAXLINELENGTH] = {0};
    char reloc[MAXLINELENGTH][6] = {{0}};
    char reloc_op[MAXLINELENGTH][6] = {{0}};
    int reloc_offset[MAXLINELENGTH] = {0};
    int symbol_offset[MAXLINELENGTH] = {0};
    int m = 0;
    int f = 0;
    int g = 0;
    int len_text = 0;
    int len_data = 0;
    int line_num = 0;
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        int check_address = -1;;
        for (int i = 0; i < MAXLINELENGTH; i++) {
            int test1 = 0;
            int test2 = 0;
            for (int k = 0; k < 6; k++) {
                if (labels[i][k] == '\0' && label[k] == '\0') {
                    break;
                }
                if (labels[i][k] == '\0' && label[k] != '\0') {
                    test1++;
                }
                if (labels[i][k] != '\0' && label[k] == '\0') {
                    test1++;
                }
                if (labels[i][k] == label[k]) {
                    test2++;
                }
                else {
                    test1++;
                    break;
                }
            }
            if (test2 > 0 && test1 == 0) {
                check_address = i;
                break;
            }
        }
        if (check_address == -1) {
            if (strcmp(label, "\0")) { // if this line has a label
                for (int k = 0; k < 6; k++) {
                    labels[m][k] = *(label+k);
                }
            }
            m++;
        }
        else {
            exit(1); // duplicate label definition
        }
    }
    rewind(inFilePtr);
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        //checked for defined local and global labels
        //now looking for use of undefined global labels, definition of global labels, and
        //use of any sort of label in lw, sw, and .fill
        if ((!strcmp(opcode, "lw") || !strcmp(opcode, "sw")) && !isNumber(arg2)) {
            for (int a = 0; a < 6; a++) {
                if (*(arg2+a) != '\0') {
                    reloc[g][a] = *(arg2+a);
                }
                else {
                    reloc[g][a] = *(arg2+a);
                    break;
                }
            }
            for (int b = 0; b < 6; b++) {
                if (*(opcode+b) != '\0')  {
                    reloc_op[g][b] = *(opcode+b);
                }
                else {
                    reloc_op[g][b] = *(opcode+b);
                    break;
                }
            }
            reloc_offset[g] = line_num;
            g++;
            //symbolic address in lw or sw
            //want to put in relocation table
            if (*arg2 < 91) {
                //have to first check that it is undefined
                
                int check_address = -1;
                for (int i = 0; i < MAXLINELENGTH; i++) {
                    int test1 = 0;
                    int test2 = 0;
                    for (int k = 0; k < 6; k++) {
                        if (labels[i][k] == '\0' && arg2[k] == '\0') {
                            break;
                        }
                        if (labels[i][k] == '\0' && arg2[k] != '\0') {
                            test1++;
                        }
                        if (labels[i][k] != '\0' && arg2[k] == '\0') {
                            test1++;
                        }
                        if (labels[i][k] == arg2[k]) {
                            test2++;
                        }
                        else {
                            test1++;
                            break;
                        }
                    }
                    if (test2 > 0 && test1 == 0) {
                        check_address = i;
                        break;
                    }
                }
                if (check_address == -1) { // if using undefined global label, then want to add to symbol table
                    //CHECK THAT WE HAVEN'T ALREADY ADDED TO SYMBOL TABLE!!
                    int check_symb = -1;
                    for (int j = 0; j < MAXLINELENGTH; j++) {
                        int test1s = 0;
                        int test2s = 0;
                        for (int l = 0; l < 6; l++) {
                            if (symbols[j][l] == '\0' & arg2[l] == '\0') {
                                break;
                            }
                            if (symbols[j][l] == '\0' & arg2[l] != '\0') {
                                test1s++;
                            }
                            if (symbols[j][l] != '\0' & arg2[l] == '\0') {
                                test1s++;
                            }
                            if (symbols[j][l] == arg2[l]) {
                                test2s++;
                            }
                            else {
                                test1s++;
                                break;
                            }
                        }
                        if (test2s > 0 && test1s == 0) {
                            check_symb = j;
                            break;
                        }
                    }
                    
                    if (check_symb == -1) {
                        //we have not yet added this label to the symbol table
                        for (int a = 0; a < 6; a++) {
                            if (*(arg2+a) != '\0') {
                                symbols[f][a] = *(arg2+a);
                            }
                            else {
                                symbols[f][a] = *(arg2+a);
                                break;
                            }
                        }
                        symbol_loc[f] = 'U';
                        symbol_offset[f] = 0;
                        f++;
                    }
                }
                //else, it's defined and doesn't need to be in symbol table
            }
        }
        if (optype(opcode) == 4 && !isNumber(arg0)) {
            for (int a = 0; a < 6; a++) {
                if (*(arg0+a) != '\0') {
                    reloc[g][a] = *(arg0+a);
                }
                else {
                    reloc[g][a] = *(arg0+a);
                    break;
                }
            }
            for (int b = 0; b < 6; b++) {
                if (*(opcode+b) != '\0')  {
                    reloc_op[g][b] = *(opcode+b);
                }
                else {
                    reloc_op[g][b] = *(opcode+b);
                    break;
                }
                
            }
            reloc_offset[g] = line_num - len_text;
            g++;
            //want to put in relocation table!
            if (*arg0 < 91) {
                //have to first check that it is undefined!
                
                int check_address = -1;
                for (int i = 0; i < MAXLINELENGTH; i++) {
                    int test1 = 0;
                    int test2 = 0;
                    for (int k = 0; k < 6; k++) {
                        if (labels[i][k] == '\0' && arg0[k] == '\0') {
                            break;
                        }
                        if (labels[i][k] == '\0' && arg0[k] != '\0') {
                            test1++;
                        }
                        if (labels[i][k] != '\0' && arg0[k] == '\0') {
                            test1++;
                        }
                        if (labels[i][k] == arg0[k]) {
                            test2++;
                        }
                        else {
                            test1++;
                            break;
                        }
                    }
                    if (test2 > 0 && test1 == 0) {
                        check_address = i;
                        break;
                    }
                }
                if (check_address == -1) { // if using undefined global label
                    //CHECK THAT WE HAVEN'T ALREADY ADDED TO SYMBOL TABLE
                    int check_symb = -1;
                    for (int j = 0; j < MAXLINELENGTH; j++) {
                        int test1s = 0;
                        int test2s = 0;
                        for (int l = 0; l < 6; l++) {
                            if (symbols[j][l] == '\0' & arg0[l] == '\0') {
                                break;
                            }
                            if (symbols[j][l] == '\0' & arg0[l] != '\0') {
                                test1s++;
                            }
                            if (symbols[j][l] != '\0' & arg0[l] == '\0') {
                                test1s++;
                            }
                            if (symbols[j][l] == arg0[l]) {
                                test2s++;
                            }
                            else {
                                test1s++;
                                break;
                            }
                        }
                        if (test2s > 0 && test1s == 0) {
                            check_symb = j;
                            break;
                        }
                    }
                    if (check_symb == -1) { // haven't yet added to symbol table
                        for (int a = 0; a < 6; a++) {
                            if (*(arg0+a) != '\0') {
                                symbols[f][a] = *(arg0+a);
                            }
                            else {
                                symbols[f][a] = *(arg0+a);
                                break;
                            }
                        }
                        symbol_loc[f] = 'U';
                        symbol_offset[f] = 0;
                        f++;
                    }
                }

            }
        }
        
        if (optype(opcode) != 4) {
            len_text++;
        }
        if (optype(opcode) == 4) {
            len_data++;
        }
        if (!(optype(opcode) == 3 || optype(opcode) == 4) && (!isNumber(arg0) || !isNumber(arg1) )) {
            exit(1); // error checking: misuse of labels
        }
        if (label[0] != '\0' && label[0] < 91 && optype(opcode) == 4) {
            //CHECK FOR DUPLICATE LABEL DEFINITION
                for (int a = 0; a < 6; a++) {
                    if (*(label+a) != '\0') {
                        symbols[f][a] = *(label+a);
                    }
                    else {
                        symbols[f][a] = *(label+a);
                        break;
                    }
                }
                symbol_loc[f] = 'D';
                symbol_offset[f] = line_num - len_text;
                f++;
            //if label is global and opcode is .fill, add to symbol table
        }
        if (label[0] != '\0' && label[0] < 91 && optype(opcode) != 4) {
            //CHECK FOR DUPLICATE LABEL DEFINITION
            int check_symb = -1;
            for (int j = 0; j < MAXLINELENGTH; j++) {
                int test1s = 0;
                int test2s = 0;
                for (int l = 0; l < 6; l++) {
                    if (symbols[j][l] == '\0' & label[l] == '\0') {
                        break;
                    }
                    if (symbols[j][l] == '\0' & label[l] != '\0') {
                        test1s++;
                    }
                    if (symbols[j][l] != '\0' & label[l] == '\0') {
                        test1s++;
                    }
                    if (symbols[j][l] == label[l]) {
                        test2s++;
                    }
                    else {
                        test1s++;
                        break;
                    }
                }
                if (test2s > 0 && test1s == 0) {
                    check_symb = j;
                    break;
                }
            }
            if (check_symb == -1) { // haven't yet added to symbol table
                for (int a = 0; a < 6; a++) {
                    if (*(label+a) != '\0') {
                        symbols[f][a] = *(label+a);
                    }
                    else {
                        symbols[f][a] = *(label+a);
                        break;
                    }
                }
                symbol_loc[f] = 'T';
                symbol_offset[f] = line_num;
                f++;
            }
            //label is global, in text section!
        }
        line_num++;
    }
    //printf("%i %i %i %i\n", len_text, len_data, f, g);
    fprintf(outFilePtr, "%i %i %i %i\n", len_text, len_data, f, g);
    rewind(inFilePtr);
    int counter = 0;
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        int temp = 0b0;
        int* line = &temp;

        switch (optype(opcode)) {
            case 0: {
                outputop(outFilePtr, opcode, line);
                convertregA(arg0, line);
                convertregB(arg1, line);
                convertdest(arg2, line); //needs twos complement
                break;
            }
            case 1: {
                outputop(outFilePtr, opcode, line);
                convertregA(arg0, line);
                convertregB(arg1, line);
                convertIreg(arg2, labels, line, opcode, counter); //need twos complement, also maybe just wrong
                break;
            }
            case 2: {
                outputop(outFilePtr, opcode, line);
                convertregA(arg0, line);
                convertregB(arg1, line);
                break;
            }
            case 3: {
                outputop(outFilePtr, opcode, line);
                break;
            }
            case 4: {
                convertfill(arg0, labels, line); //needs twos complement!
                break;
            }
            default: {
                exit(1); // unrecognized opcode
                break;
            }
        }
//
//        printf("%i", *line);
//        printf("\n");
        fprintf(outFilePtr, "%i", *line);
        fprintf(outFilePtr, "\n");
        counter++;
    }
    //print symbol table
    for (int q = 0; q < f; q++) {
        for (int p = 0; p < 6; p++) {
            if (symbols[q][p] != '\0') {
                //printf("%c", symbols[q][p]);
                fprintf(outFilePtr, "%c", symbols[q][p]);
            }
        }
//        printf(" %c", symbol_loc[q]);
//        printf(" %i\n", symbol_offset[q]);
        fprintf(outFilePtr, " %c", symbol_loc[q]);
        fprintf(outFilePtr, " %i\n", symbol_offset[q]);
    }
    
    for (int q = 0; q < g; q++) {
        fprintf(outFilePtr, "%i ", reloc_offset[q]);
        //printf("%i ", reloc_offset[q]);
        for (int p = 0; p < 6; p++) {
            if (reloc_op[q][p] != '\0') {
                //printf("%c", reloc_op[q][p]);
                fprintf(outFilePtr, "%c", reloc_op[q][p]);
            }
        }
        //printf(" ");
        fprintf(outFilePtr, " ");
        for (int p = 0; p < 6; p++) {
            if (reloc[q][p] != '\0') {
                //printf("%c", reloc[q][p]);
                fprintf(outFilePtr, "%c", reloc[q][p]);
            }
        }
        //printf("\n");
        fprintf(outFilePtr, "\n");
    }
    exit(0);
    return 1;
}

/*
* NOTE: The code defined below is not to be modifed as it is implimented correctly.
*/

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
int
readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
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

    /* check for line too long */
    if (strlen(line) == MAXLINELENGTH-1) {
    printf("error: line too long\n");
    exit(1);
    }

    // Treat a blank line as end of file.
    // Arguably, we could just ignore and continue, but that could
    // get messy in terms of label line numbers etc.
    char whitespace[4] = {'\t', '\n', '\r', ' '};
    int nonempty_line = 0;
    for(size_t line_idx = 0; line_idx < strlen(line); ++line_idx) {
        int line_char_is_whitespace = 0;
        for(int whitespace_idx = 0; whitespace_idx < 4; ++ whitespace_idx) {
            if(line[line_idx] == whitespace[whitespace_idx]) {
                ++line_char_is_whitespace;
                break;
            }
        }
        if(!line_char_is_whitespace) {
            ++nonempty_line;
            break;
        }
    }
    if(nonempty_line == 0) {
        return 0;
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
    /* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);

    return(1);
}

static inline int
isNumber(char *string)
{
    int num;
    char c;
    return((sscanf(string, "%d%c",&num, &c)) == 1);
}

int optype(char* opcode) {
    if (!strcmp(opcode, "add") || !strcmp(opcode, "nor")) {
        //r-type
        return 0;
    }
    else if (!strcmp(opcode, "lw") || !strcmp(opcode, "sw") || !strcmp(opcode, "beq")) {
        //i-type
        return 1;
    }
    else if (!strcmp(opcode, "jalr")) {
        //j-type
        return 2;
    }
    else if (!strcmp(opcode, "noop") || !strcmp(opcode, "halt")) {
        //o-type
        return 3;
    }
    else if (!strcmp(opcode, ".fill")) {
        //.fill
        return 4;
    }
    return 0;
}

void outputop(FILE * outfile, char* opcode, int* line) {
    if (!strcmp(opcode, "add")) {
        //nothing haha
    }
    else if (!strcmp(opcode, "nor")) {
        *line = *line >> 22;
        *line = *line | 1;
        *line = *line << 22;
    }
    else if (!strcmp(opcode, "lw")) {
        *line = *line >> 22;
        *line = *line | 2;
        *line = *line << 22;
    }
    else if (!strcmp(opcode, "sw")) {
        *line = *line >> 22;
        *line = *line | 3;
        *line = *line << 22;
    }
    else if (!strcmp(opcode, "beq")) {
        *line = *line >> 22;
        *line = *line | 4;
        *line = *line << 22;
    }
    else if (!strcmp(opcode, "jalr")) {
        *line = *line >> 22;
        *line = *line | 5;
        *line = *line << 22;
    }
    else if (!strcmp(opcode, "halt")) {
        *line = *line >> 22;
        *line = *line | 6;
        *line = *line << 22;
    }
    else if (!strcmp(opcode, "noop")) {
        *line = *line >> 22;
        *line = *line | 7;
        *line = *line << 22;
    }
    else {
        exit(1);
    }
    return;
}

void convertregA(char* field, int* line) {
    if (!isNumber(field)) {
        exit(1);
    }
    int num = convertdigits(field);
    if (num > 7 || num < 0) {
        exit(1);
    }
    *line = *line >> 19;
    *line = *line | num;
    *line = *line << 19;
    return;
}
void convertregB(char* field, int* line) {
    if (!isNumber(field)) {
        exit(1);
    }
    int num = convertdigits(field);
    if (num > 7 || num < 0) {
        exit(1);
    }
    *line = *line >> 16;
    *line = *line | num;
    *line = *line << 16;
    return;
}
void convertdest(char* field, int* line) {
    if (!isNumber(field)) {
        exit(1);
    }
    int num = convertdigits(field);
    if (num > 7 || num < 0) {
        exit(1);
    }
    *line = *line | num;
}
void convertIreg(char* field, char labels[MAXLINELENGTH][6], int* line, char* opcode, int PC) {
    if (isNumber(field)) {
        //        if (*field == '-') {
        //            int num = convertdigits(field);
        //            num = ~num + 1; //correct twos complement #
        //            num = num & 65535;
        //            *line = *line | num;
        //            return;
        //        }
        int num = convertdigits(field);
        if (!strcmp(opcode, "beq")) {
//            int newnum = num - PC - 1;
            int newnum = num;
            if (newnum < -32768 || newnum > 32767) {
                exit(1);
            }
            newnum = newnum & 65535;
            *line = *line | newnum;
            return;
        }
        if (num < -32768 || num > 32767) {
            exit(1);
        }
        num = num & 65535;
        *line = *line | num;
        return;
    }
    int check_address = -1;;
    for (int i = 0; i < MAXLINELENGTH; i++) {
        int test1 = 0;
        int test2 = 0;
        for (int k = 0; k < 6; k++) {
            if (labels[i][k] == '\0' && field[k] == '\0') {
                break;
            }
            if (labels[i][k] == '\0' && field[k] != '\0') {
                test1++;
            }
            if (labels[i][k] != '\0' && field[k] == '\0') {
                test1++;
            }
            if (labels[i][k] == field[k]) {
                test2++;
            }
            else {
                test1++;
                break;
            }
        }
        if (test2 > 0 && test1 == 0) {
            check_address = i;
            break;
        }
    }
    if (check_address == -1) {
        if (*field < 91 && strcmp(opcode, "beq")) {
            return;
            //have that we are not branching, so undefined global is fine
        }
        exit(1); // symbolic address is undefined and local, error
    }
    if (!strcmp(opcode, "beq")) {
        int newnum = check_address - PC - 1;
        if (newnum < -32768 || newnum > 32767) {
            exit(1);
        }
        newnum = newnum & 65535;
        *line = *line | newnum;
    }
    else {
        //lw or sw!
        int num = check_address;
        if (num < -32768 || num > 32767) {
            exit(1);
        }
        num = num & 65535;
        *line = *line | num;
        return;
    }
    //first check if we are using numeric or symbolic addressing
    //if numeric, same as usual
    //if symbolic, seach labels for label and then label_addresses to find corresponding address
    //then with that number, do same as usual
}
void convertfill(char* field , char labels[MAXLINELENGTH][6], int* line) {
    if (isNumber(field)) {
//        if (*field == '-') {
//            int num = convertdigits(field);
//            *line = *line | num;
//            return;
//        }
        int num = convertdigits(field);
        *line = *line | num;
        return;
    }
    int check_address = -1;
    for (int i = 0; i < MAXLINELENGTH; i++) {
        int test1 = 0;
        int test2 = 0;
        for (int k = 0; k < 6; k++) {
            if (labels[i][k] == '\0' && field[k] == '\0') {
                break;
            }
            if (labels[i][k] == '\0' && field[k] != '\0') {
                test1++;
            }
            if (labels[i][k] != '\0' && field[k] == '\0') {
                test1++;
            }
            if (labels[i][k] == field[k]) {
                test2++;
            }
            else {
                test1++;
                break;
            }
        }
        if (test2 > 0 && test1 == 0) {
            check_address = i;
            break;
        }
    }
    
    if (check_address == -1) {
        if (*field < 91) {
            return; // dealing with .fill, temporarily resolve to 0
        }
        exit(1);
    }
    *line = *line | check_address;
    return;
}

int convertdigits(char* number) {
    int isneg = 0;
    int digits = 0;
    int convert = 0;
    if (*number == '-') {
        isneg = 1;
    }
    if (isneg == 0) {
        while (*(number + digits) != '\0') {
            digits++;
        }
        for (int i = 0; i < digits; i++) {
            convert += (*(number + i) - '0') * pow(10, digits - i - 1);
        }
        return convert;
    }
    else {
        while (*(number + digits + 1) != '\0') {
            digits++;
        }
        for (int i = 0; i < digits; i++) {
            convert += (*(number + i + 1) - '0') * pow(10, digits - i - 1);
        }
        return convert * -1;
    }
}
