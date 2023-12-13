#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <math.h>
#include <time.h>
#include "ISA_Instructions"
#include "Global_Variables.h"
#include "string.h"

/*
    Misc Functions
*/
clock_t start, end;

void startTimer() {
    start = clock();
}

void stopTimer() {
    end = clock();
    double ElapsedTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Execution Time: %f seconds\n", ElapsedTime);
 }

void newLine(){
    printf("\n");
}

int compareChar(char a, char b) {
    if (a == b)
        return 0;
    else
        return -1;
}

int standBinaryCov(char bin[])
{
    int n = 0;
    for(int i = 0; i < strlen(bin); i++)
    {
        if(bin[i] == '1')
        {
            n += pow(2,(strlen(bin)-i-1));
        }
    }
    return n;
}

int twosBinaryConv(char bin[])
{
    bool negative = false;
    char twos[32] = "";
    strcpy(twos, bin);
    if(bin[0] == '1')
    {
        //ones
        for(int i = 0; i < strlen(bin); i++)
        {
            negative = true;
            if(twos[i] == '0')
            {
                twos[i] = '1';
            } else if(twos[i] == '1')
            {
                twos[i] = '0';
            }
        }
        //twos
        int c = 1;
        for(int i = strlen(bin) - 1; i >= 0; i--)
        {
            if(twos[i] == '1' && c == 1)
            {
                twos[i] = '0';
            } else if(twos[i] == '0' && c == 1)
            {
                twos[i] = '1';
                c = 0;
            }
        }
        // overload
        if(c == 1){return 0;}
    }

    //BIN TO INT
    int n = 0;
    for(int i = 0; i < strlen(twos); i++)
    {
        if(twos[i] == '1')
        {
            n += pow(2,(strlen(twos)-i-1));
        }
    }

    //CHECK IF NEGATIVE
    if(negative)
    {
        return -1 * n;
    } else
    {
        return n;
    }
}

void printRegisters()
{
    for(int i=0; i < 32; i++)
    {
        printf("x%d: %d\n", i,Registers[i]);
    }
}

