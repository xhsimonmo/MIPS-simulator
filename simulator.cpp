#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include <vector>
#include <bitset>

#include "simulator.hpp"

#define IMEM_OFFSET 0x10000000//head of instruction memory
#define IMEM_LENGTH 0x1000000
#define DMEM_OFFSET 0x20000000
#define DMEM_LENGTH 0x4000000
#define ADDR_GETC 0x30000000
#define ADDR_PUTC 0x30000004

void simulator::run(uint32_t instruction, uint32_t &pc, uint32_t& tmp_pc, std::vector<uint32_t> &reg, uint32_t &reg_lo, uint32_t &reg_hi, bool& delay,std::vector<uint8_t>& dmem, std::vector<char>& imem)
{


    int rs = ((instruction << 6) >> 27);
    std::cerr << "rs = " << rs <<" with current value "<< reg[rs]<<'\n';
    int rt = ((instruction << 11) >> 27);
    std::cerr << "rt = " << rt <<" with current value "<< reg[rt]<<'\n';
    int rd = ((instruction << 16) >> 27);
    std::cerr << "rd = " << rd <<" with current value "<< reg[rd]<<'\n';
    int sa = ((instruction << 21) >> 27);

    uint32_t imm = instruction & 0b00000000000000001111111111111111; //32 bits immediate, 16 0 bits and 16 immediate bits
    uint32_t address = ((instruction << 6 ) >> 6) & 0b0000001111111111111111111111111;

    int32_t sign_imm = ((instruction << 16) >> 31);
    if(sign_imm == 0)
    {
      sign_imm = instruction & 0b00000000000000001111111111111111;//upper 16 bits 0s
    }
    else
    {
      sign_imm = instruction | 0b11111111111111110000000000000000;//upper 16 bits 1s
    }



    switch (instruction >> 26)
    {
        //R type
        case 0b000000:
            R_type(instruction, pc,tmp_pc, reg, rs, rt, rd,sa, reg_lo, reg_hi, delay);
            break;
        //I type
        case 0b001000:
            addi(rt, rs, sign_imm,reg);
            break;
        case 0b001001:
            addiu( rt, rs,sign_imm,reg);
            break;
        case 0b001100:
            andi( rt, rs, imm, reg);
            break;
        case 0b000100:
            beq(rt,  rs,sign_imm,pc,tmp_pc, reg, delay);
            break;

        case 0b000001: // can be either bgez,bgezal.bltz,bltzal,
          if(rt == 0b00001){
            bgez( rs, sign_imm,  pc, tmp_pc,reg, delay);
            break;
          }
          if(rt == 0b10001){
            bgezal(rs, sign_imm,  pc,tmp_pc, reg, delay);
            break;
          }
          if(rt == 0b00000){
            bltz(rs, sign_imm,  pc,tmp_pc, reg, delay);
            break;
          }
          if(rt == 0b10000){
              bltzal(rs, sign_imm,  pc,tmp_pc, reg, delay);
            break;
          }

       case 0b000111:
           bgtz(rs, sign_imm,  pc,tmp_pc, reg, delay);
           break;
       case 0b000110:
           blez(rs, sign_imm,  pc,tmp_pc, reg, delay);
           break;
       case 0b000101:
           bne( rt,  rs, sign_imm, pc,tmp_pc, reg,delay);
           break;
       case 0b100000:
           lb(rs, rt,sign_imm, reg, dmem, imem);
           break;
        case 0b100100:
           lbu(rs,rt,sign_imm, reg,  dmem, imem);
           break;
       case 0b100001:
           lh(rs,rt,sign_imm, reg,  dmem, imem);
           break;
       case 0b100101:
           lhu(rs,rt,sign_imm, reg,  dmem, imem);
           break;
       case 0b001111:
           lui(rt,imm,reg);
           break;
       case 0b100011:
           lw(rs,rt,sign_imm, reg,  dmem, imem);
           break;
       case 0b100010:
            lwl(rs,rt,sign_imm, reg,  dmem, imem);
            break;
       case 0b100110:
            lwr(rs,rt,sign_imm, reg,  dmem, imem);
            break;
       case 0b001101:
           ori(rs, rt, imm, reg);
           break;
       case 0b101000:
           sb(rs, rt,sign_imm, reg, dmem);
           break;
       case 0b001010:
           slti(rs,rt,sign_imm,reg);
           break;
       case 0b001011:
           sltiu(rs,rt,sign_imm,reg);
           break;
       case 0b101001:
           sh(rs, rt,sign_imm, reg, dmem);
           break;
       case 0b101011:
           sw(rs, rt,sign_imm, reg, dmem);
           break;
       case 0b001110:
           xori(rs,rt,imm,reg);
           break;
        //J type
        case 0b000010:
            j(address, pc, delay);
            break;
       case 0b000011:
           jal(address, pc, reg, delay);
           break;

        std::exit(-12);//invalid instruction
    }
}

