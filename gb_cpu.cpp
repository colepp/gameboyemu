#include <iostream>
#include <stdint.h>
#define byte uint8_t
#define word uint16_t
#define MEM_SIZE  65536

// flag hexes
#define ZERO 0x80
#define SUB 0x40
#define HALF_CARRY 0x20
#define CARRY 0x10


const short RESO_WIDTH = 160;
const short RESO_HEIGHT = 144;


const byte BANK_SIZE = 16;
// the ID of a tile can be found using the following algo adress / 16 mod 256


struct CPU{
  // 8-bit Registers
  byte A; // Acummulator all math functions are done here
  byte B;
  byte C;
  byte D;
  byte E;
  byte F; // flag register
  /* the F is regsiters stores the flags
   * the first bit is the Zero flag (Z)
   * the second bit is the Subtract flag (S)
   * the thoird bit is the Half carry flag (H)
   * the fourth bit is the carry flag (c)
   *
   * */
  // 16-bit Registers
  word PC; // prgram counter
  word SP; // stack pointer

  

  byte memory[MEM_SIZE]; // 64 kb of memory
  // 0000 - 7FFF cartridge memory
  // 8000 - 9FFF Video ram
  // A000 - BFFF External work ram
  // C000 - DFFF Work ram
  // E000 - FDFF Echo ram use prohibited
  // FE00 - FE9F OAM Object Attribuite memory
  // FEA0 - FEFF Not Used
  // FF00 - FF7F I/O Registebrs
  // FF80 - FFFE HRAM(High Ram)
  // FFFF Interupt Enable
  byte readFromMemory(word address){
    byte data = memory[address];
    return data;
  }

  void execute(){
    byte opcode = memory[PC++] // get current instruction then increment program counter
    switch(opcode){

    }
  }

  void handleFlag(byte process_result){
    // takes the process of some operation and applies bitwise applications to register F
  }
  



};

void initCPU(){
  std::cout << "CPU is being intialized..." << '\n';
  CPU cpu;
  std::cout << "Testing Memory init: \n";
  cpu.PC = 0;
  cpu.memory[0] = 20;
  std::cout << cpu.memory[0] << '\n';
  byte data = cpu.readFromMemory();
  std::cout << "printing data: " << (int)data;

}


int main(){

  initCPU();

  return 0;
}
