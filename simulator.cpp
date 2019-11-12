#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include <vector>

#include "simulator.hpp"

#define IMEM_OFFSET 0x10000000//head of instruction memory
#define IMEM_LENGTH 0x1000000
#define DMEM_OFFSET 0x20000000
#define DMEM_LENGTH 0x4000000
#define ADDR_GETC 0x30000000
#define ADDR_PUTC 0x30000004

void simulator::run(uint32_t instruction, uint32_t &pc, std::vector<uint32_t> &reg, uint32_t &reg_lo, uint32_t &reg_hi, bool& delay,std::vector<uint32_t>& dmem)
{
    int rs = ((instruction << 6) >> 27);
    //std::cout << "rs = " << rs <<"with current value "<< reg[rs]<<'\n';
    int rt = ((instruction << 11) >> 27);
    //std::cout << "rt = " << rt <<"with current value "<< reg[rt]<<'\n';
    int rd = ((instruction << 16) >> 27);
    //std::cout << "rd = " << rd <<"with current value "<< reg[rd]<<'\n';
    uint32_t imm = instruction & 0b00000000000000001111111111111111; //32 bits immediate, 16 0 bits and 16 immediate bits
    uint32_t address = ((instruction << 6 ) >> 6) & 0b00000011111111111111111111111111;

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
            R_type(instruction, pc, reg,  rs, rt, rd, reg_lo, reg_hi, delay);
            break;
        //I type
        case 0b001000:
            addi(rt, rs, sign_imm,reg);
            break;
        case 0b001001:
            addiu( rt, rs,imm,reg);
            break;
        case 0b001100:
            andi( rt, rs, imm, reg);
            break;
        case 0b000100:
            beq( rt,  rs,sign_imm,pc, reg,  delay);
            break;

        case 0b000001: // can be either bgez,bgezal.bltz,bltzal,
          if(rt == 0b00001){
            bgez( rs, sign_imm,  pc, reg, delay);
            break;
          }
          if(rt == 0b10001){
            bgezal(rs, sign_imm,  pc, reg, delay);
            break;
          }
          if(rt == 0b00000){
            bltz(rs, sign_imm,  pc, reg, delay);
            break;
          }
          if(rt == 0b10000){
              bltzal(rs, sign_imm,  pc, reg, delay);
            break;
          }

       case 0b000111:
           bgtz(rs, sign_imm,  pc, reg, delay);
           break;
       case 0b000110:
           blez(rs, sign_imm,  pc, reg, delay);
           break;
       case 0b000101:
           bne( rt,  rs, sign_imm,  pc,reg,  delay);
           break;
       case 0b100000:
           lb(rs, rt,sign_imm, reg, dmem);
           break;
//        case 0b100100:
//            lbu
//            break;
//        case 0b100001:
//            lh
//            break;
//        case 0b100101:
//            lhu
//            break;
//        case 0b001111
//             lui
//           break;
//        case 0b100011:
//            lw
//            break;
//        case 0b100010
//             lwl
//             break;
//        case 0b100110
//             lwr
//             break;

//        case 0b001101:
//            ori;
//            break;
       case 0b101000:
           sb( rs, rt,sign_imm, reg, dmem);
           break;
//        case 0b001010:
//            slti
//            break;
//        case 0b001011:
//            sltiu
//            break;
//        case 0b101001:
//            sh
//            break;
//        case 0b101011:
//            sw
//            break;
//        case 0b001110:
//            xori
//            break;
        //J type
        case 0b000010:
            j(rs, address, pc, delay);
            break;
//        case 0b000011:
//            jal();
//            break;

        std::exit(-12);//invalid instruction
    }
}

void simulator::R_type(uint32_t instruction, uint32_t &pc, std::vector<uint32_t> &reg, int rs, int rt, int rd, uint32_t &reg_lo, uint32_t &reg_hi, bool& delay)
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
           div( rt, rs, reg_lo, reg_hi, reg);
           break;
       case 0b011011:
           divu(rt,  rs,  reg_lo, reg_hi, reg);
           break;
//        case 0b001001:
//            jalr
//            break;
        case 0b001000:
            jr( rs, pc, reg, delay);
            break;
//        case 0b010000:
//            mfhi
//            break;
//        case 0b010010:
//            mflo
//            break;
//        case 0b010001:
//            mthi
//            break;
//        case 0b010011:
//            mtlo
//            break;
//        case 0b011000:
//            mult
//            break;
//        case 0b011001:
//            multu
//            break;
//        case 0b100111:
//            nor
//            break;
        case 0b100101:
            or_bits(rd, rs, rt, reg);
            break;
//        case 0b000000:
//            sll
//            break;
//        case 0b000100:
//            sllv
//            break;
//        case 0b101010:
//            slt
//            break;
//        case 0b101011:
//            sltu
//            break;
//        case 0b000011:
//            sra
//            break;
//        case 0b000111:
//            srav
//            break;
//        case 0b000010:
//            srl
//            break;
//        case 0b000110:
//            srlv
//            break;
//        case 0b100010:
//            sub
//            break;
//        case 0b100011:
//            subu
//            break;
//        case 0b100110:
//            xor
//            break;
    }
}

