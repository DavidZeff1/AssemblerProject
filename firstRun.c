#include "firstRun.h"

void printInBinary(unsigned int, FILE*); // Converts binary commands into base 64 and prints to output file
int extractIntFromString(char*);
void convertToBinary(int, int, int, char*, char*, int, FILE*, FILE*); // Converts commands to bin, then send to 'printtobin' which will convert to 64 and print 

int ERROR = 0;
int sizeOfStr(char*);
int checkIfOperation(char*);

void handleStringProcessing(char *str, int *countLine, int *numOfInstructions, int *strLen, FILE *fp);
void handleDataProcessing(char *str, int *countLine, int *numOfInstructions, FILE *fp);
void handleVariableProcessing(char *str, int countLine, varnode *head, FILE *fe, instruction *currentInstruction);

void firstRun(int argc, char** argv) {
    FILE *ft, *fp, *fe, *fc = NULL, *fk = NULL;
    char str[maxLineLength] = {0};
    char c;
    int strLen = 0, countLine = 100, numOfInstructions = 0, numOfData = 0;
    instruction *currentInstruction = (instruction*)malloc(sizeof(instruction));
    varnode *head = NULL;

    if (!currentInstruction) {
        printf("Error allocating memory on line 31: firstRun.c\n");
        return;
    }
    currentInstruction->operation[0] = '\0';

    // Open files
    ft = fopen("Prog.as", "r");
    fp = fopen("Prog.ob", "w+");
    fe = fopen("errorFile.txt", "w+");
    if (!ft || !fp) {
        printf("Error opening the file on line 33 or 34: firstRun.c\n");
        return;
    }

    // First pass: Identify and process labels, data, strings, externs, and entries
    processFirstPass(ft, &strLen, str, &countLine, &numOfInstructions, &numOfData, &head, &fc, &fk);

    // Second pass: Apply addresses and types to variables
    processSecondPass(ft, &strLen, str, &countLine, head, fk, fc);

    // Third pass: Convert instructions to binary and then to base64
    processThirdPass(ft, &strLen, str, &countLine, fp, fe, head);

    // Write final counts to .ob file
    fprintf(fp, "%d,%d\n", numOfData, numOfInstructions);

    // Close files and free allocated memory
    fclose(ft);
    fclose(fp);
    fclose(fe);
    if (fc) fclose(fc);
    if (fk) fclose(fk);
    free(currentInstruction);
}

void processFirstPass(FILE *ft, int *strLen, char *str, int *countLine, int *numOfInstructions, int *numOfData, varnode **head, FILE **fc, FILE **fk) {
    char c;
    int foundRegister = 0, foundData = 0, foundString = 0, foundExtern = 0, foundEntry = 0;

    while (!feof(ft)) {
        c = fgetc(ft);
        if (c != ' ' && c != ',' && c != '\n') {
            str[(*strLen)++] = c;
        } else if (*strLen > 0) {
            processString(ft, str, strLen, countLine, numOfInstructions, numOfData, head, &foundRegister, &foundData, &foundString, &foundExtern, &foundEntry, fc, fk);
            RESET_STRING
        }
    }
    // Reset file pointer for next pass
    rewind(ft);
}

void processSecondPass(FILE *ft, int *strLen, char *str, int *countLine, varnode *head, FILE *fk, FILE *fc) {
    char c;

    while (!feof(ft)) {
        c = fgetc(ft);
        if (c != ' ' && c != ',' && c != '\n') {
            str[(*strLen)++] = c;
        } else if (*strLen > 0) {
            processSecondPassString(str, strLen, countLine, head, fk, fc);
            RESET_STRING
        }
    }
    // Reset file pointer for next pass
    rewind(ft);
}