void simulator::R_type(uint32_t instruction, uint32_t &pc, uint32_t& tmp_pc, std::vector<uint32_t> &reg, int rs, int rt, int rd,int sa, uint32_t &reg_lo, uint32_t &reg_hi, bool& delay)
{
    switch ((instruction << 26)>>26)
    {
        case 0b100000:
            add(rd, rs, rt, reg);
            break;
        case 0b100001:
            addu(rd, rs, rt, reg);
            break;
        case 0b100100:
            and_bits(rd, rs, rt, reg);
            break;
       case 0b011010:
            div_f(rt, rs, reg_lo, reg_hi, reg);
           break;
       case 0b011011:
            divu(rt,  rs,  reg_lo, reg_hi, reg);
            break;
       case 0b001001:
           jalr(rs, rd, pc, tmp_pc, reg, delay);
           break;
        case 0b001000:
            jr( rs, pc, tmp_pc, reg, delay);
            break;
        case 0b010000:
            mfhi(rd, reg, reg_hi);
            break;
        case 0b010010:
            mflo(rd, reg, reg_lo);
            break;
        case 0b010001:
            mthi(rs, reg, reg_hi);
            break;
        case 0b010011:
            mtlo(rs, reg, reg_lo);
            break;
        case 0b011000:
            mult(rt, rs, reg_lo, reg_hi, reg);
            break;
        case 0b011001:
            multu(rt, rs, reg_lo, reg_hi, reg);
            break;
        case 0b100111:
            nor(rd, rs, rt, reg);
            break;
        case 0b100101:
            or_bits(rd, rs, rt, reg);
            break;
        case 0b000000:
            sll(rt,rd,sa,reg);
            break;
       case 0b000100:
           sllv(rd,rs,rt,reg);
           break;
       case 0b101010:
           slt(rd,rs,rt,reg);
           break;
       case 0b101011:
           sltu(rd,rs,rt,reg);
           break;
       case 0b000011:
           sra(rt,rd,sa,reg);
           break;
       case 0b000111:
           srav(rd,rs,rt,reg);
           break;
       case 0b000010:
           srl(rt,rd,sa,reg);
           break;
       case 0b000110:
           srlv(rd,rs,rt,reg);
           break;
       case 0b100010:
           sub(rd, rs, rt, reg);
           break;
       case 0b100011:
           subu(rd, rs, rt, reg);
           break;
       case 0b100110:
           xor_bits(rd, rs, rt, reg);
           break;
    }
}

int32_t simulator::get_char()
{
  // int input;
  // std::cin >> input;
  // if(std::cin.eof())//emptyfile, or can't read , or press ctr + D
  // {
  //   return 0b11111111;//decimal -1
  // }
  // while(!std::cin.eof())//if eof (ctrl + D/Z) isn't reached;
  // {
  //   std::cin >> input;
  // }
  // input = input & 0xff;//"extend 8 bit to 32 bit"; keep upper 24 bits zeroes
  // return input;
 int32_t input = std::getchar();
 if(input == EOF)
 {
   return(-1);
 }
 else
 {
   return input;
 }
};

void simulator::putchar(uint32_t data_write)
{
  std::putchar(data_write);//output the WORD as bits;
                                  // other size of characters(not starting at 0x30000004) is undefined behaviour, returns 0
}



void simulator::add(int rd, int rs, int rt, std::vector<uint32_t> &reg)//rs, rt are source reg indexs, rd is destination reg index
{
    std::cerr << "ADD" << std::endl;
    //rd = rs + rt
    int32_t reg_rs = reg[rs];
    int32_t reg_rt = reg[rt];
    int32_t temp = reg_rs + reg_rt;
    //check overflow
    if ((reg_rs > 0 && reg_rt > 0 && temp <= 0)||(reg_rs < 0 && reg_rt < 0 && temp >= 0))
    {
        std::exit (-10);
    }
    reg[rd] = 0b0;
    reg[rd] = reg[rd] | temp;//use bit operation to avoid conversion from sign to unsign
    std::cerr << "reg[rs]: " << reg[rs] <<  "reg[rt]: " << reg[rt] << "get result " << reg[rd] << std::endl;
}


void simulator::addi(int rt, int rs, int32_t sign_imm,std::vector<uint32_t> &reg)
{
    std::cerr << "ADDI" << '\n';
    //rt = rs + sign_imm
    int32_t reg_rs = reg[rs];
    int32_t temp = reg_rs + sign_imm;
    //check overflow
    if ((reg_rs > 0 && sign_imm > 0 && temp <= 0)||(reg_rs < 0 && sign_imm < 0 && temp >= 0))
    {
        std::exit (-10);
    }
    reg[rt] = temp;
}

void simulator::addu(int rd, int rs, int rt, std::vector<uint32_t> &reg)
{
  std::cerr << "ADDU" << '\n';
  //rd = rs + rt (unsigned)
  // don't need to check overflow
  reg[rd] = reg[rs] + reg[rt];
}

void simulator::addiu(int rt, int rs,int32_t sign_imm, std::vector<uint32_t> &reg)
{
  std::cerr << "ADDIU" << '\n';
  // rt = rs <- imm
  //sign extend immediate
  // don't need to check overflow
  uint32_t reg_rs = reg[rs];//unsigned
  uint32_t temp = reg_rs + sign_imm;
  reg[rt] = temp;
}


void simulator::sub(int rd, int rs, int rt, std::vector<uint32_t> &reg)
{
    std::cerr << "SUB" << '\n';
    //rd = rs - rt
    int32_t reg_rs = reg[rs];
    int32_t reg_rt = reg[rt];
    int32_t temp = reg_rs - reg_rt;
    if ((reg_rs < 0 && reg_rt > 0 && temp > 0) || (reg_rs >= 0 && reg_rt < 0 && temp < 0))
    {
        std::exit (-10);
    }
    reg[rd] = temp;
}

void simulator::j(uint32_t address,uint32_t& pc, bool& delay)
{
    std::cerr << "J" << '\n';
    address = address << 2;
    address = address & 0b00001111111111111111111111111100;//to ensure cpp doesn't do weird things to sign extend my variable
    uint32_t temp_pc = ((pc+4) & 0xF0000000) | address; //the low-order two bits become "00",four bits come from the high-order four bits in the PC. then concadate them.
    if(!delay)//first enter j
    {
      pc = pc + 8;
        delay = true;
    }
    else
    {
      pc = temp_pc - 4; // simply jump PC to address in rs reg
      delay = false;//set bool back
    }
}

