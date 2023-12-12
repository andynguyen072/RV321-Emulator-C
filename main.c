#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 
#include "ISA_Instructions"
#include "Instructions_Load.h"
#include "Global_Variables.h"
#include "string.h"

/*
    Misc Functions
*/
void newLine(){
    printf("\n");
}

int compareChar(char a, char b) {
    if (a == b)
        return 0;
    else
        return -1;
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
    printf("%c", myString[2]);

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

    /*
        File Loading / Parsing
    */
    int n = LoadInstructions(FileName);


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
    char currentLine[8] = "";
    bool ProgramFinished = false;
    bool TerminalRun = true;

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
            if (ProgramFinished == true) {
                printf("Program has already been ran, please restart terminal to run this command.\n");
            } else if (strlen(currentLine) != 0) {
                printf("Program is in the middle of execution, please restart terminal to run this command.\n");
            } else {
                while ((fgets(currentLine, 50, datFileRead)) != NULL) { // While there is a string to read, print the output
                    printf("%s", currentLine);
                }

                ProgramFinished = true;
                fclose(datFileRead);
            }
        /*
           s - runs the next instruction and then stops and waits for next command
        */
        
        } else if ((strcmp(UserInputCommand, "s")) == 0) {
            if (ProgramFinished == true) {
                printf("Program has already been ran, please restart terminal to run this command.\n");
            } else if ((fgets(currentLine, 50, datFileRead)) != NULL) {
                printf("%s", currentLine);
            } else {
                ProgramFinished = true;
                fclose(datFileRead);
                printf("No more lines to read please restart terminal to run this command.\n");
            }
        
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
            if (ProgramFinished == true) {
                printf("Program has already been ran, please restart terminal to run this command.\n");
            } else if (strlen(currentLine) == 0) {
                printf("Program has not started yet. Use 'r' or 's' to begin program.\n");
            } else {
                while ((fgets(currentLine, 50, datFileRead)) != NULL) { // While there is a string to read, print the output
                    printf("%s", currentLine);
                }

                ProgramFinished = true;
                fclose(datFileRead);
            }
        
        /*
            exit - exits the terminal.
        */

        } else if ((strcmp(UserInputCommand, "exit")) == 0) {
            TerminalRun = false;
        }
    }
}