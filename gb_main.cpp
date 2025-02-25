#include <iostream>
#include <stdint.h>
#include<bitset>
// MAIN FILE TASK
// CREATE MAIN FUNCTION THAT PUTS ALL MAIN PARTS TOGETHER TO WORK
//

int main(){
    
    uint16_t a = 0x1234;
    a = (a >> 8) & 0xFF;
    std::cout << std::hex << a;
    return 0;
}