void simulator::jal(uint32_t address,uint32_t& pc, std::vector<uint32_t> &reg, bool& delay)
{
  std::cerr << "JAL" << '\n';
  address = address << 2;
  address = address & 0b00001111111111111111111111111100;//to ensure cpp doesn't do weird things to sign extend my variable
  uint32_t temp_pc = ((pc+4) & 0xF0000000) | address; //the low-order two bits become "00",four bits come from the high-order four bits in the PC. then concadate them.
  if(!delay)//first enter j
  {
    reg[31] = pc+8;
    pc = pc + 8;
    delay = true;
  }
  else
  {
    pc = temp_pc - 4; // simply jump PC to address in rs reg
    delay = false;//set bool back
  }
}

void simulator::jalr(int rs, int rd,uint32_t& pc, uint32_t& tmp_pc,std::vector<uint32_t> &reg, bool& delay )
{
  std::cerr << "JALR" << '\n';
  if(!delay)//first enter j
  {
    tmp_pc = reg[rs];//in case rs is changed in delay slot
    reg[rd] = pc+8;
    pc = pc + 8;
    delay = true;
  }
  else
  {
    pc = tmp_pc - 4; // simply jump PC to address in rs reg
    delay = false;//set bool back
  }
}


void simulator::jr(int rs, uint32_t& pc,uint32_t& tmp_pc,std::vector<uint32_t> &reg, bool& delay)
{
  std::cerr << "JR" << '\n';
  if(!delay)//first enter jr
  {
    tmp_pc = reg[rs];//in case rs is changed in delay slot
    std::cerr << "if case tmp pc of JR is " << tmp_pc <<'\n';
    delay = true;
    pc = pc + 8; //
  }
  else
  {
    std::cerr << "tmp pc in else case IS " << tmp_pc <<'\n';
    pc = tmp_pc - 4; // simply jump PC to address in rs reg
    std::cerr << "pc in JR in else case " << pc <<'\n';
    delay = false;//set bool back
  }
}



void simulator::and_bits(int rd, int rs, int rt, std::vector<uint32_t> &reg)
{
     std::cerr << "AND operation" << '\n';
    //rd = rs AND rt
    reg[rd] = reg[rs] & reg[rt];
}

void simulator::andi(int rt, int rs, uint32_t imm, std::vector<uint32_t> &reg)
{
    std::cerr << "ANDI" << '\n';
    reg[rt] = reg[rs] & imm;
}

void simulator::or_bits(int rd, int rs, int rt, std::vector<uint32_t> &reg)
{
   std::cerr << "OR operation" << '\n';
    //rd = rs OR rt
    reg[rd] = reg[rs] | reg[rt];
}

void simulator::ori(int rs, int rt, uint32_t imm, std::vector<uint32_t> &reg)
{
  std::cerr << "ORI" << '\n';
  reg[rt] = reg[rs] | imm;
}

void simulator::xori(int rs, int rt, uint32_t imm, std::vector<uint32_t> &reg)
{
  std::cerr << "XORI" << '\n';
  reg[rt] = reg[rs] ^ imm;
}

void simulator::beq(int rt, int rs, int32_t sign_imm, uint32_t& pc, uint32_t& tmp_pc,  std::vector<uint32_t> &reg, bool& delay)
{
    std::cerr << "BEQ" << '\n';
    if(!delay)//first enter jr
    {
        delay = true;
        if (reg[rt] == reg[rs])
        {
          sign_imm = (sign_imm << 2);
          tmp_pc = pc + 4;
          tmp_pc += sign_imm;
        }
        else
        {
          tmp_pc = pc + 8;
        }
        pc = pc + 8;
    }
    else
    {
        delay = false;
        pc = tmp_pc - 4;
    }
}

void simulator::bgez(int rs, int32_t sign_imm, uint32_t& pc, uint32_t& tmp_pc, std::vector<uint32_t> &reg, bool& delay)
{
    std::cerr << "BGEZ" << '\n';
    if(!delay)
    {
        delay = true;
        if((reg[rs] >> 31) == 0)//check msb
        {
          sign_imm = (sign_imm << 2);
          tmp_pc = pc + 4;
          tmp_pc += sign_imm;
        }
        else
        {
          tmp_pc = pc + 8;
        }
        pc = pc + 8; //
    }
    else
    {
        delay = false;
        pc = tmp_pc - 4;
    }
}

void simulator::bgezal(int rs, int32_t sign_imm, uint32_t& pc, uint32_t& tmp_pc, std::vector<uint32_t> &reg, bool& delay)
{
  std::cerr << "BGEZAL" << '\n';
    if(!delay)
    {
        reg[31] = pc + 8;
        delay = true;
        if((reg[rs] >> 31) == 0)//check msb
        {
          sign_imm = (sign_imm << 2);
          tmp_pc = pc + 4;
          tmp_pc += sign_imm;
        }
        else
        {
          tmp_pc = pc + 8;
        }
        pc = pc + 8;
    }
    else
    {
        delay = false;
        pc = tmp_pc - 4;
    }
}

void simulator::bgtz(int rs, int32_t sign_imm, uint32_t& pc, uint32_t& tmp_pc, std::vector<uint32_t> &reg, bool& delay)
{
    std::cerr << "BGTZ" << '\n';
    if(!delay)
    {
        delay = true;
        if((reg[rs] >> 31) == 0 and reg[rs] != 0)//check msb and not 0
        {
          sign_imm = (sign_imm << 2);
          tmp_pc = pc + 4;
          tmp_pc += sign_imm;
        }
        else
        {
          tmp_pc = pc + 8;
        }
        pc = pc + 8; //
    }
    else
    {
        delay = false;
        pc = tmp_pc - 4;
    }
}

