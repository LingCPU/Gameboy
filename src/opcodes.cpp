#include "cpu.h"

// ADC
uint8_t CPU::_opcode_adc(const uint8_t val){
    uint8_t reg = a.value();
    uint8_t carry = flagCarry() ? 1 : 0;
    uint16_t result = static_cast<uint16_t>(reg) + static_cast<uint16_t>(val) + carry;

    setFlagZero((result & 0xFF) == 0);
    setFlagSubtract(false);
    setFlagHalfCarry(((reg & 0x0F) + (val & 0x0F) + carry) > 0x0F);
    setFlagCarry(result > 0xFF);

    return static_cast<uint8_t>(result & 0xFF);
}

void CPU::opcode_adc(){
    uint8_t result = _opcode_adc(getByteFromPC());
    a.set(result); 
}

void CPU::opcode_adc(const Register& reg){
    uint8_t result = _opcode_adc(reg.value());
    a.set(result);
}

void CPU::opcode_adc(const Address& addr){
    uint8_t result = _opcode_adc(mmu.readByte(addr));
    a.set(result);
}

// ADD
uint8_t CPU::_opcode_add(const uint8_t val){
    uint8_t reg = a.value();
    uint16_t result = static_cast<uint16_t>(reg) + static_cast<uint16_t>(val);

    setFlagZero((result & 0xFF) == 0);
    setFlagSubtract(false);
    setFlagHalfCarry((reg & 0x0F) + (val & 0x0F) > 0x0F);
    setFlagCarry(result > 0xFF);

    return static_cast<uint8_t>(result & 0xFF);
}

uint16_t CPU::_opcode_add(const Register& high, const Register& low){
    uint16_t reg = getHL();
    uint16_t val = pairVal(high, low);
    uint32_t result = static_cast<uint32_t>(reg) + static_cast<uint32_t>(val);

    setFlagSubtract(false);
    setFlagHalfCarry((reg & 0x0FFF) + (val & 0x0FFF) > 0x0FFF);
    setFlagCarry(result > 0xFFFF);

    return static_cast<uint16_t>(result); // couldn't figure out how to escape overflow
}

void CPU::opcode_add(){
    uint8_t result = _opcode_add(getByteFromPC());
    a.set(result);
}

void CPU::opcode_add(const Register& reg){
    uint8_t result = _opcode_add(reg.value());
    a.set(result);
}

void CPU::opcode_add(const Address& addr){
    uint8_t result = _opcode_add(mmu.readByte(addr));
    a.set(result);
}

void CPU::opcode_add(const Register& high, const Register& low){
    uint16_t result = _opcode_add(high, low);
    setHL(result);
}

void CPU::opcode_add_sp(){
    // TODO

}
void CPU::opcode_add_hl(){
    // TODO
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
void CPU::opcode_call(){
    uint16_t address = getWordFromPC();
    stackPush(pc);
    pc = address;
}

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
}

void CPU::opcode_dec(Register& reg){
    reg.decrement();
    auto result = reg.value();

    setFlagZero(result == 0);
    setFlagSubtract(true);
    setFlagHalfCarry((result & 0x0F) == 0x0F);
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
}

void CPU::opcode_inc(Register& reg){
    reg.increment();
    auto result = reg.value();

    setFlagZero(result == 0);
    setFlagSubtract(false);
    setFlagHalfCarry((result & 0x0F) == 0);
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
void CPU::opcode_jr(){
    int8_t offset = static_cast<int8_t>(getByteFromPC());
    pc = static_cast<uint16_t>(pc + offset);
}

void CPU::opcode_jr(Condition cond){
    int8_t offset = static_cast<int8_t>(getByteFromPC());
    if(isCondition(cond)) pc = static_cast<uint16_t>(pc + offset);
}

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
void CPU::opcode_pop(Register& high, Register& low){
    setPair(high, low, stackPop());
}

// PUSH
void CPU::opcode_push(const Register& high, const Register& low){
    stackPush(pairVal(high, low));
}

// RLA

// RLCA

// RRA

// RRCA

// RST

// RET
void CPU::opcode_ret(){
    pc = stackPop();
}

// RETI

// SBC

// SCF

// STOP

// SUB

// XOR






