/*
 * EECS 370, University of Michigan
 * Project 3: LC-2K Pipeline Simulator
 * Instructions are found in the project spec.
 * Make sure NOT to modify printState or any of the associated functions
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Machine Definitions
#define NUMMEMORY 65536 // maximum number of data words in memory
#define NUMREGS 8 // number of machine registers

#define ADD 0
#define NOR 1
#define LW 2
#define SW 3
#define BEQ 4
#define JALR 5 // will not implemented for Project 3
#define HALT 6
#define NOOP 7

const char* opcode_to_str_map[] = {
    "add",
    "nor",
    "lw",
    "sw",
    "beq",
    "jalr",
    "halt",
    "noop"
};

#define NOOPINSTRUCTION (NOOP << 22)

typedef struct IFIDStruct {
	int instr;
	int pcPlus1;
} IFIDType;

typedef struct IDEXStruct {
	int instr;
	int pcPlus1;
	int readRegA;
	int readRegB;
	int offset;
} IDEXType;

typedef struct EXMEMStruct {
	int instr;
	int branchTarget;
    int eq;
	int aluResult;
	int readRegB;
} EXMEMType;

typedef struct MEMWBStruct {
	int instr;
	int writeData;
    int aluResult;
} MEMWBType;

typedef struct WBENDStruct {
	int instr;
	int writeData;
    int aluResult;
} WBENDType;

typedef struct stateStruct {
	int pc;
	int instrMem[NUMMEMORY];
	int dataMem[NUMMEMORY];
    int reg[NUMREGS];
	int numMemory;
	IFIDType IFID;
	IDEXType IDEX;
	EXMEMType EXMEM;
	MEMWBType MEMWB;
	WBENDType WBEND;
	int cycles; // number of cycles run so far
} stateType;

static inline int opcode(int instruction) {
    return instruction>>22;
}

static inline int field0(int instruction) {
    return (instruction>>19) & 0x7;
}

static inline int field1(int instruction) {
    return (instruction>>16) & 0x7;
}

static inline int field2(int instruction) {
    return instruction & 0xFFFF;
}

// convert a 16-bit number into a 32-bit Linux integer
static inline int convertNum(int num) {
    return num - ( (num & (1<<15)) ? 1<<16 : 0 );
}

void printState(stateType*);
void printInstruction(int);
void readMachineCode(stateType*, char*);

int main(int argc, char *argv[]) {
    stateType state, newState;
    memset(state.reg, 0, sizeof(state.reg)); //REMOVE THIS LINE LATER MAYBE
    state.IFID.instr = NOOPINSTRUCTION;
    state.IDEX.instr = NOOPINSTRUCTION;
    state.EXMEM.instr = NOOPINSTRUCTION;
    state.MEMWB.instr = NOOPINSTRUCTION;
    state.WBEND.instr = NOOPINSTRUCTION;
    state.cycles = 0;
    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    readMachineCode(&state, argv[1]);

    while (opcode(state.MEMWB.instr) != HALT) {
        printState(&state);

        newState = state;
        newState.cycles++;
        int branched = 0;
        int lwimm = 0;

        //if BEQ just left EX, check if .eq is true, if so squash IF, ID, and EX (replace with noops)
        if (opcode(state.EXMEM.instr) == 4 && state.EXMEM.eq) {
            branched = 1;
            newState.pc = state.EXMEM.branchTarget;
            newState.IFID.instr = NOOPINSTRUCTION;
            newState.IDEX.instr = NOOPINSTRUCTION;
            newState.EXMEM.instr = NOOPINSTRUCTION;
        }
        //if instsruction in ID depends on lw in EX, insert noop in EX, hold back IF and ID, let everyone else
        //move on
        if ((field0(state.IFID.instr) == field1(state.IDEX.instr) ||
             (field1(state.IFID.instr) == field1(state.IDEX.instr) &&
             opcode(state.IFID.instr) != 2)) &&
            opcode(state.IDEX.instr) == 2) {
            lwimm = 1;
            newState.IDEX.instr = NOOPINSTRUCTION;
            
        }
        int realRegA = state.IDEX.readRegA;
        int fixedA = 0;
        int realRegB = state.IDEX.readRegB;
        int fixedB = 0;
        //if instruction in EX depends on instruction in EXMEM, MEMWB, or WBEND, just get the right values from
        //those pipeline registers
        
        if (opcode(state.EXMEM.instr) == 0 || opcode(state.EXMEM.instr) == 1) {
            if (field0(state.IDEX.instr) == field2(state.EXMEM.instr)) {
                realRegA = state.EXMEM.aluResult;
                fixedA = 1;
            }
            if (field1(state.IDEX.instr) == field2(state.EXMEM.instr)) {
                realRegB = state.EXMEM.aluResult;
                fixedB = 1;
            }
        }
        if (opcode(state.MEMWB.instr) == 0 || opcode(state.MEMWB.instr) == 1) {
            if (field0(state.IDEX.instr) == field2(state.MEMWB.instr) && fixedA == 0) {
                realRegA = state.MEMWB.aluResult;
                fixedA = 1;
            }
            if (field1(state.IDEX.instr) == field2(state.MEMWB.instr) && fixedB == 0) {
                realRegB = state.MEMWB.aluResult;
                fixedB = 1;
            }
        }
        if (opcode(state.MEMWB.instr) == 2) {
            if (field0(state.IDEX.instr) == field1(state.MEMWB.instr) && fixedA == 0) {
                realRegA = state.MEMWB.writeData;
                fixedA = 1;
            }
            if (field1(state.IDEX.instr) == field1(state.MEMWB.instr) && fixedB == 0) {
                realRegB = state.MEMWB.writeData;
                fixedB = 1;
            }
        }
        if (opcode(state.WBEND.instr) == 0 || opcode(state.WBEND.instr) == 1) {
            if (field0(state.IDEX.instr) == field2(state.WBEND.instr) && fixedA == 0) {
                realRegA = state.WBEND.writeData;
                fixedA = 1;
            }
            if (field1(state.IDEX.instr) == field2(state.WBEND.instr) && fixedB == 0) {
                realRegB = state.WBEND.writeData;
                fixedB = 1;
            }
        }
        if (opcode(state.WBEND.instr) == 2) {
            if (field0(state.IDEX.instr) == field1(state.WBEND.instr) && fixedA == 0) {
                realRegA = state.WBEND.writeData;
                fixedA = 1;
            }
            if (field1(state.IDEX.instr) == field1(state.WBEND.instr) && fixedB == 0) {
                realRegB = state.WBEND.writeData;
                fixedB = 1;
            }
        }

        //NOT EVERYTHING BELOW IS SUPPOSED TO HAPPEN, ADD CONDITIONALS TO ACCOUNT FOR ABOVE CODE
        //
        
        /* ---------------------- IF stage --------------------- */
        if (!branched && !lwimm) {
            if (state.pc < state.numMemory) {
                newState.IFID.instr = state.instrMem[state.pc];
            }
            else {
                newState.IFID.instr = 0;
            }
            newState.pc = state.pc + 1;
            newState.IFID.pcPlus1 = state.pc + 1;
        }

        /* ---------------------- ID stage --------------------- */
        if (!branched && !lwimm) {
            if (state.IFID.instr != NOOPINSTRUCTION) {
                newState.IDEX.readRegA = state.reg[field0(state.IFID.instr)];
                newState.IDEX.readRegB = state.reg[field1(state.IFID.instr)];
                newState.IDEX.offset = convertNum(field2(state.IFID.instr));
            }
            newState.IDEX.instr = state.IFID.instr;
            newState.IDEX.pcPlus1 = state.IFID.pcPlus1;
        }
        /* ---------------------- EX stage --------------------- */
        if (!branched) {
            if (state.IDEX.instr != NOOPINSTRUCTION) {
                newState.EXMEM.branchTarget = state.IDEX.pcPlus1 + state.IDEX.offset;
                newState.EXMEM.eq = (realRegA == realRegB) + 0;
                if (opcode(state.IDEX.instr) == 0) { //add or nor?
                    newState.EXMEM.aluResult = realRegA + realRegB;
                }
                if (opcode(state.IDEX.instr) == 1) {
                    newState.EXMEM.aluResult = ~(realRegA | realRegB);
                }
                if (opcode(state.IDEX.instr) == 2 || opcode(state.IDEX.instr) == 3) { //lw or sw, else
                    newState.EXMEM.aluResult = realRegA + state.IDEX.offset;
                }
                newState.EXMEM.readRegB = realRegB;
            }
            newState.EXMEM.instr = state.IDEX.instr;
        }

        /* --------------------- MEM stage --------------------- */
        if (state.EXMEM.instr != NOOPINSTRUCTION) {
            newState.MEMWB.aluResult = state.EXMEM.aluResult;
            if (opcode(state.EXMEM.instr) == 0 || opcode(state.EXMEM.instr) == 1) {
                newState.MEMWB.writeData = state.EXMEM.aluResult;
            }
            if (opcode(state.EXMEM.instr) == 2) {
                newState.MEMWB.writeData = state.dataMem[state.EXMEM.aluResult];
            }
            if (opcode(state.EXMEM.instr) == 3) {
                newState.dataMem[state.EXMEM.aluResult] = state.EXMEM.readRegB;
            }
        }
        newState.MEMWB.instr = state.EXMEM.instr;
        
        /* ---------------------- WB stage --------------------- */
        if (opcode(state.MEMWB.instr) == 0 || opcode(state.MEMWB.instr) == 1) {
            newState.reg[field2(state.MEMWB.instr)] = state.MEMWB.aluResult;
        }
        if (opcode(state.MEMWB.instr) == 2) {
            newState.reg[field1(state.MEMWB.instr)] = state.MEMWB.writeData;
        }
        newState.WBEND.instr = state.MEMWB.instr;
        newState.WBEND.writeData = state.MEMWB.writeData;
        /* ------------------------ END ------------------------ */
        state = newState; /* this is the last statement before end of the loop. It marks the end
        of the cycle and updates the current state with the values calculated in this cycle */
    }
    printf("machine halted\n");
    printf("total of %d cycles executed\n", state.cycles);
    printf("final state of machine:\n");
    printState(&state);
}