void simulator::blez(int rs, int32_t sign_imm, uint32_t& pc, uint32_t& tmp_pc, std::vector<uint32_t> &reg, bool& delay)
{
   std::cerr << "BLEZ" << '\n';
    if(!delay)
    {
        delay = true;
        if((reg[rs] >> 31) == 1 | reg[rs] == 0)//check msb and not 0
        {
          sign_imm = (sign_imm << 2);
          tmp_pc = pc + 4;
          tmp_pc += sign_imm;
        }
        else
        {
          tmp_pc = pc + 8;
        }
        pc = pc + 8; //
    }
    else
    {
        delay = false;
        pc = tmp_pc -4;
    }
}

void simulator::bltz(int rs, int32_t sign_imm, uint32_t& pc, uint32_t& tmp_pc,  std::vector<uint32_t> &reg, bool& delay)
{
  std::cerr << "BLTZ" << '\n';
    if(!delay)
    {
        delay = true;
        if((reg[rs] >> 31) == 1)
        {
          sign_imm = (sign_imm << 2);
          tmp_pc = pc + 4;
          tmp_pc += sign_imm;
        }
        else
        {
          tmp_pc = pc + 8;
        }

        pc = pc + 8; //
    }
    else
    {
        delay = false;
        pc = tmp_pc -4;
    }
}

void simulator::bltzal(int rs, int32_t sign_imm, uint32_t& pc, uint32_t& tmp_pc, std::vector<uint32_t> &reg, bool& delay)
{
  std::cerr << "BLTZAL" << '\n';
    reg[31] = pc + 8;
    if(!delay)
    {
        delay = true;
        if((reg[rs] >> 31) == 1)
        {
          sign_imm = (sign_imm << 2);
          tmp_pc = pc + 4;
          tmp_pc += sign_imm;
        }
        else
        {
          tmp_pc = pc + 8;
        }

        pc = pc + 8; //
    }
    else
    {
        delay = false;
        pc = tmp_pc -4;
    }
}

void simulator::bne(int rt, int rs, int32_t sign_imm, uint32_t& pc, uint32_t& tmp_pc, std::vector<uint32_t> &reg, bool& delay)
{
    std::cerr << "BNE" << '\n';
    if(!delay)//first enter jr
    {
      if(reg[rt] != reg[rs])
      {
        sign_imm = (sign_imm << 2);
        tmp_pc = pc + 4;
        tmp_pc += sign_imm;
      }
        delay = true;
        pc = pc + 8;
    }
    else
    {
        delay = false;
        pc = tmp_pc -4;
    }
}

void simulator::div_f(int rt, int rs, uint32_t &reg_lo, uint32_t &reg_hi, std::vector<uint32_t> &reg)
{
    std::cerr << "DIV" << '\n';
    int tmp_rs = reg[rs] | 0b0;
    int tmp_rt = reg[rt] | 0b0;//load exact bits
    if (reg[rt] != 0)
    {
      reg_lo = (tmp_rs/tmp_rt) | 0b0;
      reg_hi = (tmp_rs%tmp_rt) | 0b0;
        // reg_lo = reg[rs]/reg[rt];
        // reg_hi = reg[rs]%reg[rt];
    }
}

void simulator::divu(int rt, int rs, uint32_t &reg_lo, uint32_t &reg_hi, std::vector<uint32_t> &reg)
{
  std::cerr << "DIVU" << '\n';
    if (reg[rs] != 0)
    {
        reg_lo = reg[rs]/reg[rt];
        reg_hi = reg[rs]%reg[rt];
    }
}

void simulator::lb(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem, std::vector<char>& imem)
{
  std::cerr << "LB" << '\n';
  uint32_t base = 0;
  base = base | reg[rs];
  int address = base + sign_imm;
  std::cerr << "address is " << std::hex << address <<'\n';
  //std::cout << std::hex<<address << '\n';
  if((address >= DMEM_OFFSET and address < DMEM_OFFSET + DMEM_LENGTH) | (address >= IMEM_OFFSET and address < IMEM_OFFSET + IMEM_OFFSET))
  {
    if(address >= DMEM_OFFSET and address < DMEM_OFFSET + DMEM_LENGTH)
    {
      uint32_t word = ((dmem[address - DMEM_OFFSET] << 24)>>24);
      //std::cout << "the first word is " << std::hex << word<<'\n';
      uint8_t byte = (dmem[address - DMEM_OFFSET] >> 7);

      //std::cout << " the byte in lb is "<< std::bitset<8>(byte) << '\n';

      if(byte == 0)
      {
        word = word & 0x000000FF;//upper 24 bits 0s
      }
      else
      {
        word = word | 0xFFFFFF00;//upper 24 bits 1s
      }
      //std::cout << "the word to load in lb is " << std::hex << word <<'\n';
      reg[rt] = word;//load content in data memory to register. potential bug in sign/unsign convert?
    }

    else
    {
      uint32_t word = ((imem[address - IMEM_OFFSET] << 24)>>24);
      //std::cout << "the first word is " << std::hex << word<<'\n';
      uint8_t byte = (imem[address - IMEM_OFFSET] >> 7);

      //std::cout << " the byte in lb is "<< std::bitset<8>(byte) << '\n';
      if(byte == 0)
      {
        word = word & 0x000000FF;//upper 24 bits 0s
      }
      else
      {
        word = word | 0xFFFFFF00;//upper 24 bits 1s
      }
      //std::cout << "the word to load in lb is " << std::hex << word <<'\n';
      reg[rt] = word;//load content in data memory to register. potential bug in sign/unsign convert?
    }

  }
  else{

    if(address >= ADDR_GETC and address < ADDR_GETC + 4){
      reg[rt] = get_char();
    //  std::cout << "register " << rt <<" is "<< reg[rt] <<'\n';
    }
    else{
      std::exit(-11);//out of range of data mem
    }
  }
}

