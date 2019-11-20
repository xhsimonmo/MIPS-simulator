#include <iostream>
#include <math.h>
#include <vector>


class simulator
{
public:
    void run(uint32_t instruction, uint32_t &pc, uint32_t& tmp_pc,std::vector<uint32_t> &reg, uint32_t &reg_lo, uint32_t &reg_hi, bool& delay,std::vector<uint8_t>& dmem, std::vector<char>& imem);
    void R_type(uint32_t instruction, uint32_t &pc,uint32_t& tmp_pc, std::vector<uint32_t> &reg, int rs, int rt, int rd, int sa, uint32_t &reg_lo, uint32_t &reg_hi, bool& delay);

private:
    void add(int rd, int rs, int rt, std::vector<uint32_t> &reg);
    void addi(int rt, int rs, int32_t sign_imm, std::vector<uint32_t> &reg);//imm has been sign extended
    void addiu(int rt, int rs,uint32_t imm, std::vector<uint32_t> &reg);
    void addu(int rd, int rs, int rt, std::vector<uint32_t> &reg);
    void sub(int rd, int rs, int rt, std::vector<uint32_t> &reg);
    void j(uint32_t address,uint32_t& pc, bool& delay);
    void jal(uint32_t address,uint32_t& pc, std::vector<uint32_t> &reg, bool& delay);
    void jalr(int rs, int rd,uint32_t& pc,uint32_t& tmp_pc, std::vector<uint32_t> &reg, bool& delay);
    void jr(int rs, uint32_t& pc,uint32_t& tmp_pc,std::vector<uint32_t> &reg, bool&delay);
    //all operation functions here...
    int32_t get_char();
    void putchar(uint32_t data_write);

    void and_bits(int rd, int rs, int rt, std::vector<uint32_t> &reg);
    void or_bits(int rd, int rs, int rt, std::vector<uint32_t> &reg);
    void ori(int rs, int rt, uint32_t imm, std::vector<uint32_t> &reg);
    void xori(int rs, int rt, uint32_t imm, std::vector<uint32_t> &reg);
    void andi(int rt, int rs, uint32_t imm, std::vector<uint32_t> &reg);
    void beq(int rt, int rs, uint32_t sign_imm, uint32_t& pc, uint32_t& tmp_pc,std::vector<uint32_t> &reg, bool& delay);
    void bgez(int rs, uint32_t sign_imm, uint32_t& pc,uint32_t& tmp_pc, std::vector<uint32_t> &reg, bool& delay);
    void bgezal(int rs, uint32_t sign_imm, uint32_t& pc,uint32_t& tmp_pc, std::vector<uint32_t> &reg, bool& delay);
    void bgtz(int rs, uint32_t sign_imm, uint32_t& pc, uint32_t& tmp_pc,  std::vector<uint32_t> &reg, bool& delay);
    void blez(int rs, uint32_t sign_imm, uint32_t& pc,  uint32_t& tmp_pc, std::vector<uint32_t> &reg, bool& delay);
    void bltz(int rs, uint32_t sign_imm, uint32_t& pc, uint32_t& tmp_pc,  std::vector<uint32_t> &reg, bool& delay);
    void bltzal(int rs, uint32_t sign_imm, uint32_t& pc, uint32_t& tmp_pc,  std::vector<uint32_t> &reg, bool& delay);
    void bne(int rt, int rs, uint32_t sign_imm, uint32_t& pc, uint32_t& tmp_pc, std::vector<uint32_t> &reg, bool& delay);
    void div_f(int rt, int rs, uint32_t &reg_lo, uint32_t &reg_hi, std::vector<uint32_t> &reg);
    void divu(int rt, int rs, uint32_t &reg_lo, uint32_t &reg_hi, std::vector<uint32_t> &reg);

    void lb(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem, std::vector<char>& imem);
    void lbu(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem, std::vector<char>& imem);
    void lh(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem, std::vector<char>& imem);
    void lhu(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem, std::vector<char>& imem);
    void lw(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem, std::vector<char>& imem);
    void lwl(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem, std::vector<char>& imem);
    void lwr(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem, std::vector<char>& imem);
    void lui(int rt, uint32_t imm,std::vector<uint32_t> &reg);
    void sb(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem);
    void slti(int rs,int rt,int32_t sign_imm, std::vector<uint32_t> &reg);
    void sltiu(int rs,int rt,uint32_t imm, std::vector<uint32_t> &reg);
    void sh(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem);
    void sw(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint8_t>& dmem);
    void sll(int rt,int rd,int sa, std::vector<uint32_t> &reg);
    void sllv(int rd,int rs,int rt, std::vector<uint32_t> &reg);
    void slt(int rd, int rs, int rt, std::vector<uint32_t> &reg);
    void sltu(int rd, int rs, int rt, std::vector<uint32_t> &reg);
    void sra(int rt,int rd,int sa, std::vector<uint32_t> &reg);
    void srav(int rd,int rs,int rt, std::vector<uint32_t> &reg);
    void srl(int rt,int rd,int sa, std::vector<uint32_t> &reg);
    void srlv(int rd,int rs,int rt, std::vector<uint32_t> &reg);

    void mfhi(int rd, std::vector<uint32_t> &reg, uint32_t &reg_hi);
    void mflo(int rd, std::vector<uint32_t> &reg, uint32_t &reg_lo);
    void mthi(int rd, std::vector<uint32_t> &reg, uint32_t &reg_hi);
    void mtlo(int rd, std::vector<uint32_t> &reg, uint32_t &reg_lo);
    void mult(int rt, int rs, uint32_t &reg_lo, uint32_t &reg_hi, std::vector<uint32_t> &reg);
    void multu(int rt, int rs, uint32_t &reg_lo, uint32_t &reg_hi, std::vector<uint32_t> &reg);
    void nor(int rd, int rs, int rt, std::vector<uint32_t> &reg);
    void subu(int rd, int rs, int rt, std::vector<uint32_t> &reg);
    void xor_bits(int rd, int rs, int rt, std::vector<uint32_t> &reg);
};