/*
* DO NOT MODIFY ANY OF THE CODE BELOW.
*/

void printInstruction(int instr) {
    char instr_opcode_str[10];
    int instr_opcode = opcode(instr);
    if(0 <= instr_opcode && instr_opcode<= NOOP) {
        strcpy(instr_opcode_str, opcode_to_str_map[instr_opcode]);
    }

    switch (instr_opcode) {
        case ADD:
        case NOR:
        case LW:
        case SW:
        case BEQ:
            printf("%s %d %d %d", instr_opcode_str, field0(instr), field1(instr), convertNum(field2(instr)));
            break;
        case JALR:
            printf("%s %d %d", instr_opcode_str, field0(instr), field1(instr));
            break;
        case HALT:
        case NOOP:
            printf("%s", instr_opcode_str);
            break;
        default:
            printf(".fill %d", instr);
            return;
    }
}

void printState(stateType *statePtr) {
    printf("\n@@@\n");
    printf("state before cycle %d starts:\n", statePtr->cycles);
    printf("\tpc = %d\n", statePtr->pc);

    printf("\tdata memory:\n");
    for (int i=0; i<statePtr->numMemory; ++i) {
        printf("\t\tdataMem[ %d ] = %d\n", i, statePtr->dataMem[i]);
    }
    printf("\tregisters:\n");
    for (int i=0; i<NUMREGS; ++i) {
        printf("\t\treg[ %d ] = %d\n", i, statePtr->reg[i]);
    }

    // IF/ID
    printf("\tIF/ID pipeline register:\n");
    printf("\t\tinstruction = %d ( ", statePtr->IFID.instr);
    printInstruction(statePtr->IFID.instr);
    printf(" )\n");
    printf("\t\tpcPlus1 = %d", statePtr->IFID.pcPlus1);
    if(opcode(statePtr->IFID.instr) == NOOP){
        printf(" (Don't Care)");
    }
    printf("\n");

    // ID/EX
    int idexOp = opcode(statePtr->IDEX.instr);
    printf("\tID/EX pipeline register:\n");
    printf("\t\tinstruction = %d ( ", statePtr->IDEX.instr);
    printInstruction(statePtr->IDEX.instr);
    printf(" )\n");
    printf("\t\tpcPlus1 = %d", statePtr->IDEX.pcPlus1);
    if(idexOp == NOOP){
        printf(" (Don't Care)");
    }
    printf("\n");
    printf("\t\treadRegA = %d", statePtr->IDEX.readRegA);
    if (idexOp >= HALT || idexOp < 0) {
        printf(" (Don't Care)");
    }
    printf("\n");
    printf("\t\treadRegB = %d", statePtr->IDEX.readRegB);
    if(idexOp == LW || idexOp > BEQ || idexOp < 0) {
        printf(" (Don't Care)");
    }
    printf("\n");
    printf("\t\toffset = %d", statePtr->IDEX.offset);
    if (idexOp != LW && idexOp != SW && idexOp != BEQ) {
        printf(" (Don't Care)");
    }
    printf("\n");

    // EX/MEM
    int exmemOp = opcode(statePtr->EXMEM.instr);
    printf("\tEX/MEM pipeline register:\n");
    printf("\t\tinstruction = %d ( ", statePtr->EXMEM.instr);
    printInstruction(statePtr->EXMEM.instr);
    printf(" )\n");
    printf("\t\tbranchTarget %d", statePtr->EXMEM.branchTarget);
    if (exmemOp != BEQ) {
        printf(" (Don't Care)");
    }
    printf("\n");
    printf("\t\teq ? %s", (statePtr->EXMEM.eq ? "True" : "False"));
    if (exmemOp != BEQ) {
        printf(" (Don't Care)");
    }
    printf("\n");
    printf("\t\taluResult = %d", statePtr->EXMEM.aluResult);
    if (exmemOp > SW || exmemOp < 0) {
        printf(" (Don't Care)");
    }
    printf("\n");
    printf("\t\treadRegB = %d", statePtr->EXMEM.readRegB);
    if (exmemOp != SW) {
        printf(" (Don't Care)");
    }
    printf("\n");

    // MEM/WB
	int memwbOp = opcode(statePtr->MEMWB.instr);
    printf("\tMEM/WB pipeline register:\n");
    printf("\t\tinstruction = %d ( ", statePtr->MEMWB.instr);
    printInstruction(statePtr->MEMWB.instr);
    printf(" )\n");
    printf("\t\twriteData = %d", statePtr->MEMWB.writeData);
    if (memwbOp >= SW || memwbOp < 0) {
        printf(" (Don't Care)");
    }
    printf("\n");

    // WB/END
	int wbendOp = opcode(statePtr->WBEND.instr);
    printf("\tWB/END pipeline register:\n");
    printf("\t\tinstruction = %d ( ", statePtr->WBEND.instr);
    printInstruction(statePtr->WBEND.instr);
    printf(" )\n");
    printf("\t\twriteData = %d", statePtr->WBEND.writeData);
    if (wbendOp >= SW || wbendOp < 0) {
        printf(" (Don't Care)");
    }
    printf("\n");

    printf("end state\n");
    fflush(stdout);
}

// File
#define MAXLINELENGTH 1000 // MAXLINELENGTH is the max number of characters we read

void readMachineCode(stateType *state, char* filename) {
    char line[MAXLINELENGTH];
    FILE *filePtr = fopen(filename, "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", filename);
        exit(1);
    }

    printf("instruction memory:\n");
    for (state->numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; ++state->numMemory) {
        if (sscanf(line, "%d", state->instrMem+state->numMemory) != 1) {
            printf("error in reading address %d\n", state->numMemory);
            exit(1);
        }
        printf("\tinstrMem[ %d ] = ", state->numMemory);
        printInstruction(state->dataMem[state->numMemory] = state->instrMem[state->numMemory]);
        printf("\n");
    }
}