void simulator::lbu(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem, std::vector<char>& imem)
{
  std::cerr << "LBU" << '\n';
  uint32_t base = 0;
  base = base | reg[rs];
  int address = base + sign_imm;
  //std::cout << std::hex<<address << '\n';
  if((address >= DMEM_OFFSET and address < DMEM_OFFSET + DMEM_LENGTH ) | (address >= IMEM_OFFSET and address < IMEM_OFFSET + IMEM_OFFSET ))
  {
    if(address >= DMEM_OFFSET and address < DMEM_OFFSET + DMEM_LENGTH)
    {
      uint32_t word = ((dmem[address - DMEM_OFFSET] <<24)>>24);
      word = word & 0x000000FF;//zero extend
      //std::cout << "the word to load in lbu is " << std::hex << word <<'\n';
      reg[rt] = word;//load content in data memory to register. potential bug in sign/unsign convert?
    }
    else
    {
      uint32_t word = ((imem[address - IMEM_OFFSET] <<24)>>24);
      word = word & 0x000000FF;//zero extend
      //std::cout << "the word to load in lbu is " << std::hex << word <<'\n';
      reg[rt] = word;//load content in data memory to register. potential bug in sign/unsign convert?
    }

  }
  else{
    if(address == ADDR_GETC){
      reg[rt] = get_char();
      //std::cout << "register " << rt <<" is "<< reg[rt] <<'\n';
    }
    else{
      std::exit(-11);//out of range of data mem
    }
  }
}

void simulator::lh(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem, std::vector<char>& imem)
{
  std::cerr << "LH" << '\n';
  uint32_t base = 0;
  base = base | reg[rs];
  int address = base + sign_imm;
  uint32_t lsb = address & 0b1;//extract the lsb to detect address error exception
  if(lsb ==1)
  {
    std::exit(-11); //address exception
  }
  //std::cout << std::hex<<address << '\n';
  if((address >= DMEM_OFFSET and address <= DMEM_OFFSET + DMEM_LENGTH - 2) | (address >= IMEM_OFFSET and address <= IMEM_OFFSET + IMEM_OFFSET - 2 ))
  {
    if(address >= DMEM_OFFSET and address <= DMEM_OFFSET + DMEM_LENGTH - 2)
    {
      uint32_t word1 = ((dmem[address - DMEM_OFFSET] << 24)>>16);
      word1 = word1 & 0x0000FF00;
      uint32_t word2 = ((dmem[address - DMEM_OFFSET + 1] << 24)>>24);
      word2 = word2 & 0x000000FF;
      uint32_t word = word1 | word2;
      uint8_t byte = (dmem[address - DMEM_OFFSET] >> 7);
      if(byte == 0)
      {
        word = word & 0x0000FFFF;//upper 24 bits 0s
      }
      else
      {
        word = word | 0xFFFF0000;//upper 24 bits 1s
      }
      //std::cout << "the word to load in lh is " << std::hex << word <<'\n';
      reg[rt] = word;//load content in data memory to register. potential bug in sign/unsign convert?
    }
    else
    {
      uint32_t word1 = ((imem[address - IMEM_OFFSET] << 24)>>16);
      word1 = word1 & 0x0000FF00;
      uint32_t word2 = ((imem[address - IMEM_OFFSET + 1] << 24)>>24);
      word2 = word2 & 0x000000FF;
      uint32_t word = word1 | word2;
      uint8_t byte = (imem[address - IMEM_OFFSET] >> 7);
      if(byte == 0)
      {
        word = word & 0x0000FFFF;//upper 24 bits 0s
      }
      else
      {
        word = word | 0xFFFF0000;//upper 24 bits 1s
      }
      //std::cout << "the word to load in lh is " << std::hex << word <<'\n';
      reg[rt] = word;//load content in data memory to register. potential bug in sign/unsign convert?
    }
  }
  else{

    if(address == ADDR_GETC){
      reg[rt] = get_char();
      //std::cout << "register " << rt <<" after calling getchar is "<< reg[rt] <<'\n';
    }
    else{
      std::exit(-11);//out of range of data mem
    }
  }
}

