#include <iostream>
#include <math.h>
#include <vector>


class simulator
{
public:
    void run(uint32_t instruction, uint32_t &pc, std::vector<uint32_t> &reg, uint32_t &reg_lo, uint32_t &reg_hi, bool& delay,std::vector<uint32_t>& dmem);
    void R_type(uint32_t instruction, uint32_t &pc, std::vector<uint32_t> &reg, int rs, int rt, int rd, uint32_t &reg_lo, uint32_t &reg_hi, bool& delay);

private:
    void add(int rd, int rs, int rt, std::vector<uint32_t> &reg);
    void addi(int rt, int rs, int32_t sign_imm, std::vector<uint32_t> &reg);//imm has been sign extended
    void addiu(int rt, int rs,uint32_t imm, std::vector<uint32_t> &reg);
    void addu(int rd, int rs, int rt, std::vector<uint32_t> &reg);
    void sub(int rd, int rs, int rt, std::vector<uint32_t> &reg);
    void j(int rs, uint32_t address,uint32_t& pc, bool& delay);
    void jr(int rs, uint32_t& pc,std::vector<uint32_t> &reg, bool&delay);
    //all operation functions here...
    int32_t get_char();
    void putchar(uint32_t data_write);

    void and_bits(int rd, int rs, int rt, std::vector<uint32_t> &reg);
    void or_bits(int rd, int rs, int rt, std::vector<uint32_t> &reg);
    void andi(int rt, int rs, uint32_t imm, std::vector<uint32_t> &reg);
    void beq(int rt, int rs, uint32_t sign_imm, uint32_t& pc, std::vector<uint32_t> &reg, bool& delay);
    void bgez(int rs, uint32_t sign_imm, uint32_t& pc, std::vector<uint32_t> &reg, bool& delay);
    void bgezal(int rs, uint32_t sign_imm, uint32_t& pc, std::vector<uint32_t> &reg, bool& delay);
    void bgtz(int rs, uint32_t sign_imm, uint32_t& pc, std::vector<uint32_t> &reg, bool& delay);
    void blez(int rs, uint32_t sign_imm, uint32_t& pc, std::vector<uint32_t> &reg, bool& delay);
    void bltz(int rs, uint32_t sign_imm, uint32_t& pc, std::vector<uint32_t> &reg, bool& delay);
    void bltzal(int rs, uint32_t sign_imm, uint32_t& pc, std::vector<uint32_t> &reg, bool& delay);
    void bne(int rt, int rs, uint32_t sign_imm, uint32_t& pc, std::vector<uint32_t> &reg, bool& delay);
    void div(int rt, int rs, uint32_t &reg_lo, uint32_t &reg_hi, std::vector<uint32_t> &reg);
    void divu(int rt, int rs, uint32_t &reg_lo, uint32_t &reg_hi, std::vector<uint32_t> &reg);

    void lb(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint32_t>& dmem);
    void sb(int rs,int rt, int32_t sign_imm, std::vector<uint32_t> &reg, std::vector<uint32_t>& dmem);


};
