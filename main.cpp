#include <iostream>
#include <string>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
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

  std::vector<char> imem; //set up instruction memory

    imem.resize(IMEM_LENGTH);// bianry instruction memory is saved in byte address
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
  //for(int i = 0; i < size_infile; i++){
    //std::cout << std::bitset<8> (imem[i]) << '\n';
  //}
  if(imem.size() > IMEM_LENGTH)
  {
      std::exit(-11);//喵喵喵: what to do with those in range (ask)
  }

    uint32_t ADDR_GETC;
    uint32_t ADDR_PUTC;
  std::vector<uint32_t> dmem(DMEM_LENGTH, 0); // initilise the length, also content to zero!

  std::vector<uint32_t> reg(32,0);
    uint32_t reg_lo;
    uint32_t reg_hi;

    reg[1] = 3;
    reg[2] = 3;
    reg[5] = 5;
    reg[6] = 2;
    reg[4] = 0x20000000;
    reg[8] = 20;
    reg[9] = 0x20000000;

  uint32_t pc = IMEM_OFFSET; // let pc first point to first instruction in instruction memory


  simulator s;//set up class simulator;

  bool delay = false;//branch delay slot bool default to FALSE;

    //////////////////////STAFF ONLY         SIMULATOR RUNNING HERE!!!
    //execute instructions

    while (pc < (IMEM_OFFSET + size_infile) && pc >= IMEM_OFFSET)
    {
        uint32_t instruction = 0;
        for (int i = 0; i < 4; i++)
        {
            //std::cout<< i << ":" << std::bitset<8> (imem[pc - IMEM_OFFSET + i]) << '\n';
            uint32_t mem_tmp = imem[pc - IMEM_OFFSET + i];
            mem_tmp = mem_tmp & 0b11111111;
            instruction = instruction | mem_tmp;//imem is 8 bit
            if (i != 3)
            {
                instruction = instruction  << 8;
            }
        }//load char into 32 bit instruction


        s.run(instruction, pc, reg, reg_hi, reg_lo, delay,dmem);

        if(!delay)
        {
          pc = pc + 4;//if pc is changed due to J or Branch, pc = desired address -4 and it will be restore to correct address here;
        }
        else//delay is true
        {
          pc = pc - 4 ;//go back to execute branch or
        }


    }

    //std::cout << "pc is " << std::hex <<pc << std::endl;
    for (int i = 0; i < 32; i++)
    {
        //std::cout << "Register " << i << " has value " << reg[i] << std::endl;
    }
    //std::cout << "hi is " << reg_hi <<'\n';
    //std::cout << "lo is " << reg_lo <<'\n';


    //finish: no more instruction or out of range
    if (pc == IMEM_OFFSET + size_infile)
    {
        pc = 0;//set pc to zero
        //std::cout << "pc reaches the end" << '\n';
        std::exit (reg[2] & 0xFF);//no more instructions; return code given by low 8-bits of register 2

    }
    else
    {
        std::exit(-11);//pc try to access out of range
    }






}