void simulator::lhu(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem, std::vector<char>& imem)
{
  std::cerr << "LHU" << '\n';
  uint32_t base = 0;
  base = base | reg[rs];
  int address = base + sign_imm;
  std::cerr << "address is " << std::hex << address <<'\n';
  uint32_t lsb = address & 0b1;//extract the lsb to detect address error exception
  if(lsb ==1)
  {
    std::exit(-11); //address exception
  }

  if((address >= DMEM_OFFSET and address <= DMEM_OFFSET + DMEM_LENGTH - 2) | (address >= IMEM_OFFSET and address <= IMEM_OFFSET + IMEM_OFFSET - 2))
  {
    if(address >= DMEM_OFFSET and address <= DMEM_OFFSET + DMEM_LENGTH - 2)
    {
      uint32_t word1 = ((dmem[address - DMEM_OFFSET] << 24)>>16);
      word1 = word1 & 0x0000FF00;
      uint32_t word2 = ((dmem[address - DMEM_OFFSET + 1] << 24)>>24);
      word2 = word2 & 0x000000FF;
      uint32_t word = word1 | word2;
      //std::cout << "the word to load in lh is " << std::hex << word <<'\n';
      reg[rt] = word;//load content in data memory to register. potential bug in sign/unsign convert?
    }
    else
    {
      uint32_t word1 = ((imem[address - IMEM_OFFSET] << 24)>>16);
      word1 = word1 & 0x0000FF00;
      uint32_t word2 = ((imem[address - IMEM_OFFSET + 1] << 24)>>24);
      word2 = word2 & 0x000000FF;
      uint32_t word = word1 | word2;
      //std::cout << "the word to load in lh is " << std::hex << word <<'\n';
      reg[rt] = word;//load content in data memory to register. potential bug in sign/unsign convert?
    }
  }
  else{

    if(address == ADDR_GETC){
      reg[rt] = get_char();
      //std::cout << "register " << rt <<" after calling getchar is "<< reg[rt] <<'\n';
    }
    else{
      std::exit(-11);//out of range of data mem
    }
  }
}


void simulator::lw(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem, std::vector<char>& imem)
{
  std::cerr << "LW" << '\n';
  uint32_t base = 0;
  base = base | reg[rs];
  int address = base + sign_imm;
  std::cerr << "address is " << std::hex << address <<'\n';
  uint32_t ls2b = address & 0b11;
  if(ls2b != 0)
  {
    std::exit(-11);
  }

  if((address >= DMEM_OFFSET and address <= DMEM_OFFSET + DMEM_LENGTH - 4) | (address >= IMEM_OFFSET and address <= IMEM_OFFSET + IMEM_OFFSET - 4))
  {
    if(address >= DMEM_OFFSET and address <= DMEM_OFFSET + DMEM_LENGTH - 4)
    {
      uint32_t word1 = dmem[address - DMEM_OFFSET] << 24;
      word1 = word1 & 0xFF000000;
      uint32_t word2 = ((dmem[address - DMEM_OFFSET + 1] << 24)>>8);
      word2 = word2 & 0x00FF0000;
      uint32_t word3 = ((dmem[address - DMEM_OFFSET + 2] << 24)>>16);
      word3 = word3 & 0x0000FF00;
      uint32_t word4 = ((dmem[address - DMEM_OFFSET + 3] << 24)>>24);
      word4 = word4 & 0x000000FF;
      uint32_t word = word1 | word2 | word3 | word4;
      //std::cout << "the word to load in lw is " << std::hex << word <<'\n';
      reg[rt] = word;//load content in data memory to register. potential bug in sign/unsign convert?
    }
    else
    {
      uint32_t word1 = imem[address - IMEM_OFFSET] << 24;
      word1 = word1 & 0xFF000000;
      uint32_t word2 = ((imem[address - IMEM_OFFSET + 1] << 24)>>8);
      word2 = word2 & 0x00FF0000;
      uint32_t word3 = ((imem[address - IMEM_OFFSET + 2] << 24)>>16);
      word3 = word3 & 0x0000FF00;
      uint32_t word4 = ((imem[address - IMEM_OFFSET + 3] << 24)>>24);
      word4 = word4 & 0x000000FF;
      uint32_t word = word1 | word2 | word3 | word4;
      //std::cout << "the word to load in lw is " << std::hex << word <<'\n';
      reg[rt] = word;//load content in data memory to register. potential bug in sign/unsign convert?
    }
  }
  else{
    if(address == ADDR_GETC){
      std::cerr << "enter get char condition, wait for in char" << '\n';
      reg[rt] = get_char();
      std::cerr << "successfully fetch char" << '\n';
      //std::cout << "register " << rt <<" is "<< reg[rt] <<'\n';
    }
    else{
      std::exit(-11);//out of range of data mem
    }
  }
}

void simulator::lwl(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem, std::vector<char>& imem)
{
  std::cerr << "LWL" << '\n';
  uint32_t base = 0;
  base = base | reg[rs];
  int address = base + sign_imm;
  //std::cout << std::hex<<address << '\n';
  if((address >= DMEM_OFFSET and address <= DMEM_OFFSET + DMEM_LENGTH - 4) | (address >= IMEM_OFFSET and address <= IMEM_OFFSET + IMEM_OFFSET))
  {
    if(address >= DMEM_OFFSET and address <= DMEM_OFFSET + DMEM_LENGTH - 4)
    {
      uint32_t word1 = dmem[address - DMEM_OFFSET] << 24;
      word1 = word1 & 0xFF000000;
      uint32_t word2 = ((dmem[address - DMEM_OFFSET + 1] << 24)>>8);
      word2 = word2 & 0x00FF0000;
      uint32_t word = word1 | word2;//the most significant half word
      reg[rt] = reg[rt] & 0x0000FFFF;//set upper 16 bits to 0s
      reg[rt] = word | reg[rt];//load content in data memory to register. potential bug in sign/unsign convert?
    }

    else
    {
      uint32_t word1 = imem[address - IMEM_OFFSET] << 24;
      word1 = word1 & 0xFF000000;
      uint32_t word2 = ((imem[address - IMEM_OFFSET + 1] << 24)>>8);
      word2 = word2 & 0x00FF0000;
      uint32_t word = word1 | word2;//the most significant half word
      reg[rt] = reg[rt] & 0x0000FFFF;//set upper 16 bits to 0s
      reg[rt] = word | reg[rt];//load content in data memory to register. potential bug in sign/unsign convert?
    }
  }
  else{
    if(address == ADDR_GETC){
      reg[rt] = get_char();
      //std::cout << "register " << rt <<" is "<< reg[rt] <<'\n';
    }
    else{
      std::exit(-11);//out of range of data mem
    }
  }
}

