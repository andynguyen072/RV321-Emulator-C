#include <stdio.h>
#include "ISA_Instructions"
#include "Global_Variables.h"

int main(){
    ADDI(5, 3, 2);
    ADDI(1, 5, 5);
    printf("%d ", Registers[5]);
    printf("%d ", Registers[1]);
    printf("%d ", Registers[4]);
    return 0;
}