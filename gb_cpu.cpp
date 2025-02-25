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
#define FLAG_RESET 0xF0


#include <bitset> // using this for testing will remove once im done


int GLOBAL_COUNTER = 0; // global cycle counter for all units 


// Resolution
const short RESO_WIDTH = 160;
const short RESO_HEIGHT = 144;



const byte BANK_SIZE = 16;
// the ID of a tile can be found using the following algo adress / 16 mod 256


// main processor using struct just because they feel simpler no need for super abstract OOP
struct CPU{
  // 8-bit Registers
  byte A; // Acummulator all math functions are done here
  byte B;
  byte C;
  byte D;
  byte E;
  byte H;
  byte L;
  byte F; // flag register
  /* the F is regsiters stores the flags
   * the first bit is the Zero flag (Z)
   * the second bit is the Subtract flag (S)
   * the thoird bit is the Half carry flag (H)
   * the fourth bit is the carry flag (c)
   * the upper four bits are for the combinational address
   * */

  // 16-bit Registers
  word PC; // prgram counter
  word SP; // stack pointer
  /*NOTE that some of the 8bit registers can be stored as 
   * AF
   * BC
   *
   *
   *
   * */

  // initialize in cpu init function later
  bool halt_master_controller = false; 
  int* global_counter_ptr = &GLOBAL_COUNTER;


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

  void writeToMemory(word address,byte value){
    memory[address] = value;
  }

  byte decBytes(byte b1,byte b2){
    return b1 - b2;
  }

  byte addBytes(byte b1,byte b2){
    return b1 + b2;
  }

  word addWords(word w1,word w2){
    *global_counter_ptr+=1;
    return w1 + w2;
  }

  byte& operator[](word index) {
    ++(*global_counter_ptr);
    return memory[index];
  }