void simulator::lwr(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem, std::vector<char>& imem)
{
  std::cerr << "LWR" << '\n';
  uint32_t base = 0;
  base = base | reg[rs];
  int address = base + sign_imm;
  //std::cout << std::hex<<address << '\n';
  if((address >= DMEM_OFFSET and address < DMEM_OFFSET + DMEM_LENGTH) | (address >= IMEM_OFFSET and address < IMEM_OFFSET + IMEM_OFFSET))//slight difference in memo range
  {
    if(address >= DMEM_OFFSET and address < DMEM_OFFSET + DMEM_LENGTH)
    {
      uint32_t word1 = ((dmem[address - DMEM_OFFSET - 1] << 24)>>16);
      word1 = word1 & 0x0000FF00;
      uint32_t word2 = ((dmem[address - DMEM_OFFSET ] << 24)>>24);
      word2 = word2 & 0x000000FF;
      uint32_t word = word1 | word2;//the most significant half word
      reg[rt] = reg[rt] & 0xFFFF0000;//set upper 16 bits to 0s
      reg[rt] = word | reg[rt];//load content in data memory to register. potential bug in sign/unsign convert?
    }
    else
    {
      uint32_t word1 = ((imem[address - IMEM_OFFSET - 1] << 24)>>16);
      word1 = word1 & 0x0000FF00;
      uint32_t word2 = ((imem[address - IMEM_OFFSET ] << 24)>>24);
      word2 = word2 & 0x000000FF;
      uint32_t word = word1 | word2;//the most significant half word
      reg[rt] = reg[rt] & 0xFFFF0000;//set upper 16 bits to 0s
      reg[rt] = word | reg[rt];//load content in data memory to register. potential bug in sign/unsign convert?
    }
  }
  else{
    if(address == ADDR_GETC){
      reg[rt] = get_char();
      //std::cout << "register " << rt <<" is "<< reg[rt] <<'\n';
    }
    else{
      std::exit(-11);//out of range of data mem
    }
  }
}


void simulator::lui(int rt, uint32_t imm,std::vector<uint32_t> &reg)
{
  std::cerr << "LUI" << '\n';
  imm = (imm << 16) & 0xFFFF0000;
  reg[rt] = imm;
}

void simulator::sb(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem)
{
  std::cerr << "SB" << '\n';
  int base = 0;
  base = base | reg[rs];
  int address = base + sign_imm;
  std::cerr << "address is " << std::hex << address <<'\n';
  if((address >= DMEM_OFFSET) && (address < DMEM_OFFSET + DMEM_LENGTH))
  {
   uint8_t byte = reg[rt] & 0x000000FF;//only keep lowest 8 bits
   dmem[address - DMEM_OFFSET] = byte;
  }
  else{
    if(address >= ADDR_PUTC && address < ADDR_PUTC + 4)
    {
      putchar(reg[rt]);
    }
    else{
      std::exit(-11);
    }
  }
}

void simulator::slti(int rs,int rt,int32_t sign_imm,std::vector<uint32_t> &reg)
{
  std::cerr << "SLTI" << '\n';
  int temp = reg[rs];//convert unsigned register value to signed

  if(temp < sign_imm)
  {
    reg[rt] = 1;
  }
  else{
    reg[rt] = 0;
  }
}

void simulator::sltiu(int rs,int rt,int32_t sign_imm,std::vector<uint32_t> &reg)
{
  std::cerr << "SLTIU" << '\n';
  uint32_t unsign_imm = sign_imm;
  if(reg[rs] < unsign_imm)
  {
    reg[rt] = 1;
  }
  else{
    reg[rt] = 0;
  }
}

void simulator::sh(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem)
{
  std::cerr << "SH" << '\n';
  int base = 0;
  base = base | reg[rs];
  int address = base + sign_imm;
  std::cerr << "address is " << std::hex << address <<'\n';
  uint32_t lsb = address & 0b1;//extract the lsb to detect address error exception
  if(lsb != 0)
  {
    std::exit(-11); //address exception
  }
  if((address >= DMEM_OFFSET) && (address <= DMEM_OFFSET + DMEM_LENGTH - 2 ))
  {
   uint32_t word = reg[rt];
   uint8_t byte = word & 0x000000FF;//only keep lowest 8 bits
   dmem[address - DMEM_OFFSET + 1] = byte;
   byte = (word >> 8) & 0x000000FF;
   dmem[address - DMEM_OFFSET] = byte;
  }
  else{
    if(address == ADDR_PUTC)
    {
      putchar(reg[rt]);
    }
    else{
      std::exit(-11);
    }
  }
}

void simulator::sw(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem)//the lower order 2 bits have to be zero otherwise it is undefined
{
  std::cerr << "SW" << '\n';
  int base = 0;
  base = base | reg[rs];
  int address = base + sign_imm;
  std::cerr << "address is " << std::hex << address <<'\n';
  uint32_t ls2b = address & 0b11;
  if(ls2b != 0)
  {
    std::exit(-11);
  }

  if((address >= DMEM_OFFSET) && (address <= DMEM_OFFSET + DMEM_LENGTH - 4))
  {
   uint32_t word = reg[rt];
   uint8_t byte = word & 0x000000FF;//only keep lowest 8 bits
   dmem[address - DMEM_OFFSET + 3] = byte;
   byte = (word >> 8) & 0x000000FF;
   dmem[address - DMEM_OFFSET + 2] = byte;
   byte = (word >> 16) & 0x000000FF;
   dmem[address - DMEM_OFFSET + 1] = byte;
   byte = (word >> 24) & 0x000000FF;
   dmem[address - DMEM_OFFSET] = byte;
  }
  else{
    if(address == ADDR_PUTC)
    {
      putchar(reg[rt]);
    }
    else{
      std::exit(-11);
    }
  }
}