int32_t simulator::get_char()
{
  int input;
  std::cin >> input;
  if(std::cin.eof())//emptyfile, or can't read , or press ctr + D
  {
    return 0b11111111;//decimal -1
  }

  while(!std::cin.eof())//if eof (ctrl + D/Z) isn't reached;
  {
    std::cin >> input;
  }
  input = input & 0xff;//"extend 8 bit to 32 bit"; keep upper 24 bits zeroes
  return input;
};

void simulator::putchar(uint32_t data_write)
{
  std::cout << data_write << '\n';//output the WORD as bits;
                                  // other size of characters(not starting at 0x30000004) is undefined behaviour, returns 0
}



void simulator::add(int rd, int rs, int rt, std::vector<uint32_t> &reg)//rs, rt are source reg indexs, rd is destination reg index
{
    //rd = rs + rt
    int32_t reg_rs = reg[rs];
    int32_t reg_rt = reg[rt];
    int32_t temp = reg_rs + reg_rt;
    //check overflow
    if ((reg_rs > 0 && reg_rt > 0 && temp < 0)||(reg_rs < 0 && reg_rt < 0 && temp > 0))
    {
        std::exit (-10);
    }
    reg[rd] = 0b0;
    reg[rd] = reg[rd] & temp;//use bit operation to avoid conversion from sign to unsign
}


void simulator::addi(int rt, int rs, int32_t sign_imm,std::vector<uint32_t> &reg)
{
    //rt = rs + sign_imm
    int32_t reg_rs = reg[rs];
    int32_t temp = reg_rs + sign_imm;
    //check overflow
    if ((reg_rs > 0 && sign_imm > 0 && temp < 0)||(reg_rs < 0 && sign_imm < 0 && temp > 0))
    {
        std::exit (-10);
    }
    reg[rt] = temp;
}

void simulator::addu(int rd, int rs, int rt, std::vector<uint32_t> &reg)
{
  //rd = rs + rt (unsigned)
  // don't need to check overflow
  reg[rd] = reg[rs] + reg[rt];
}

void simulator::addiu(int rt, int rs,uint32_t imm, std::vector<uint32_t> &reg)
{  // rt = rs <- imm
  //sign extend immediate
  // don't need to check overflow
  uint32_t reg_rs = reg[rs];//unsigned
  uint32_t temp = reg_rs + imm;
  reg[rt] = temp;
}


