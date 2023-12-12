#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 
#include "Global_Variables.h"
#include "string.h"

int LoadInstructions(char filePath[])
{
    
    FILE* datFileRead;
    datFileRead = fopen(filePath, "r");
    char currentLine[8] = "";
    void (*instr_func_array[])() = {};


    if (datFileRead == NULL) { // If file cannot be read, print error
        printf("Error opening file.\n");
        return 0;
    }

    while ((fgets(currentLine, 50, datFileRead)) != NULL) { // While there is a string to read, print the output
        //  GET FULL LINE
        char fullLine[32] = "";

        for(int i = 0; i < 8; i++){fullLine[24+i] = currentLine[i];}
        
        fgets(currentLine, 50, datFileRead);
        for(int i = 0; i < 8; i++){fullLine[16+i] = currentLine[i];}
        
        fgets(currentLine, 50, datFileRead);
        for(int i = 0; i < 8; i++){fullLine[8+i] = currentLine[i];}
        
        fgets(currentLine, 50, datFileRead);
        for(int i = 0; i < 8; i++){fullLine[i] = currentLine[i];}

        // SELECT OPCODE

        char opcode[8] = "";
        for(int i = 0; i < 7; i++){opcode[i] = fullLine[25+i];}
        opcode[7] = '\0';
        printf("%s", opcode);

        // INSTRUCTION SEARCH FROM OPCODE
        if(strcmp(opcode, "0110111") == 0)
        {
            // LUI
            printf("LUI");
            
        } else if(strcmp(opcode, "0010111") == 0)
        {
            // AUIPC
            printf("AUIPC");

        } else if(strcmp(opcode, "1101111") == 0)
        {
            // JAL
            printf("JAL");

        } else if(strcmp(opcode, "1100111") == 0)
        {
            // JALR
            printf("JALR");

        } else if(strcmp(opcode, "1100011") == 0)
        {
            // BEQ, BNE, BLT, BGE, BLTU, BGEU
            // Check through func code
            printf("Branch functions");
        } else if(strcmp(opcode, "0000011") == 0)
        {
            // LB, LH, LW, LBU, LHU
            // Check through func code
            printf("Load functions");
        } else if(strcmp(opcode, "0100011") == 0)
        {
            // SB, SH, SW
            // Check through func code
            printf("Store functions");
        } else if(strcmp(opcode, "0010011") == 0)
        {
            // ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI
            // Check through func code
            printf("Immediate functions");
        } else if(strcmp(opcode, "0110011") == 0)
        {
            // ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND
            // Check through func code
            printf("Register functions");
        } 
    }

    fclose(datFileRead);
    return 0;
}