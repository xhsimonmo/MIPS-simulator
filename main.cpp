 #include <iostream>
#include <string>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <sstream>
#include "simulator.hpp"

#define IMEM_OFFSET 0x10000000//head of instruction memory
#define IMEM_LENGTH 0x1000000
#define DMEM_OFFSET 0x20000000
#define DMEM_LENGTH 0x4000000


int main(int argc, char *argv[])
{
  std::string binaryin = argv[1];
  if(argv[1] == NULL)
  {
    std::exit (-21);// IO error (-21) : the simulator encountered an error reading/writing input/output
  }

  std::vector<char> imem (IMEM_LENGTH, 0); //set up instruction memory

    // imem.resize(IMEM_LENGTH);// bianry instruction memory is saved in byte address
    std::streampos begin,end;
    std::streampos infile_size;
    std::ifstream infile (argv[1], std::ios::binary | std::ios::ate);

    infile_size = infile.tellg();
    int size_infile = infile_size;

  if (infile.is_open())
  {
    infile_size = infile.tellg();
    infile.seekg (0, std::ios::beg);
    infile.read (&imem[0], infile_size);
    infile.close();
  }
  else{
    std::exit(-21);// IO error (-21) : the simulator encountered an error reading/writing input/output
  }

  if(imem.size() > IMEM_LENGTH)
  {
      std::exit(-11);//喵喵喵: what to do with those in range (ask)
  }

    uint32_t ADDR_GETC;
    uint32_t ADDR_PUTC;
  std::vector<uint8_t> dmem(DMEM_LENGTH, 0); // initilise the length, also content to zero!

  std::vector<uint32_t> reg(32,0);
    uint32_t reg_lo = 0;
    uint32_t reg_hi = 0;

//   reg[1] = 3;
//   reg[2] = 103;
//   reg[5] = 5;
//   reg[6] = 2;
//   reg[4] = 0x20000000;
//   reg[8] = 0x7f;

  uint32_t pc = IMEM_OFFSET; // let pc first point to first instruction in instruction memory
  uint32_t tmp_pc;

  simulator s;//set up class simulator;

  bool delay = false;//branch delay slot bool default to FALSE;

    //////////////////////STAFF ONLY         SIMULATOR RUNNING HERE!!!
    //execute instructions

    while (pc < (IMEM_OFFSET + size_infile + 4) && pc >= IMEM_OFFSET && pc < IMEM_LENGTH + IMEM_OFFSET) //in case theres a jump instruction at the end
    {
        std::cerr << "R2 is " << reg[2]  << std::endl;
        uint32_t instruction = 0;
        for (int i = 0; i < 4; i++)
        {
            uint32_t mem_tmp = imem[pc - IMEM_OFFSET + i];
            mem_tmp = mem_tmp & 0b11111111;
            instruction = instruction | mem_tmp;//imem is 8 bit
            if (i != 3)
            {
                instruction = instruction  << 8;
            }
        }//load char into 32 bit instruction
         std::cerr << "--------------------        instruction is  " <<std::hex << instruction << '\n';
         reg[0] = 0; //keep reg 0 const to 0 in every iteration
        s.run(instruction, pc,tmp_pc, reg, reg_lo, reg_hi, delay, dmem, imem);

        if(!delay)
        {
          pc = pc + 4;//if pc is changed due to J or Branch, pc = desired address -4 and it will be restore to correct address here;
        }
        else//delay is true
        {
          pc = pc - 4 ;//go back to execute branch or
        }

        std::cerr << " PC here has value: " << std::hex << pc << std::endl;

    }

   for (int i = 0; i < 32; i++)
   {
       std::cerr << "Register " << i << " has value " << std::dec <<reg[i] << std::endl;
   }
   std::cerr << "hi is " << reg_hi <<'\n';
   std::cerr << "lo is " << reg_lo <<'\n';


    //finish: no more instruction or out of range
    if (pc == IMEM_OFFSET + size_infile || pc == 0)
    {
        pc = 0;//set pc to zero
        std::cerr << "entering exit place with R2 value: "  <<reg[2] << std::endl;
        // std::cerr << "exitcode is " << exitcode <<'\n';
        std::exit (reg[2] & 0xff);//no more instructions; return code given by low 8-bits of register 2
    }
    else
    {
        std::exit(-11);//pc try to access out of range
    }
}