  /* NOTE the processor is little endian
  So the least sig byte is stored first
  0x1234 is stored like [0x34,0x12] in memory */ 
  void execute(){
    while(true){
      while(!halt_master_controller){
        byte opcode = memory[PC++];
        switch(opcode){
          /* some naming conventions
              low_byte = least sig bit
              high_byte = most sig_bit */
          case 0x01:{
            // stores 16 bit value into register BC
            // 3 cycles
            byte low_byte = memory[PC++];
            byte high_byte = memory[PC++];
            B = low_byte;
            C = high_byte;
            break;
          }
          case 0x11:{
            // store 16bit value into DE
            // 3 cycles
            byte low_byte = memory[PC++];
            byte high_byte = memory[PC++];
            D = low_byte;
            E = high_byte;
            break;
          }
          case 0x21:{
            //stores 16bit value in HL
            // 3 cycles
            byte low_byte = memory[PC++];
            byte high_byte = memory[PC++];
            H = low_byte;
            L = high_byte;
            break;
          }
          case 0x31:{
            //load 16bit value into SP
            // 3 cycles
            word low_byte = memory[PC++];
            word high_byte = memory[PC++];
            word v16 = (high_byte << 8)| low_byte;
            SP = v16;
            break;
          }
          case 0x08:{
            // store Sp in to 16bit address a16  and a16 + 1
            // 5 cycles
            // low and high bytes of SP
            byte low_byte = SP & 0xFF;
            byte high_byte = (SP >> 8) & 0xFF;
            
            // full 16bit address
            byte address_low_byte = memory[PC++];
            byte address_high_byte = memory[PC++];
            word a16 = address_high_byte << 8 | address_low_byte;
            writeToMemory(a16++,low_byte);
            writeToMemory(a16,high_byte);
            break;

          }
          case 0x03: {
            // INC BC By 1
            // 2 cycles
            word v16 = (C << 8) | B;
            v16 = addWords(v16, 0x0001); 
            B = v16 & 0xFF;            
            C = (v16 >> 8) & 0xFF;       
            std::cout << "C: 0x" << std::hex <<  static_cast<int>(C) << '\n';
            std::cout << "B: 0x" << std::hex << static_cast<int>(B)<< '\n';
            break;
        }
        case 0x13: {
          // INC DE by 1
          // 2 cycles
          word v16 = (E << 8) | D;
          v16 = addWords(v16,0x0001);
          E = v16 & 0xFF;
          D = (v16 >> 8) & 0xFF;
          break;

        }
        case 0x23:{
          // INC HL by 1
          // 2 cycles
          word v16 = (L << 8) | H;
          v16 = addWords(v16,0x0001);
          L = v16 & 0xFF;
          H = (v16 >> 8) & 0xFF;
          break;
        }
        case 0x33:{
          // Inc SP by 1
          // 2 cycles
          SP = addWords(SP,0x0001);
          break;
        }
        case 0x09:{
          //add BC to HL store to HL
          // 2 cycles
          word v16 = (C << 8) | B;
          word v16_2 = (H << 8) | L;
          v16 = addWords(v16,v16_2);
          L = v16 & 0xFF;
          H = (v16 >> 8) & 0xFF;
          break;

        }
        case 0x19:{
          //add DE to HL store to HL
          // 2 cycles
          word v16 = (E << 8) | D;
          word v16_2 = (H << 8) | L;
          v16 = addWords(v16,v16_2);
          L = v16 & 0xFF;
          H = (v16 >> 8) & 0xFF;
          break;

        }
        case 0x29:{
          //add DE to HL store to HL
          // 2 cycles
          word v16 = (H << 8) | L;
          word v16_2 = (H << 8) | L;
          v16 = addWords(v16,v16_2);
          L = v16 & 0xFF;
          H = (v16 >> 8) & 0xFF;
          break;

        }
        case 0x39:{
          //add SP to HL store to HL
          // 2 cycles
          word v16 = (H << 8) | L;
          v16 = addWords(v16,SP);
          L = v16 & 0xFF;
          H = (v16 >> 8) & 0xFF;
          break;
        }
        case 0x04:{
          // INC B by 1
          B = addBytes(B,0x01);
          handleFlag_8(B);
          break;
        }
        case 0x14:{
          // INC D by 1
          // 1 cycle
          D = addBytes(D,0x01);
          handleFlag_8(D);
          break;
        }

        case 0x24:{
          H = addBytes(D,0x01);
          handleFlag_8(H);
          break;
          // INC H by 1
        }

        case 0x34:{
          // INC item stored at memory address in HL by 1
          word a16 = (H << 8) | L;
          byte val = readFromMemory(a16);
          val = addBytes(val,0x01);
          writeToMemory(a16,val);
          handleFlag_8(val);
          break;
        }
        case 0x0C:{
          //INC C by 1
          C = addBytes(C,0x01);
          handleFlag_8(C);
          break;
        }
        case 0x1C:{
          //INC E by 1
          E = addBytes(E,0x01);
          handleFlag_8(E);
          break;
        }
        case 0x2C:{
          //INC L by 1
          L = addBytes(L,0x01);
          handleFlag_8(L);
          break;
        }
        case 0x3C:{
          //INC A by 1
          A = addBytes(A,0x01);
          handleFlag_8(A);
          break;
        }
        
        }
      }
      break;
    }
  }

  void handleFlag_8(byte process_result){
    // takes the process of some operation and applies bitwise applications to register F
    // this only accounts for ZERO CARRY AND HALF CARRY
    
    if(process_result == 0){ // ZERO
      std::cout << "ZERO\n";
      F |= ZERO;
      std::cout << std::bitset<8>(F) << '\n';
    }else{
      F &= ~ZERO;
    }
    if(process_result > 0xFF){ // Carry
      std::cout << "CARRY\n";
      F |= CARRY;
    }else{
      F &= ~CARRY;
    }
    if(process_result > 0x0F){ // Half Carry
      F |= HALF_CARRY;
      std::cout << "HALF CARRY\n";
    }
    std::cout << "Register F Value: " << std::bitset<8>(F) << '\n';

  }
  
  // flag handling for 16-bit operations 
  void handleFlag_16(word process_result){ 
    std::cout << "sixteen bit operation" << '\n';


  }
  
};

void initCPU(){
  std::cout << "CPU is being intialized..." << '\n';
  CPU cpu;
  std::cout << "Testing Memory init: \n";
  cpu.PC = 0;
  cpu.memory[0] = 0x01;
  cpu.memory[1] = 0x34;
  cpu.memory[2] = 0x12;
  cpu.memory[3] = 0x03;

  cpu.execute();
  
  

  
}

int main(){

  initCPU();
  return 0;
}