/*
    Main
*/
int main(){
    /*
        Workspace Testing
    */
    ADDI(5, 3, 2);
    ADDI(1, 5, 5);
    printf("%d ", Registers[5]);
    printf("%d ", Registers[1]);
    printf("%d ", Registers[4]);
    char myString[] = "joe";
    printf("%c ", myString[2]);
    printf("%d ", twosBinaryConv("10010001001010110111"));

    /*
        Reading File + Initializing
    */
    printf("\n");
    ProgramCounter = 1000;

        // Open file in reading mode
    FILE* datFileRead;
    char FileName[100] = "Test_Files/line.dat";

    datFileRead = fopen(FileName, "r");
        // fgetc    return each character in a sequence
        // fgets    return each string in a sequence
        // EOF      end of file

    if (datFileRead == NULL) { // If file cannot be read, print error
        printf("Error opening file.\n");
        return 0;
    }

        // Open asm in writing mode
    FILE* asmFileWrite;
    char FileName2[100] = "Test_Files/assembly.asm";
    asmFileWrite = fopen(FileName2, "w");



    /*
        File Loading / Parsing
    */
    char currentLine[8] = "";
    int asmPC = 0;
    char asmPrint[50][50];

    //function array
    //void (*instr_func_array[100])() = {};
    //int (*parameters[100])[3]={};
    //int ProgramPointer = 0;

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

        // SELECT RD / IMMEDIATE
        char rd[6] = "";
        for(int i = 0; i < 5; i++){rd[i] = fullLine[20+i];}
        rd[5] = '\0';

        // SELECT POSSIBLE FUNC CODE

        char func[4] = "";
        for(int i = 0; i < 3; i++){func[i] = fullLine[17+i];}
        func[3] = '\0';

        /*
            INSTRUCTION SEARCH - This can be made much more efficient if time permits
        */
        if(strcmp(opcode, "0110111") == 0)
        {
            // LUI(GI)
            char immC[21] = "";
            for(int i = 0; i < 20; i++){immC[i] = fullLine[i];}
            int imm = twosBinaryConv(immC);
            int ird = standBinaryCov(rd);
            
            char statement[100] = "lui ";
            char ImmChar[20] = "";
            sprintf(ImmChar, "%d", imm);
            char RDChar[20] = "";
            sprintf(RDChar, "t%d", ird);
            strncat(statement, RDChar, 20);
            strncat(statement, ", ", 3);
            strncat(statement, ImmChar, 20);
            strcpy(asmPrint[asmPC], statement);
            asmPC += 1;
            //instr_func_array[ProgramPointer] = LUI;
            //int param[3] = {ird, imm, NULL};
            //parameters[ProgramPointer] = param;
            //ProgramPointer += 1;
            LUI(ird,imm);
            
            
        } else if(strcmp(opcode, "0010111") == 0)
        {
            // AUIPC
            char immC[21] = "";
            for(int i = 0; i < 20; i++){immC[i] = fullLine[i];}
            int imm = twosBinaryConv(immC);
            int ird = standBinaryCov(rd);

            char statement[100] = "AUIPC ";
            char ImmChar[20] = "";
            sprintf(ImmChar, "%d", imm);
            char RDChar[20] = "";
            sprintf(RDChar, "t%d", ird);
            strncat(statement, RDChar, 20);
            strncat(statement, ", ", 3);
            strncat(statement, ImmChar, 20);
            strcpy(asmPrint[asmPC], statement);
            asmPC += 1;
            AUIPC(ird,imm);

        } else if(strcmp(opcode, "1101111") == 0)
        {
            // JAL
            char immC[21] = "";
            immC[0] = fullLine[0];
            for(int i = 0; i < 8; i++){immC[i+1] = fullLine[i+12];}
            immC[9] = fullLine[11];
            for(int i = 0; i < 10; i++){immC[i+10] = fullLine[i+1];}
            int imm = twosBinaryConv(immC);
            int ird = standBinaryCov(rd);

            char statement[100] = "jal ";
            char ImmChar[20] = "";
            sprintf(ImmChar, "%d", imm);
            char RDChar[20] = "";
            sprintf(RDChar, "t%d", ird);
            strncat(statement, RDChar, 20);
            strncat(statement, ", ", 3);
            strncat(statement, ImmChar, 20);
            strcpy(asmPrint[asmPC], statement);
            asmPC += 1;
            JAL(ird,imm);

        } else if(strcmp(opcode, "1100111") == 0)
        {
            // JALR
            char rs1C[6] = "";
            for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
            char immC[13] = "";
            for(int i = 0; i < 12; i++){immC[i] = fullLine[i];}
            int imm = twosBinaryConv(immC);
            int rs1 = standBinaryCov(rs1C);
            int ird = standBinaryCov(rd);

            char statement[100] = "jalr ";
            char ImmChar[20] = "";
            sprintf(ImmChar, "%d", imm);
            char RDChar[20] = "";
            sprintf(RDChar, "t%d", ird);
            char RSChar[20] = "";
            sprintf(RSChar, "t%d", rs1);
            strncat(statement, RDChar, 20);
            strncat(statement, ", ", 3);
            strncat(statement, RSChar, 20);
            strncat(statement, ", ", 3);
            strncat(statement, ImmChar, 20);
            strcpy(asmPrint[asmPC], statement);
            asmPC += 1;
            JALR(ird,rs1,imm);

        } else if(strcmp(opcode, "1100011") == 0)
        {
            // Branch Functions
            if(strcmp(func, "000") == 0)
            {
                //BEQ
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char rs2C[6] = "";
                for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                char immC[14] = "";
                immC[0] = fullLine[0];
                immC[1] = fullLine[24];
                for(int i = 0; i < 6; i++){immC[i+2] = fullLine[i+1];}
                for(int i = 0; i < 4; i++){immC[i+8] = fullLine[20+i];}
                immC[13] = '0';
                int imm = twosBinaryConv(immC);
                int rs2 = standBinaryCov(rs2C);
                int rs1 = standBinaryCov(rs1C);

                char statement[100] = "beq ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RS2Char[20] = "";
                sprintf(RS2Char, "t%d", rs2);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RS2Char, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                BEQ(rs1,rs2,imm);
            } else if(strcmp(func, "001") == 0)
            {
                //BNE
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char rs2C[6] = "";
                for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                char immC[14] = "";
                immC[0] = fullLine[0];
                immC[1] = fullLine[24];
                for(int i = 0; i < 6; i++){immC[i+2] = fullLine[i+1];}
                for(int i = 0; i < 4; i++){immC[i+8] = fullLine[20+i];}
                immC[13] = '0';
                int imm = twosBinaryConv(immC);
                int rs2 = standBinaryCov(rs2C);
                int rs1 = standBinaryCov(rs1C);

                char statement[100] = "bne ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RS2Char[20] = "";
                sprintf(RS2Char, "t%d", rs2);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RS2Char, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                BNE(rs1,rs2,imm);
            } else if(strcmp(func, "100") == 0)
            {
                //BLT
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char rs2C[6] = "";
                for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                char immC[14] = "";
                immC[0] = fullLine[0];
                immC[1] = fullLine[24];
                for(int i = 0; i < 6; i++){immC[i+2] = fullLine[i+1];}
                for(int i = 0; i < 4; i++){immC[i+8] = fullLine[20+i];}
                immC[13] = '0';
                int imm = twosBinaryConv(immC);
                int rs2 = standBinaryCov(rs2C);
                int rs1 = standBinaryCov(rs1C);

                char statement[100] = "blt ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RS2Char[20] = "";
                sprintf(RS2Char, "t%d", rs2);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RS2Char, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                BLT(rs1,rs2,imm);
            } else if(strcmp(func, "101") == 0)
            {
                //BGE
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char rs2C[6] = "";
                for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                char immC[14] = "";
                immC[0] = fullLine[0];
                immC[1] = fullLine[24];
                for(int i = 0; i < 6; i++){immC[i+2] = fullLine[i+1];}
                for(int i = 0; i < 4; i++){immC[i+8] = fullLine[20+i];}
                immC[13] = '0';
                int imm = twosBinaryConv(immC);
                int rs2 = standBinaryCov(rs2C);
                int rs1 = standBinaryCov(rs1C);

                char statement[100] = "bge ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RS2Char[20] = "";
                sprintf(RS2Char, "t%d", rs2);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RS2Char, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                BGE(rs1,rs2,imm);
            } else if(strcmp(func, "110") == 0)
            {
                //BLTU
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char rs2C[6] = "";
                for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                char immC[14] = "";
                immC[0] = fullLine[0];
                immC[1] = fullLine[24];
                for(int i = 0; i < 6; i++){immC[i+2] = fullLine[i+1];}
                for(int i = 0; i < 4; i++){immC[i+8] = fullLine[20+i];}
                immC[13] = '0';
                int imm = twosBinaryConv(immC);
                int rs2 = standBinaryCov(rs2C);
                int rs1 = standBinaryCov(rs1C);

                char statement[100] = "bltu ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RS2Char[20] = "";
                sprintf(RS2Char, "t%d", rs2);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RS2Char, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                BLTU(rs1,rs2,imm);
            } else if(strcmp(func, "111") == 0)
            {
                //BGEU
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char rs2C[6] = "";
                for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                char immC[14] = "";
                immC[0] = fullLine[0];
                immC[1] = fullLine[24];
                for(int i = 0; i < 6; i++){immC[i+2] = fullLine[i+1];}
                for(int i = 0; i < 4; i++){immC[i+8] = fullLine[20+i];}
                int imm = twosBinaryConv(immC);
                int rs2 = standBinaryCov(rs2C);
                int rs1 = standBinaryCov(rs1C);

                char statement[100] = "bgeu ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RS2Char[20] = "";
                sprintf(RS2Char, "t%d", rs2);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RS2Char, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                BGEU(rs1,rs2,imm);
            } 
        } else if(strcmp(opcode, "0000011") == 0)
        {
            // Load Functions
            if(strcmp(func, "000") == 0)
            {
                //LB
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char immC[13] = "";
                for(int i = 0; i < 12; i++){immC[i] = fullLine[i];}
                int imm = twosBinaryConv(immC);
                int rs1 = standBinaryCov(rs1C);
                int ird = standBinaryCov(rd);
                
                char statement[100] = "lb ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RDChar[20] = "";
                sprintf(RDChar, "t%d", ird);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RDChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strncat(statement, "(", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ")", 3);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                LB(ird,imm,rs1);

            } else if(strcmp(func, "001") == 0)
            {
                //LH
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char immC[13] = "";
                for(int i = 0; i < 12; i++){immC[i] = fullLine[i];}
                int imm = twosBinaryConv(immC);
                int rs1 = standBinaryCov(rs1C);
                int ird = standBinaryCov(rd);

                char statement[100] = "lh ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RDChar[20] = "";
                sprintf(RDChar, "t%d", ird);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RDChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strncat(statement, "(", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ")", 3);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                LH(ird,imm,rs1);
            } else if(strcmp(func, "010") == 0)
            {
                //LW
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char immC[13] = "";
                for(int i = 0; i < 12; i++){immC[i] = fullLine[i];}
                int imm = twosBinaryConv(immC);
                int rs1 = standBinaryCov(rs1C);
                int ird = standBinaryCov(rd);

                char statement[100] = "lw ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RDChar[20] = "";
                sprintf(RDChar, "t%d", ird);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RDChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strncat(statement, "(", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ")", 3);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                LW(ird,imm,rs1);
            } else if(strcmp(func, "100") == 0)
            {
                //LBU
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char immC[13] = "";
                for(int i = 0; i < 12; i++){immC[i] = fullLine[i];}
                int imm = twosBinaryConv(immC);
                int rs1 = standBinaryCov(rs1C);
                int ird = standBinaryCov(rd);

                char statement[100] = "lbu ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RDChar[20] = "";
                sprintf(RDChar, "t%d", ird);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RDChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strncat(statement, "(", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ")", 3);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                LBU(ird,imm,rs1);
            } else if(strcmp(func, "101") == 0)
            {
                //LHU
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char immC[13] = "";
                for(int i = 0; i < 12; i++){immC[i] = fullLine[i];}
                int imm = twosBinaryConv(immC);
                int rs1 = standBinaryCov(rs1C);
                int ird = standBinaryCov(rd);

                char statement[100] = "lhu ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RDChar[20] = "";
                sprintf(RDChar, "t%d", ird);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RDChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strncat(statement, "(", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ")", 3);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                LHU(ird,imm,rs1);
            }
        } else if(strcmp(opcode, "0100011") == 0)
        {
            // Store Functions
            if(strcmp(func, "000") == 0)
            {
                //SB
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char rs2C[6] = "";
                for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                char immC[11] = "";
                for(int i = 0; i < 6; i++){immC[i] = fullLine[i];}
                for(int i = 0; i < 5; i++){immC[i+6] = fullLine[20+i];}
                int imm = twosBinaryConv(immC);
                int rs2 = standBinaryCov(rs2C);
                int rs1 = standBinaryCov(rs1C);

                char statement[100] = "sb ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RS2Char[20] = "";
                sprintf(RS2Char, "t%d", rs2);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RS2Char, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strncat(statement, "(", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ")", 3);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                SB(rs2,imm,rs1);
            } else if(strcmp(func, "001") == 0)
            {
                //SH
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char rs2C[6] = "";
                for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                char immC[11] = "";
                for(int i = 0; i < 6; i++){immC[i] = fullLine[i];}
                for(int i = 0; i < 5; i++){immC[i+6] = fullLine[20+i];}
                int imm = twosBinaryConv(immC);
                int rs2 = standBinaryCov(rs2C);
                int rs1 = standBinaryCov(rs1C);

                char statement[100] = "sh ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RS2Char[20] = "";
                sprintf(RS2Char, "t%d", rs2);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RS2Char, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strncat(statement, "(", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ")", 3);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                SH(rs2,imm,rs1);
            } else if(strcmp(func, "010") == 0)
            {
                //SW
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char rs2C[6] = "";
                for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                char immC[11] = "";
                for(int i = 0; i < 6; i++){immC[i] = fullLine[i];}
                for(int i = 0; i < 5; i++){immC[i+6] = fullLine[20+i];}
                int imm = twosBinaryConv(immC);
                int rs2 = standBinaryCov(rs2C);
                int rs1 = standBinaryCov(rs1C);

                char statement[100] = "sw ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RS2Char[20] = "";
                sprintf(RS2Char, "t%d", rs2);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RS2Char, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strncat(statement, "(", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ")", 3);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                SW(rs2,imm,rs1);
            }
        } else if(strcmp(opcode, "0010011") == 0)
        {
            // Immediate Instructions
            if(strcmp(func, "000") == 0)
            {
                //ADDI
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char immC[13] = "";
                for(int i = 0; i < 12; i++){immC[i] = fullLine[i];}
                int imm = twosBinaryConv(immC);
                int rs1 = standBinaryCov(rs1C);
                int ird = standBinaryCov(rd);

                char statement[100] = "addi ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RDChar[20] = "";
                sprintf(RDChar, "t%d", ird);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RDChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                ADDI(ird,rs1,imm);
            } else if(strcmp(func, "010") == 0)
            {
                //SLTI
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char immC[13] = "";
                for(int i = 0; i < 12; i++){immC[i] = fullLine[i];}
                int imm = twosBinaryConv(immC);
                int rs1 = standBinaryCov(rs1C);
                int ird = standBinaryCov(rd);

                char statement[100] = "slti ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RDChar[20] = "";
                sprintf(RDChar, "t%d", ird);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RDChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                SLTI(ird,rs1,imm);
            } else if(strcmp(func, "011") == 0)
            {
                //SLTIU
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char immC[13] = "";
                for(int i = 0; i < 12; i++){immC[i] = fullLine[i];}
                int imm = twosBinaryConv(immC);
                int rs1 = standBinaryCov(rs1C);
                int ird = standBinaryCov(rd);

                char statement[100] = "sltiu ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RDChar[20] = "";
                sprintf(RDChar, "t%d", ird);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RDChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                SLTIU(ird,rs1,imm);
            } else if(strcmp(func, "100") == 0)
            {
                //XORI
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char immC[13] = "";
                for(int i = 0; i < 12; i++){immC[i] = fullLine[i];}
                int imm = twosBinaryConv(immC);
                int rs1 = standBinaryCov(rs1C);
                int ird = standBinaryCov(rd);

                char statement[100] = "xori ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RDChar[20] = "";
                sprintf(RDChar, "t%d", ird);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RDChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                XORI(ird,rs1,imm);
            } else if(strcmp(func, "110") == 0)
            {
                //ORI
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char immC[13] = "";
                for(int i = 0; i < 12; i++){immC[i] = fullLine[i];}
                int imm = twosBinaryConv(immC);
                int rs1 = standBinaryCov(rs1C);
                int ird = standBinaryCov(rd);

                char statement[100] = "ori ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RDChar[20] = "";
                sprintf(RDChar, "t%d", ird);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RDChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                ORI(ird,rs1,imm);
            } else if(strcmp(func, "111") == 0)
            {
                //ANDI
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char immC[13] = "";
                for(int i = 0; i < 12; i++){immC[i] = fullLine[i];}
                int imm = twosBinaryConv(immC);
                int rs1 = standBinaryCov(rs1C);
                int ird = standBinaryCov(rd);

                char statement[100] = "andi ";
                char ImmChar[20] = "";
                sprintf(ImmChar, "%d", imm);
                char RDChar[20] = "";
                sprintf(RDChar, "t%d", ird);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RDChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ImmChar, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                ANDI(ird,rs1,imm);
            } else if(strcmp(func, "001") == 0)
            {
                //SLLI
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char rs2C[6] = "";
                for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                int rs2 = standBinaryCov(rs2C);
                int rs1 = standBinaryCov(rs1C);
                int ird = standBinaryCov(rd);

                char statement[100] = "slli ";
                char ShmantChar[20] = "";
                sprintf(ShmantChar, "%d", rs2);
                char RDChar[20] = "";
                sprintf(RDChar, "t%d", ird);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RDChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, ShmantChar, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                SLLI(ird,rs1,rs2);
            } else if(strcmp(func, "101") == 0)
            {
                if(fullLine[1] == '0')
                {
                    //SRLI
                    char rs1C[6] = "";
                    for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                    char rs2C[6] = "";
                    for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                    int rs2 = standBinaryCov(rs2C);
                    int rs1 = standBinaryCov(rs1C);
                    int ird = standBinaryCov(rd);

                    char statement[100] = "srli ";
                    char ShmantChar[20] = "";
                    sprintf(ShmantChar, "%d", rs2);
                    char RDChar[20] = "";
                    sprintf(RDChar, "t%d", ird);
                    char RSChar[20] = "";
                    sprintf(RSChar, "t%d", rs1);
                    strncat(statement, RDChar, 20);
                    strncat(statement, ", ", 3);
                    strncat(statement, RSChar, 20);
                    strncat(statement, ", ", 3);
                    strncat(statement, ShmantChar, 20);
                    strcpy(asmPrint[asmPC], statement);
                    asmPC += 1;
                    SRLI(ird,rs1,rs2);
                } else
                {
                    //SRAI
                    char rs1C[6] = "";
                    for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                    char rs2C[6] = "";
                    for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                    int rs2 = standBinaryCov(rs2C);
                    int rs1 = standBinaryCov(rs1C);
                    int ird = standBinaryCov(rd);

                    char statement[100] = "srai ";
                    char ShmantChar[20] = "";
                    sprintf(ShmantChar, "%d", rs2);
                    char RDChar[20] = "";
                    sprintf(RDChar, "t%d", ird);
                    char RSChar[20] = "";
                    sprintf(RSChar, "t%d", rs1);
                    strncat(statement, RDChar, 20);
                    strncat(statement, ", ", 3);
                    strncat(statement, RSChar, 20);
                    strncat(statement, ", ", 3);
                    strncat(statement, ShmantChar, 20);
                    strcpy(asmPrint[asmPC], statement);
                    asmPC += 1;
                    SRAI(ird,rs1,rs2);
                }
            } 
        } else if(strcmp(opcode, "0110011") == 0)
        {
            // Register Instructions
            if(strcmp(func, "000") == 0)
            {
                if(fullLine[1] == '0')
                {
                    //ADD
                    char rs1C[6] = "";
                    for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                    char rs2C[6] = "";
                    for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                    int rs2 = standBinaryCov(rs2C);
                    int rs1 = standBinaryCov(rs1C);
                    int ird = standBinaryCov(rd);

                    char statement[100] = "add ";
                    char RS2Char[20] = "";
                    sprintf(RS2Char, "t%d", rs2);
                    char RDChar[20] = "";
                    sprintf(RDChar, "t%d", ird);
                    char RSChar[20] = "";
                    sprintf(RSChar, "t%d", rs1);
                    strncat(statement, RDChar, 20);
                    strncat(statement, ", ", 3);
                    strncat(statement, RSChar, 20);
                    strncat(statement, ", ", 3);
                    strncat(statement, RS2Char, 20);
                    strcpy(asmPrint[asmPC], statement);
                    asmPC += 1;
                    ADD(ird,rs1,rs2);
                } else
                {
                    //SUB
                    char rs1C[6] = "";
                    for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                    char rs2C[6] = "";
                    for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                    int rs2 = standBinaryCov(rs2C);
                    int rs1 = standBinaryCov(rs1C);
                    int ird = standBinaryCov(rd);

                    char statement[100] = "sub ";
                    char RS2Char[20] = "";
                    sprintf(RS2Char, "t%d", rs2);
                    char RDChar[20] = "";
                    sprintf(RDChar, "t%d", ird);
                    char RSChar[20] = "";
                    sprintf(RSChar, "t%d", rs1);
                    strncat(statement, RDChar, 20);
                    strncat(statement, ", ", 3);
                    strncat(statement, RSChar, 20);
                    strncat(statement, ", ", 3);
                    strncat(statement, RS2Char, 20);
                    strcpy(asmPrint[asmPC], statement);
                    asmPC += 1;
                    SUB(ird,rs1,rs2);
                }
            } else if(strcmp(func, "001") == 0)
            {
                // SSL
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char rs2C[6] = "";
                for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                int rs2 = standBinaryCov(rs2C);
                int rs1 = standBinaryCov(rs1C);
                int ird = standBinaryCov(rd);

                char statement[100] = "ssl ";
                char RS2Char[20] = "";
                sprintf(RS2Char, "t%d", rs2);
                char RDChar[20] = "";
                sprintf(RDChar, "t%d", ird);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RDChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RS2Char, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                SLL(ird,rs1,rs2);
            } else if(strcmp(func, "010") == 0)
            {
                // SLT
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char rs2C[6] = "";
                for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                int rs2 = standBinaryCov(rs2C);
                int rs1 = standBinaryCov(rs1C);
                int ird = standBinaryCov(rd);

                char statement[100] = "slt ";
                char RS2Char[20] = "";
                sprintf(RS2Char, "t%d", rs2);
                char RDChar[20] = "";
                sprintf(RDChar, "t%d", ird);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RDChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RS2Char, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                SLT(ird,rs1,rs2);
            } else if(strcmp(func, "011") == 0)
            {
                // SLTU
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char rs2C[6] = "";
                for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                int rs2 = standBinaryCov(rs2C);
                int rs1 = standBinaryCov(rs1C);
                int ird = standBinaryCov(rd);

                char statement[100] = "sltu ";
                char RS2Char[20] = "";
                sprintf(RS2Char, "t%d", rs2);
                char RDChar[20] = "";
                sprintf(RDChar, "t%d", ird);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RDChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RS2Char, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                SLTU(ird,rs1,rs2);
            } else if(strcmp(func, "100") == 0)
            {
                // XOR
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char rs2C[6] = "";
                for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                int rs2 = standBinaryCov(rs2C);
                int rs1 = standBinaryCov(rs1C);
                int ird = standBinaryCov(rd);

                char statement[100] = "xor ";
                char RS2Char[20] = "";
                sprintf(RS2Char, "t%d", rs2);
                char RDChar[20] = "";
                sprintf(RDChar, "t%d", ird);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RDChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RS2Char, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                XOR(ird,rs1,rs2);
            } else if(strcmp(func, "101") == 0)
            {
                if(fullLine[1] == '0')
                {
                    //SRL
                    char rs1C[6] = "";
                    for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                    char rs2C[6] = "";
                    for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                    int rs2 = standBinaryCov(rs2C);
                    int rs1 = standBinaryCov(rs1C);
                    int ird = standBinaryCov(rd);

                    char statement[100] = "srl ";
                    char RS2Char[20] = "";
                    sprintf(RS2Char, "t%d", rs2);
                    char RDChar[20] = "";
                    sprintf(RDChar, "t%d", ird);
                    char RSChar[20] = "";
                    sprintf(RSChar, "t%d", rs1);
                    strncat(statement, RDChar, 20);
                    strncat(statement, ", ", 3);
                    strncat(statement, RSChar, 20);
                    strncat(statement, ", ", 3);
                    strncat(statement, RS2Char, 20);
                    strcpy(asmPrint[asmPC], statement);
                    asmPC += 1;
                    SRL(ird,rs1,rs2);
                } else
                {
                    //SRA
                    char rs1C[6] = "";
                    for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                    char rs2C[6] = "";
                    for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                    int rs2 = standBinaryCov(rs2C);
                    int rs1 = standBinaryCov(rs1C);
                    int ird = standBinaryCov(rd);

                    char statement[100] = "sra ";
                    char RS2Char[20] = "";
                    sprintf(RS2Char, "t%d", rs2);
                    char RDChar[20] = "";
                    sprintf(RDChar, "t%d", ird);
                    char RSChar[20] = "";
                    sprintf(RSChar, "t%d", rs1);
                    strncat(statement, RDChar, 20);
                    strncat(statement, ", ", 3);
                    strncat(statement, RSChar, 20);
                    strncat(statement, ", ", 3);
                    strncat(statement, RS2Char, 20);
                    strcpy(asmPrint[asmPC], statement);
                    asmPC += 1;
                    SRA(ird,rs1,rs2);
                }
            } else if(strcmp(func, "110") == 0)
            {
                // OR
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char rs2C[6] = "";
                for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                int rs2 = standBinaryCov(rs2C);
                int rs1 = standBinaryCov(rs1C);
                int ird = standBinaryCov(rd);

                char statement[100] = "or ";
                char RS2Char[20] = "";
                sprintf(RS2Char, "t%d", rs2);
                char RDChar[20] = "";
                sprintf(RDChar, "t%d", ird);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RDChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RS2Char, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                OR(ird,rs1,rs2);
            } else if(strcmp(func, "111") == 0)
            {
                // AND
                char rs1C[6] = "";
                for(int i = 0; i < 5; i++){rs1C[i] = fullLine[12+i];}
                char rs2C[6] = "";
                for(int i = 0; i < 5; i++){rs2C[i] = fullLine[7+i];}
                int rs2 = standBinaryCov(rs2C);
                int rs1 = standBinaryCov(rs1C);
                int ird = standBinaryCov(rd);

                char statement[100] = "and ";
                char RS2Char[20] = "";
                sprintf(RS2Char, "t%d", rs2);
                char RDChar[20] = "";
                sprintf(RDChar, "t%d", ird);
                char RSChar[20] = "";
                sprintf(RSChar, "t%d", rs1);
                strncat(statement, RDChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RSChar, 20);
                strncat(statement, ", ", 3);
                strncat(statement, RS2Char, 20);
                strcpy(asmPrint[asmPC], statement);
                asmPC += 1;
                AND(ird,rs1,rs2);
            } 
        } 
    }





    /*
        Command Prompt
    */
    printf("Current command:\n");
    printf("    'r'             runs the entire program in one go till it hits a breakpoint or exits.\n");
    printf("    's'             runs the next instruction and then stops and waits for next command.\n");
    printf("    'x0' to 'x31'   return the contents of the register from the register file (x0 must always stay 0).\n");
    printf("    '0x12345678'    returns the contents from the address 0x12345678 in the data memory.\n");
    printf("    'pc'            returns the value of the PC.\n");
    printf("    'inst'          prints the \"assembly of the instruction\" that will be executed next.\n");
    printf("    'b[pc]'         will put a breakpoint at a particular/ specied [pc]. So, if 'r' is given as the next command, the code will run till [pc] (or the end, if [pc] is not hit) and then break there, allowing any of these commands after the break. The simulator must allow storing up to 5 breakpoints at a time.\n");
    printf("    'c'             continues the execution, till it hits the next breakpoint pc or exits.\n");
    printf("    'exit'          exits the terminal.\n");

    char UserInputCommand[10];
    bool ProgramStarted = false;
    bool ProgramFinished = false;
    bool TerminalRun = true;

    char nextLine[50] = ".text\n";
    fwrite(nextLine,strlen(nextLine),1,asmFileWrite);
    for(int i = 0; i < 50; i++)
    {
        if(asmPrint[i][0] != '\0')
        {
            strcpy(nextLine,asmPrint[i]);
            strcat(nextLine,"\n");
            fwrite(nextLine,strlen(nextLine),1,asmFileWrite);
        } else {i = 50;}
    }

    /*
        Terminal Backend
    */

    while (TerminalRun == true){
        printf("Enter a command: ");
        scanf("%s", UserInputCommand);
 
        /*
            r - runs the entire program in one go till it hits a breakpoint or exits
        */
            // strcmp returns -1 if not equal, 0 if equal
        if ((strcmp(UserInputCommand, "r")) == 0) {
            startTimer();
            if (ProgramFinished == true) {
                printf("Program has already been ran, please restart terminal to run this command.\n");
            } else if (ProgramStarted == true) {
                printf("Program is in the middle of execution, please restart terminal to run this command.\n");
            } else {
                ProgramStarted = true;
                while((fgets(currentLine, 50, datFileRead)) != NULL)
                {
                    printf("%s", currentLine);
                }
                ProgramFinished = true; 
                fclose(datFileRead);
                fclose(asmFileWrite);
            }
            stopTimer();
        /*
           s - runs the next instruction and then stops and waits for next command
        */
        
        } else if ((strcmp(UserInputCommand, "s")) == 0) {
            startTimer();
            if (ProgramFinished == true) {
                printf("Program has already been ran, please restart terminal to run this command.\n");
            } else if ((fgets(currentLine, 50, datFileRead)) != NULL) {
                printf("%s", currentLine);
            } else {
                ProgramFinished = true;
                fclose(datFileRead);
                fclose(asmFileWrite);
                printf("No more lines to read please restart terminal to run this command.\n");
            }
            stopTimer();

        /*
            'x0' to 'x31' - return the contents of the register from the register file (x0 must always stay 0).
        */

        } else if (compareChar(UserInputCommand[0], 'x') == 0) { // Register Command
            if (strlen(UserInputCommand) > 1) {
                char indexString[10];
                sprintf(indexString, "%c%c", UserInputCommand[1], UserInputCommand[2]);
                int index = atoi(indexString);

                char registerString[10];

                if (strlen(UserInputCommand) == 2){
                    sprintf(registerString, "x%c: ", UserInputCommand[1]);
                } else {
                    sprintf(registerString, "x%c%c: ", UserInputCommand[1], UserInputCommand[2]);
                }

                char str[100];
                char RegisterValueString[100];
                sprintf(RegisterValueString, "%d", Registers[index]);
                strcpy(str, registerString);
                strcat(str, RegisterValueString);
                printf("%s", str);
                newLine();
            }
        
        /*
            pc - returns the value of the PC.
        */

        } else if ((strcmp(UserInputCommand, "pc")) == 0) {
            char str[100];
            char PCString[100];
            sprintf(PCString, "%d", ProgramCounter);
            strcpy(str, "PC: 0x");
            strcat(str, PCString);
            printf("%s",str);
            newLine();
        
        /*
            c - continues the execution, till it hits the next breakpoint pc or exits.
        */
        
        } else if ((strcmp(UserInputCommand, "c")) == 0) {
            startTimer();
            if (ProgramFinished == true) {
                printf("Program has already been ran, please restart terminal to run this command.\n");
            } else if (ProgramStarted == true) {
                printf("Program has not started yet. Use 'r' or 's' to begin program.\n");
            } else {
                ProgramStarted = true;
                while ((fgets(currentLine, 50, datFileRead)) != NULL) { // While there is a string to read, print the output
                    printf("%s", currentLine);
                }

                printRegisters();
                ProgramFinished = true;
                fclose(datFileRead);
                fclose(asmFileWrite);
            }
            stopTimer();
        /*
            exit - exits the terminal.
        */

        } else if ((strcmp(UserInputCommand, "exit")) == 0) {
            TerminalRun = false;
        }
    }
}