void processThirdPass(FILE *ft, int *strLen, char *str, int *countLine, FILE *fp, FILE *fe, varnode *head) {
    char c;
    int amountOfOps = 0, foundRegister = 0, foundData = 0, foundString = 0, foundExtern = 0, foundEntry = 0, existsOperation = 0;
    instruction *currentInstruction = (instruction*)malloc(sizeof(instruction));

    if (!currentInstruction) {
        printf("Error allocating memory on line 31: firstRun.c\n");
        return;
    }
    currentInstruction->operation[0] = '\0';

    while (!feof(ft)) {
        c = fgetc(ft);
        if (c != ' ' && c != ',' && c != '\n') {
            str[(*strLen)++] = c;
        } else if (*strLen > 0) {
            processThirdPassString(str, strLen, countLine, &amountOfOps, foundRegister, foundData, foundString, foundExtern, foundEntry, existsOperation, currentInstruction, fp, fe, head);
            RESET_STRING
        }
    }

    free(currentInstruction);
}

void processString(FILE *ft, char *str, int *strLen, int *countLine, int *numOfInstructions, int *numOfData, varnode **head, int *foundRegister, int *foundData, int *foundString, int *foundExtern, int *foundEntry, FILE **fc, FILE **fk) {
    if (str[*strLen - 1] == ':') {
        str[*strLen - 1] = '\0';
        *head = addVarNode(*head, str, *countLine, 0, 0);
    } else {
        // Process based on current flags
        // ... (remaining logic here)
    }
    // Handle end of line
    if (*c == '\n') {
        resetFlags(foundRegister, foundData, foundString, foundExtern, foundEntry);
        RESET_STRING
    }
}

// Additional refactored functions...



/* Typedefs for bit fields */
typedef struct {
    unsigned int are : 2;
    unsigned int dest : 3;
    unsigned int op : 4;
    unsigned int source : 3;
} BitField;

typedef struct {
    unsigned int are : 2;
    unsigned int all : 10;
} BitField2;

typedef struct {
    unsigned int are : 2;
    unsigned int dest : 5;
    unsigned int source : 5;
} BitField3;

void printInBinary(unsigned int num, FILE* fp);

/* Main conversion function */
void convertToBinary(int sourceNum, int destinationNum, int operationNum, char* sourceType, char* destinationType, int numOfOperands, FILE* fp, FILE* fe) {
    unsigned int combinedValue = 0;

    /* Dynamic allocation for bit field structures */
    BitField *command = malloc(sizeof(BitField));
    BitField2 *direct = malloc(sizeof(BitField2));
    BitField3 *reg = malloc(sizeof(BitField3));

    /* Check for specific operations with operands */
    if ((operationNum == 14 || operationNum == 15) && numOfOperands > 0) {
        fprintf(fe, "Error0: Invalid operation with stop or rts\n");
        ERROR = 1;
    }

    if (sourceType && destinationType) {
        /* Check for different types of source and destination */
        if (!strcmp(sourceType, "register") && !strcmp(destinationType, "register")) {
            if (operationNum != 0 && operationNum != 1 && operationNum != 2 && operationNum != 3) {
                fprintf(fe, "Error1: Invalid operation with 2 registers\n");
                ERROR = 1;
            }

            command->dest = 5;
            command->source = 5;
            COMMAND_PRINT;
            reg->are = 0;
            reg->dest = destinationNum;
            reg->source = sourceNum;
            combinedValue |= reg->are;
            combinedValue |= reg->dest << 2;
            combinedValue |= reg->source << 7;
            printInBinary(combinedValue, fp);

            free(command);
            free(reg);
            free(direct);
            return;
        }

        /* Add similar checks for other combinations of sourceType and destinationType */

    }

    /* Add similar checks for only SourceType */
}

void printInBinary(unsigned int num, FILE* fp) {
    const char base64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char str[7] = {0};  // 6 bits + null terminator

    // Convert number to binary string
    for (int i = 0; i < 6; ++i) {
        str[5 - i] = (num & (1 << i)) ? '1' : '0';
    }

    // Map binary string to base64
    for (int j = 0; j < 64; ++j) {
        if (strncmp(str, base64Table2[j], 6) == 0) {
            fprintf(fp, "%c", base64Table[j]);
            break;
        }
    }
    fprintf(fp, "\n");
}




		
	







	