void simulator::sll(int rt,int rd,int sa, std::vector<uint32_t> &reg)
{
  std::cerr << "SLL" << '\n';
  uint32_t tmp = reg[rt];
  tmp = tmp << sa;
  reg[rd] = tmp;
}

void simulator::sllv(int rd,int rs,int rt, std::vector<uint32_t> &reg)
{
  std::cerr << "SLLV" << '\n';
  uint32_t shift_amount = reg[rs];
  shift_amount = shift_amount & 0x0000001F;//extract lower 5 bits
  uint32_t tmp = reg[rt];
  tmp = tmp << shift_amount;
  reg[rd] = tmp;
}

void simulator::slt(int rd, int rs, int rt, std::vector<uint32_t> &reg)
{
  std::cerr << "SLT" << '\n';
  int tmp_rs = reg[rs], tmp_rt = reg[rt];
  if(tmp_rs < tmp_rt)
  {
    reg[rd] = 1;
  }
  else{
    reg[rd] = 0;
  }
}

void simulator::sltu(int rd, int rs, int rt, std::vector<uint32_t> &reg)
{
  std::cerr << "SLTU" << '\n';
  if(reg[rs] < reg[rt])
  {
    reg[rd] = 1;
  }
  else{
    reg[rd] = 0;
  }
}

void simulator::sra(int rt,int rd,int sa, std::vector<uint32_t> &reg)
{
  std::cerr << "SRA" << '\n';
  signed int tmp = reg[rt];//make sure this is signed value
  tmp = tmp >> sa;//should sign extend
  reg[rd] = tmp;
}

void simulator::srav(int rd,int rs,int rt, std::vector<uint32_t> &reg)
{
  std::cerr << "SRAV" << '\n';
  //rd <- rt>>rs
  uint32_t shift_amount = reg[rs];
  shift_amount = shift_amount & 0x0000001F;//extract lower 5 bits
  int tmp = 0;
  tmp = tmp | reg[rt];//load exact bit into tmp
  tmp = tmp >> shift_amount;//should be signed extended
  reg[rd] = tmp | 0;
}

void simulator::srl(int rt,int rd,int sa, std::vector<uint32_t> &reg)
{
  std::cerr << "SRL" << '\n';
  uint32_t tmp = reg[rt] >> sa;// unsigned type, extend right will be 0 extended
  reg[rd] = tmp;
}

void simulator::srlv(int rd,int rs,int rt, std::vector<uint32_t> &reg)
{
   std::cerr << "SRLV" << '\n';
   //rd <- rt>>rs
  uint32_t shift_amount = reg[rs];
  shift_amount = shift_amount & 0x0000001F;//extract lower 5 bits
  uint32_t tmp = reg[rt];
  tmp = tmp >> shift_amount;//should be signed extended
  reg[rd] = tmp;
}

void simulator::mfhi(int rd, std::vector<uint32_t> &reg, uint32_t &reg_hi)
{
  std::cerr << "MFHI" << '\n';
    reg[rd] = reg_hi;
}

void simulator::mflo(int rd, std::vector<uint32_t> &reg, uint32_t &reg_lo)
{
    std::cerr << "MFLO" << '\n';
    std::cerr << "reg lo in mflo is " << reg_lo <<'\n';
    reg[rd] = reg_lo;
}

void simulator::mthi(int rs, std::vector<uint32_t> &reg, uint32_t &reg_hi)
{
  std::cerr << "MTHI" << '\n';
    reg_hi = reg[rs];
}

void simulator::mtlo(int rs, std::vector<uint32_t> &reg, uint32_t &reg_lo)
{
    std::cerr << "MTLO" << '\n';
    reg_lo = reg[rs];
    std::cerr << "reg lo in mtlo is " << reg_lo <<'\n';
}

void simulator::mult(int rt, int rs, uint32_t &reg_lo, uint32_t &reg_hi, std::vector<uint32_t> &reg)
{
    std::cerr << "MULT" << '\n';
    int64_t temp_rt = reg[rt];
    int64_t temp_rs = reg[rs];
    int64_t temp = temp_rt * temp_rs;
    reg_lo = temp & 0xFFFFFFFF;
    reg_hi = (temp & 0xFFFFFFFF00000000) >> 32;
}

void simulator::multu(int rt, int rs, uint32_t &reg_lo, uint32_t &reg_hi, std::vector<uint32_t> &reg)
{
  std::cerr << "MULTU" << '\n';
    uint64_t temp_rt = reg[rt];
    uint64_t temp_rs = reg[rs];
    uint64_t temp = temp_rt * temp_rs;
    reg_lo = temp & 0xFFFFFFFF;
    reg_hi = (temp & 0xFFFFFFFF00000000) >> 32;
}

void simulator::nor(int rd, int rs, int rt, std::vector<uint32_t> &reg)
{
  std::cerr << "NOR" << '\n';
    reg[rd] = ~(reg[rs] | reg[rt]);
}

void simulator::subu(int rd, int rs, int rt, std::vector<uint32_t> &reg)
{
  std::cerr << "SUBU" << '\n';
    reg[rd] = reg[rs] - reg[rt];
}

void simulator::xor_bits(int rd, int rs, int rt, std::vector<uint32_t> &reg)
{
  std::cerr << "XOR" << '\n';
    reg[rd] = (reg[rs] ^ reg[rt]);
}
