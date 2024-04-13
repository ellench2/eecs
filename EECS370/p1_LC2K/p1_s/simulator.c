/*
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 *
 * Make sure to NOT modify printState or any of the associated functions
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//DO NOT CHANGE THE FOLLOWING DEFINITIONS 

// Machine Definitions
#define NUMMEMORY 65536 /* maximum number of words in memory (maximum number of lines in a given file)*/
#define NUMREGS 8 /*total number of machine registers [0,7]*/

// File Definitions
#define MAXLINELENGTH 1000 /* MAXLINELENGTH is the max number of characters we read */

typedef struct 
stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
static inline int convertNum(int32_t);
int opextract(int);
void callop(int, stateType*);
void add(int, int, int, stateType*);
void nor(int, int, int, stateType*);
void lw(int, int, int, stateType*);
void sw(int, int, int, stateType*);
void beq(int, int, int, stateType*);
void jalr(int, int, stateType*);

int 
main(int argc, char **argv)
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s , please ensure you are providing the correct path", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; ++state.numMemory) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address  %d\n. Please ensure you are providing a machine code file.", state.numMemory);
            perror("sscanf");
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    //Your code starts here!
    int instructions = 0;
    state.pc = 0;
    for (int i = 0; i < NUMREGS; i++) {
        state.reg[i] = 0;
    }

    stateType* stateptr = &state;
    int curr_op = opextract(state.mem[state.pc]);
    while (curr_op != 6) { //while command is not halt
        //give to thing
        printState(stateptr);
        callop(curr_op, stateptr);
        curr_op = opextract(state.mem[state.pc]);
        instructions++;
    }

    //Your code ends here!
    printState(stateptr);
    instructions++;
    state.pc++;
    printf("machine halted\ntotal of %i instructions executed\nfinal state of machine:\n", instructions);
    printState(stateptr);
    return(0);
}

/*
* DO NOT MODIFY ANY OF THE CODE BELOW. 
*/

void 
printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++) 
              printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++) 
              printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    printf("end state\n");
}

// convert a 16-bit number into a 32-bit Linux integer
static inline int convertNum(int num) 
{
    return num - ( (num & (1<<15)) ? 1<<16 : 0 );
}

/*
* Write any helper functions that you wish down here. 
*/

int opextract(int mc) {
    int op = (mc >> 22) & 7;
    return op;
}

void callop(int op, stateType* stateptr) {
    int mc = stateptr->mem[stateptr->pc];
    switch (op) {
        case 0: { //add
            int regA = (mc >> 19) & 7;
            int regB = (mc >> 16) & 7;
            int destReg = mc & 7;
            add(regA, regB, destReg, stateptr);
            break;
        }
        case 1: { //nor
            int regA = (mc >> 19) & 7;
            int regB = (mc >> 16) & 7;
            int destReg = mc & 7;
            nor(regA, regB, destReg, stateptr);
            break;
        }
        case 2: { //lw
            int regA = (mc >> 19) & 7;
            int regB = (mc >> 16) & 7;
            int add = mc & 65535;
            add = convertNum(add);
            lw(regA, regB, add, stateptr);
            break;
        }
        case 3: { //sw
            int regA = (mc >> 19) & 7;
            int regB = (mc >> 16) & 7;
            int add = mc & 65535;
            add = convertNum(add);
            sw(regA, regB, add, stateptr);
            break;
        }
        case 4: { //beq
            int regA = (mc >> 19) & 7;
            int regB = (mc >> 16) & 7;
            int add = mc & 65535;
            add = convertNum(add);
            beq(regA, regB, add, stateptr);
            break;
        }
        case 5: { //jalr
            int regA = (mc >> 19) & 7;
            int regB = (mc >> 16) & 7;
            jalr(regA, regB, stateptr);
            break;
        }
        case 7: { //noop
            stateptr->pc++;
            break;
        }
    }
    return;
}

void add(int regA, int regB, int destReg, stateType* stateptr) {
    stateptr->reg[destReg] = stateptr->reg[regA] + stateptr->reg[regB];
    stateptr->pc++;
    return;
}

void nor(int regA, int regB, int destReg, stateType* stateptr) {
    stateptr->reg[destReg] = ~(stateptr->reg[regA] | stateptr->reg[regB]);
    stateptr->pc++;
    return;
}

void lw(int regA, int regB, int add, stateType* stateptr) {
    stateptr->reg[regB] = stateptr->mem[stateptr->reg[regA] + add];
    stateptr->pc++;
    return;
}

void sw(int regA, int regB, int add, stateType* stateptr) {
    stateptr->mem[stateptr->reg[regA] + add] = stateptr->reg[regB];
    stateptr->pc++;
    return;
}

void beq(int regA, int regB, int add, stateType* stateptr) {
    if (stateptr->reg[regA] == stateptr->reg[regB]) {
        stateptr->pc = stateptr->pc + 1 + add;
    }
    else {
        stateptr->pc++;
    }
    return;
}

void jalr(int regA, int regB, stateType* stateptr) {
    stateptr->reg[regB] = stateptr->pc + 1;
    stateptr->pc = stateptr->reg[regA];
    return;
}