void simulator::sub(int rd, int rs, int rt, std::vector<uint32_t> &reg)
{
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

void simulator::j(int rs, uint32_t address,uint32_t& pc, bool& delay) // pc = R[rs], need branch delay
{   address = address << 2;
    address = address & 0b00001111111111111111111111111100;//to ensure cpp doesn't do weird things to sign extend my variable
    uint32_t temp_pc = (pc & 0xf0000000) | (address << 2); //the low-order two bits become "00",four bits come from the high-order four bits in the PC. then concadate them.
    if(!delay)//first enter jr
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

void simulator::jr(int rs, uint32_t& pc,std::vector<uint32_t> &reg, bool& delay)
{

  if(!delay)//first enter jr
  {
    delay = true;
    pc = pc + 8; //
  }
  else
  {
    pc = reg[rs] - 4; // simply jump PC to address in rs reg
    delay = false;//set bool back
  }
}

void simulator::and_bits(int rd, int rs, int rt, std::vector<uint32_t> &reg)
{
    //rd = rs AND rt
    reg[rd] = reg[rs] & reg[rt];
}

void simulator::andi(int rt, int rs, uint32_t imm, std::vector<uint32_t> &reg)
{
    reg[rt] = reg[rs] & imm;
}

void simulator::or_bits(int rd, int rs, int rt, std::vector<uint32_t> &reg)
{
    //rd = rs OR rt
    reg[rd] = reg[rs] | reg[rt];
}

void simulator::beq(int rt, int rs, uint32_t sign_imm, uint32_t& pc, std::vector<uint32_t> &reg, bool& delay)
{
    int32_t temp_pc = pc;
    temp_pc += (sign_imm << 2);
    if(!delay)//first enter jr
    {
        delay = true;
        pc = pc + 8;
    }
    else
    {
        delay = false;
        if (reg[rt] == reg[rs])
        {pc = temp_pc - 4;}
        else
        {pc = pc + 4;}
    }
}

void simulator::bgez(int rs, uint32_t sign_imm, uint32_t& pc, std::vector<uint32_t> &reg, bool& delay)
{
    int32_t temp_pc = pc;
    temp_pc += (sign_imm << 2);
    if(!delay)
    {
        delay = true;
        pc = pc + 8; //
    }
    else
    {
        delay = false;
        if (reg[rs] >= 0)
        {pc = temp_pc - 4;}
        else
        {pc = pc + 4;}
    }
}

void simulator::bgezal(int rs, uint32_t sign_imm, uint32_t& pc, std::vector<uint32_t> &reg, bool& delay)
{
    int32_t temp_pc = pc;
    temp_pc += (sign_imm << 2);
    reg[31] = pc + 8;
    if(!delay)
    {
        delay = true;
        pc = pc + 8;
    }
    else
    {
        delay = false;
        if (reg[rs] >= 0)
        {
        pc = temp_pc - 4;
        }
        else
        {pc = pc + 4;}
    }
}

void simulator::bgtz(int rs, uint32_t sign_imm, uint32_t& pc, std::vector<uint32_t> &reg, bool& delay)
{
    int32_t temp_pc = pc;
    temp_pc += (sign_imm << 2);
    if(!delay)
    {
        delay = true;
        pc = pc + 8; //
    }
    else
    {
        delay = false;
        if (reg[rs] > 0)
        {pc = temp_pc - 4;}
        else
        {pc = pc + 4;}
    }
}

void simulator::blez(int rs, uint32_t sign_imm, uint32_t& pc, std::vector<uint32_t> &reg, bool& delay)
{
    int32_t temp_pc = pc;
    temp_pc += (sign_imm << 2);
    if(!delay)
    {
        delay = true;
        pc = pc + 8; //
    }
    else
    {
        delay = false;
        if (reg[rs] <= 0)
        {pc = temp_pc - 4;}
        else
        {pc = pc + 4;}
    }
}

void simulator::bltz(int rs, uint32_t sign_imm, uint32_t& pc, std::vector<uint32_t> &reg, bool& delay)
{
    int32_t temp_pc = pc;
    temp_pc += (sign_imm << 2);
    if(!delay)
    {
        delay = true;
        pc = pc + 8; //
    }
    else
    {
        delay = false;
        if (reg[rs] < 0)
        {pc = temp_pc - 4;}
        else
        {pc = pc + 4;}
    }
}

void simulator::bltzal(int rs, uint32_t sign_imm, uint32_t& pc, std::vector<uint32_t> &reg, bool& delay)
{
    int32_t temp_pc = pc;//store return address in R31
    temp_pc += (sign_imm << 2);
    reg[31] = pc + 8;
    if(!delay)
    {
        delay = true;
        pc = pc + 8;
    }
    else
    {
        delay = false;
        if (reg[rs] < 0)
        {
        pc = temp_pc - 4;
        }
        else
        {pc = pc + 4;}
    }
}

void simulator::bne(int rt, int rs, uint32_t sign_imm, uint32_t& pc, std::vector<uint32_t> &reg, bool& delay)
{
    int32_t temp_pc = pc;
    temp_pc += (sign_imm << 2);
    if(!delay)//first enter jr
    {
        delay = true;
        pc = pc + 8;
    }
    else
    {
        delay = false;
        if (reg[rt] != reg[rs])
        {pc = temp_pc - 4;}
        else
        {pc = pc + 4;}
    }
}

void simulator::div(int rt, int rs, uint32_t &reg_lo, uint32_t &reg_hi, std::vector<uint32_t> &reg)
{
    if (reg[rt] != 0)
    {
        reg_lo = reg[rs]/reg[rt];
        reg_hi = reg[rs]%reg[rt];
    }
}

void simulator::divu(int rt, int rs, uint32_t &reg_lo, uint32_t &reg_hi, std::vector<uint32_t> &reg)
{
    if (reg[rs] != 0)
    {
        reg_lo = reg[rs]/reg[rt];
        reg_hi = reg[rs]%reg[rt];
    }
}

void simulator::lb(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint32_t>& dmem)
{
  uint32_t base = 0;
  base = base | reg[rs];
  int address = base + sign_imm;
  if(address >= DMEM_OFFSET and address <= DMEM_OFFSET + DMEM_LENGTH)
  {
    //int word = dmem[(address - DMEM_OFFSET) / 4];
    int word = (( dmem[(address - DMEM_OFFSET) / 4] << 16 ) >> 31);
    if(word == 0)
    {
      word = dmem[(address - DMEM_OFFSET) / 4] & 0b00000000000000001111111111111111;//upper 16 bits 0s
    }
    else
    {
      word = dmem[(address - DMEM_OFFSET) / 4] | 0b11111111111111110000000000000000;//upper 16 bits 1s
    }

    reg[rt] = word;//load content in data memory to register. potential bug in sign/unsign convert?
  }
  else{
    if(address == ADDR_GETC){
      reg[rt] = get_char();
    }
    else{
      std::exit(-11);//out of range of data mem
    }
  }
}

void simulator::sb(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint32_t>& dmem)
{
  int base = 0;
  base = base | reg[rs];
  int address = base + sign_imm;
  if(address >= DMEM_OFFSET and address <= DMEM_OFFSET + DMEM_LENGTH)
  {
  uint8_t byte = reg[rt] & 0xff;//only keep lowest 8 bits
  dmem[(sign_imm + reg[rs] - DMEM_OFFSET)/4] = byte;
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
