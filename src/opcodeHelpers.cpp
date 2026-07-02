#include "cpu.h"

// ADC
void CPU::opcode_adc(){
}
void CPU::opcode_adc(const Register& reg){
}
void CPU::opcode_adc(const Address& addr){
}

// ADD
void CPU::opcode_add(){
}
void CPU::opcode_add(const Register& reg){
}
void CPU::opcode_add(const Address& addr){
}
void CPU::opcode_add(const Register& high, const Register& low){
}

// AND
void CPU::opcode_and(){
}
void CPU::opcode_and(Register& reg){
}
void CPU::opcode_and(const Address& addr){
}

// BIT

// CALL

// CCF
void CPU::opcode_ccf(){
}

// CP
void CPU::opcode_cp(){}
void CPU::opcode_cp(Register& reg){}
void CPU::opcode_cp(const Address& addr){}

// CPL
void CPU::opcode_cpl(){}

// DAA
void CPU::opcode_daa(){}

// DEC
void CPU::opcode_dec(){
    decrementSP();
}
void CPU::opcode_dec(Register& reg){
    reg.decrement();
}
void CPU::opcode_dec(Register& high, Register& low){
    decrementPair(high, low);
}
void CPU::opcode_dec(const Address& addr){

}


// EI
void CPU::opcode_ei(){}

// HALT
void CPU::opcode_halt(){};

// INC
void CPU::opcode_inc(){
    incrementSP();
}
void CPU::opcode_inc(Register& reg){
    reg.increment();
}
void CPU::opcode_inc(Register& high, Register& low){
    incrementPair(high, low);
}
void CPU::opcode_inc(const Address& addr){
    
}

// JP
void CPU::opcode_jp(){}
void CPU::opcode_jp(const Address& addr){}
void CPU::opcode_jp(Condition cond){}

// JR
void CPU::opcode_jr(){}
void CPU::opcode_jr(Condition cond){}

// LD
void CPU::opcode_ld(Register& reg){
    uint8_t val = getByteFromPC();
    reg.set(val);
}
void CPU::opcode_ld(Register& dest, const Register& src){
    dest.set(src.value());
}
void CPU::opcode_ld(Register& reg, const Address& addr){
    reg.set(mmu.readByte(addr));
}
void CPU::opcode_ld_pair(Register& high, Register& low){
    setPair(high, low, getWordFromPC());
}
void CPU::opcode_ld(const Address& addr){
    uint8_t val = getByteFromPC();
    mmu.writeByte(addr, val);
}
void CPU::opcode_ld(const Address& addr, const Register& reg){
    mmu.writeByte(addr, reg.value());
}

// LDH

// NOP

// OR

// POP

// PUSH

// RLA

// RLCA

// RRA

// RRCA

// RST

// RET

// RETI

// SBC

// SCF

// STOP

// SUB

// XOR






