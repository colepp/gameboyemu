#include <iostream>
#define byte unsigned char

const short RESO_WIDTH = 160;
const short RESO_HEIGHT = 144;

const int VIDEO_MEM_SIZE = 8192;
const int MEM_SIZE = 8192;

const byte BANK_SIZE = 16;
// the ID of a tile can be found using the following algo adress / 16 mod 256


struct CPU{
  // 8-bit Registers
  byte A;
  byte B;
  byte C;
  byte D;
  byte E;
  byte F;

  // 16-bit Registers
  
  byte memeory[MEM_SIZE]; // 8 Kib of mem
  byte video_memeory[VIDEO_MEM_SIZE];
};

void initCPU(){
  std::cout << "CPU is being intialized..." << '\n';
  CPU cpu;
}

int main(){
  initCPU();
  return 0;
}
