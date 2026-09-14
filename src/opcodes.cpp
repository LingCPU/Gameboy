#include "cpu.h"
#include "bitwise.h"

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

    return static_cast<uint16_t>(result);
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
    uint8_t raw = getByteFromPC();
    int8_t offset = static_cast<int8_t>(raw);
    uint16_t oldSP = sp;
    uint16_t result = static_cast<uint16_t>(static_cast<int32_t>(sp) + offset);

    setFlagZero(false);
    setFlagSubtract(false);
    setFlagHalfCarry(((oldSP & 0x0F) + (raw & 0x0F)) > 0x0F);
    setFlagCarry(((oldSP & 0xFF) + raw) > 0xFF);

    sp = result;
}

void CPU::opcode_add_hl(){
    uint16_t hl = getHL();
    uint32_t result = static_cast<uint32_t>(hl) + static_cast<uint32_t>(sp);

    setFlagSubtract(false);
    setFlagHalfCarry(((hl & 0x0FFF) + (sp & 0x0FFF)) > 0x0FFF);
    setFlagCarry(result > 0xFFFF);
    setHL(static_cast<uint16_t>(result));
}

// AND
uint8_t CPU::_opcode_and(uint8_t val){
    uint8_t reg = a.value();
    uint8_t result = reg & val;

    setFlagZero(result == 0);
    setFlagSubtract(false);
    setFlagHalfCarry(true);
    setFlagCarry(false);

    return result;
}
void CPU::opcode_and(){
    a.set(_opcode_and(getByteFromPC()));
}
void CPU::opcode_and(Register& reg){
    a.set(_opcode_and(reg.value()));
}
void CPU::opcode_and(const Address& addr){
    a.set(_opcode_and(mmu.readByte(addr)));
}

// BIT
void CPU::_opcode_bit(const uint8_t bit, const uint8_t val){
    setFlagZero(!checkBit(val, bit));
    setFlagSubtract(false);
    setFlagHalfCarry(true);
}

void CPU::opcode_bit(const uint8_t bit, Register& reg){
    _opcode_bit(bit, reg.value());
}

void CPU::opcode_bit(const uint8_t bit, const Address& addr){
    _opcode_bit(bit, mmu.readByte(addr));
}

// CALL
void CPU::opcode_call(){
    uint16_t address = getWordFromPC();
    stackPush(pc);
    pc = address;
}

void CPU::opcode_call(Condition cond){
    uint16_t address = getWordFromPC();
    if(isCondition(cond)){
        stackPush(pc);
        pc = address;
    }
}

// CCF
void CPU::opcode_ccf(){
    setFlagSubtract(false);
    setFlagHalfCarry(false);
    setFlagCarry(!flagCarry());
}

// CP
void CPU::_opcode_cp(const uint8_t val){
    uint8_t reg = a.value();
    uint8_t result = static_cast<uint8_t>(reg - val);

    setFlagZero(result == 0);
    setFlagSubtract(true);
    setFlagHalfCarry(((reg & 0xF) - (val & 0xF)) < 0);
    setFlagCarry(reg < val);
}

void CPU::opcode_cp(){
    _opcode_cp(getByteFromPC());
}

void CPU::opcode_cp(Register& reg){
    _opcode_cp(reg.value());
}
void CPU::opcode_cp(const Address& addr){
    _opcode_cp(mmu.readByte(addr));
}

// CPL
void CPU::opcode_cpl(){
    uint8_t reg = a.value();
    uint8_t result = ~reg;
    a.set(result);
    setFlagSubtract(true);
    setFlagHalfCarry(true);
}

// DAA
void CPU::opcode_daa(){
    uint8_t reg = a.value();
    uint16_t correction = flagCarry() ? 0x60 : 0x00;

    if(flagHalfCarry() || (!flagSubtract() && ((reg & 0x0F) > 9))) correction |= 0x06;
    if(flagCarry() || (!flagSubtract() && (reg > 0x99))) correction |= 0x60;
    
    if(flagSubtract()) reg = static_cast<uint8_t>(reg - correction);
    else reg = static_cast<uint8_t>(reg + correction);

    if(((correction << 2) & 0x100) != 0) setFlagCarry(true);
    setFlagHalfCarry(false);
    setFlagZero(reg == 0);

    a.set(static_cast<uint8_t>(reg));
}

// DEC
void CPU::opcode_dec(){
    --sp;
}

void CPU::opcode_dec(Register& reg){
    reg.decrement();
    uint8_t result = reg.value();

    setFlagZero(result == 0);
    setFlagSubtract(true);
    setFlagHalfCarry((result & 0x0F) == 0x0F);
}

void CPU::opcode_dec(Register& high, Register& low){
    decrementPair(high, low);
}

void CPU::opcode_dec(const Address& addr){
    uint8_t val = mmu.readByte(addr);
    uint8_t result = static_cast<uint8_t>(val - 1);
    mmu.writeByte(addr, result);

    setFlagZero(result == 0);
    setFlagSubtract(true);
    setFlagHalfCarry((result & 0x0F) == 0x0F);
}

// DI
void CPU::opcode_di(){
    IME = false;
    interruptEnableDelay = 0;
}

// EI
void CPU::opcode_ei(){
    interruptEnableDelay = 2;
}

// HALT
void CPU::opcode_halt(){
    const bool interruptPending = mmu.pendingInterrupts() != 0;
    const bool imeWillEnableAfterHalt = interruptEnableDelay == 1;

    if(!IME && !imeWillEnableAfterHalt && interruptPending){
        haltBug = true;
        return;
    }
    halted = true;
};

// INC
void CPU::opcode_inc(){
    ++sp;
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
    uint8_t val = mmu.readByte(addr);
    uint8_t result = static_cast<uint8_t>(val + 1);
    mmu.writeByte(addr, result);

    setFlagZero(result == 0);
    setFlagSubtract(false);
    setFlagHalfCarry((result & 0x0F) == 0);
}

// JP
void CPU::opcode_jp(){
    pc = getWordFromPC();
}

void CPU::opcode_jp(const Address& addr){
    pc = addr.value();
}

void CPU::opcode_jp(Condition cond){
    uint16_t jump = getWordFromPC();
    if(isCondition(cond)) pc = jump;
}

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

void CPU::opcode_ld(const Address& addr){
    uint8_t val = getByteFromPC();
    mmu.writeByte(addr, val);
}

void CPU::opcode_ld(const Address& addr, const Register& reg){
    mmu.writeByte(addr, reg.value());
}

void CPU::opcode_ld_pair(Register& high, Register& low){
    setPair(high, low, getWordFromPC());
}

void CPU::opcode_ld_sp(){
    sp = getWordFromPC();
}

void CPU::opcode_ld_sp(const Address& addr){
    mmu.writeWord(addr, sp);
}

void CPU::opcode_ld_sp(Register& high, Register& low){
    sp = pairVal(high, low);
    // sp = getHL() also works but this gives less compiler warnings
}

void CPU::opcode_ld_hl(){
    uint8_t raw = getByteFromPC();
    int8_t offset = static_cast<int8_t>(raw);
    uint16_t result = static_cast<uint16_t>(static_cast<int32_t>(sp) + offset);

    setFlagZero(false);
    setFlagSubtract(false);
    setFlagHalfCarry(((sp & 0x0F) + (raw & 0x0F)) > 0x0F);
    setFlagCarry(((sp & 0xFF) + raw) > 0xFF);

    setHL(result);
}

void CPU::opcode_ld_c_a(){
    uint16_t addr = static_cast<uint16_t>(0xFF00 + c.value());
    mmu.writeByte(Address(addr), a.value());
}

void CPU::opcode_ld_a_c(){
    uint16_t addr = static_cast<uint16_t>(0xFF00 + c.value());
    a.set(mmu.readByte(Address(addr)));
}

void CPU::opcode_ld_addr(){
    uint16_t addr = getWordFromPC();
    a.set(mmu.readByte(Address(addr)));
}

void CPU::opcode_ld_addr(Register& reg){
    uint16_t addr = getWordFromPC();
    mmu.writeByte(Address(addr), reg.value());
}

// LDH
void CPU::opcode_ldh(){
    uint8_t offset = getByteFromPC();
    uint16_t addr = static_cast<uint16_t>(0xFF00 + offset);
    a.set(mmu.readByte(Address(addr)));
}

void CPU::opcode_ldh(const Register& reg){
    uint8_t offset = getByteFromPC();
    uint16_t addr = static_cast<uint16_t>(0xFF00 + offset);
    mmu.writeByte(Address(addr), reg.value());
}

// NOP
void CPU::opcode_nop(){
    // Do nothing
}

// OR
uint8_t CPU::_opcode_or(uint8_t val){
    uint8_t reg = a.value();
    uint8_t result = reg | val;

    setFlagZero(result == 0);
    setFlagHalfCarry(false);
    setFlagCarry(false);
    setFlagSubtract(false);

    return result;
}

void CPU::opcode_or(){
    a.set(_opcode_or(getByteFromPC()));
}

void CPU::opcode_or(const Register& reg){
    a.set(_opcode_or(reg.value()));
}

void CPU::opcode_or(const Address& addr){
    a.set(_opcode_or(mmu.readByte(addr)));
}

// POP
void CPU::opcode_pop(Register& high, Register& low){
    setPair(high, low, stackPop());
}

// PUSH
void CPU::opcode_push(const Register& high, const Register& low){
    stackPush(pairVal(high, low));
}

// RL
uint8_t CPU::_opcode_rl(uint8_t val){
    uint8_t carry = flagCarry();
    bool willCarry = checkBit(val, 7);
    setFlagCarry(willCarry);
    uint8_t result = static_cast<uint8_t>(val << 1);
    result |= carry;

    setFlagZero(result == 0);
    setFlagSubtract(false);
    setFlagHalfCarry(false);

    return result;
}

void CPU::opcode_rl(const Address& addr){
    mmu.writeByte(addr, _opcode_rl(mmu.readByte(addr)));
}

void CPU::opcode_rl(Register& reg){
    reg.set(_opcode_rl(reg.value()));
}

// RLA
void CPU::opcode_rla(){
    opcode_rl(a);
    setFlagZero(false);
}

// RLC
uint8_t CPU::_opcode_rlc(uint8_t val){
    uint8_t carry = checkBit(val, 7);
    uint8_t truncated = checkBit(val, 7);
    uint8_t result = static_cast<uint8_t>((val << 1) | truncated);

    setFlagCarry(carry);
    setFlagZero(result == 0);
    setFlagHalfCarry(false);
    setFlagSubtract(false);

    return result;
}

void CPU::opcode_rlc(const Address& addr){
    mmu.writeByte(addr, _opcode_rlc(mmu.readByte(addr)));
}

void CPU::opcode_rlc(Register& reg){
    reg.set(_opcode_rlc(reg.value()));
}


// RLCA
void CPU::opcode_rlca(){
    opcode_rlc(a);
    setFlagZero(false);
}

// RR
uint8_t CPU::_opcode_rr(uint8_t val){
    uint8_t carry = flagCarry();
    bool willCarry = checkBit(val, 0);
    setFlagCarry(willCarry);
    uint8_t result = static_cast<uint8_t>(val >> 1);
    result |= (carry << 7);

    setFlagZero(result == 0);
    setFlagSubtract(false);
    setFlagHalfCarry(false);

    return result;
}

void CPU::opcode_rr(const Address& addr){
    mmu.writeByte(addr, _opcode_rr(mmu.readByte(addr)));
}

void CPU::opcode_rr(Register& reg){
    reg.set(_opcode_rr(reg.value()));
}

// RRA
void CPU::opcode_rra(){
    opcode_rr(a);
    setFlagZero(false);
}

// RRC
uint8_t CPU::_opcode_rrc(uint8_t val){
    uint8_t carry = checkBit(val, 0);
    uint8_t truncated = checkBit(val, 0);
    uint8_t result = static_cast<uint8_t>((val >> 1) | (truncated << 7));

    setFlagZero(result == 0);
    setFlagSubtract(false);
    setFlagHalfCarry(false);
    setFlagCarry(carry);

    return result;
}

void CPU::opcode_rrc(const Address& addr){
    mmu.writeByte(addr, _opcode_rrc(mmu.readByte(addr)));
}

void CPU::opcode_rrc(Register& reg){
    reg.set(_opcode_rrc(reg.value()));
}

// RRCA
void CPU::opcode_rrca(){
    opcode_rrc(a);
    setFlagZero(false);
}

// RST
void CPU::opcode_rst(const uint8_t offset){
    stackPush(pc);
    pc = offset;
}

// RES
void CPU::opcode_res(const uint8_t bit, Register& reg){
    reg.set(clearBit(reg.value(), bit));
}

void CPU::opcode_res(const uint8_t bit, const Address& addr){
    uint8_t val = mmu.readByte(addr);
    mmu.writeByte(addr, clearBit(val, bit));
}

// RET
void CPU::opcode_ret(){
    pc = stackPop();
}

void CPU::opcode_ret(Condition cond){
    if(isCondition(cond)) opcode_ret();
}

// RETI
void CPU::opcode_reti(){
    opcode_ret();
    IME = true;
    interruptEnableDelay = 0;
}

// SBC
uint8_t CPU::_opcode_sbc(uint8_t val){
    uint8_t carry = flagCarry();
    uint8_t reg = a.value();
    int result = reg - val - carry;
    
    setFlagZero(static_cast<uint8_t>(result) == 0);
    setFlagSubtract(true);
    setFlagHalfCarry(((reg & 0xF) - (val & 0xF) - carry) < 0);
    setFlagCarry(result < 0);

    return static_cast<uint8_t>(result);
}

void CPU::opcode_sbc() {
    a.set(_opcode_sbc(getByteFromPC()));
}

void CPU::opcode_sbc(Register& reg) {
    a.set(_opcode_sbc(reg.value()));
}

void CPU::opcode_sbc(const Address& addr) {
    a.set(_opcode_sbc(mmu.readByte(addr)));
}

// SCF
void CPU::opcode_scf(){
    setFlagSubtract(false);
    setFlagHalfCarry(false);
    setFlagCarry(true);
}

// SET
void CPU::opcode_set(const uint8_t bit, const Address& addr){
    uint8_t val = mmu.readByte(addr);
    uint8_t result = setBit(val, bit);
    mmu.writeByte(addr, result);
}

void CPU::opcode_set(const uint8_t bit, Register& reg){
    uint8_t result = setBit(reg.value(), bit);
    reg.set(result);
}

// SLA
uint8_t CPU::_opcode_sla(uint8_t val){
    uint8_t carry = checkBit(val, 7);
    uint8_t result = static_cast<uint8_t>(val << 1);

    setFlagZero(result == 0);
    setFlagSubtract(false);
    setFlagHalfCarry(false);
    setFlagCarry(carry);

    return result;
}

void CPU::opcode_sla(Register& reg){
    reg.set(_opcode_sla(reg.value()));
}
void CPU::opcode_sla(const Address& addr){
    mmu.writeByte(addr, _opcode_sla(mmu.readByte(addr)));
}

// SRA
uint8_t CPU::_opcode_sra(uint8_t val){
    uint8_t carry = checkBit(val, 0);
    uint8_t result = static_cast<uint8_t>((val >> 1) | (val & 0x80));

    setFlagZero(result == 0);
    setFlagSubtract(false);
    setFlagHalfCarry(false);
    setFlagCarry(carry);

    return result;
}

void CPU::opcode_sra(Register& reg){
    reg.set(_opcode_sra(reg.value()));
}

void CPU::opcode_sra(const Address& addr){
    mmu.writeByte(addr, _opcode_sra(mmu.readByte(addr)));
}

// SRL
uint8_t CPU::_opcode_srl(uint8_t val){
    uint8_t carry = checkBit(val, 0);
    uint8_t result = static_cast<uint8_t>(val >> 1);

    setFlagZero(result == 0);
    setFlagSubtract(false);
    setFlagHalfCarry(false);
    setFlagCarry(carry);

    return result;
}

void CPU::opcode_srl(Register& reg){
    reg.set(_opcode_srl(reg.value()));
}

void CPU::opcode_srl(const Address& addr){
    mmu.writeByte(addr, _opcode_srl(mmu.readByte(addr)));
}

// STOP
void CPU::opcode_stop(){
    // halted = true; 
    // TODO
}

// SUB
uint8_t CPU::_opcode_sub(uint8_t val){
    uint8_t reg = a.value();
    uint8_t result = static_cast<uint8_t>(reg - val);

    setFlagZero(result == 0);
    setFlagSubtract(true);
    setFlagHalfCarry(((reg & 0xF) - (val & 0xF)) < 0);
    setFlagCarry(reg < val);

    return result;
}

void CPU::opcode_sub(){
    a.set(_opcode_sub(getByteFromPC()));
}

void CPU::opcode_sub(Register& reg){
    a.set(_opcode_sub(reg.value()));
}

void CPU::opcode_sub(const Address& addr){
    a.set(_opcode_sub(mmu.readByte(addr)));
}

// SWAP
uint8_t CPU::_opcode_swap(uint8_t val){
    uint8_t result = static_cast<uint8_t>((val << 4) | (val >> 4));

    setFlagZero(result == 0);
    setFlagSubtract(false);
    setFlagHalfCarry(false);
    setFlagCarry(false);

    return result;
}

void CPU::opcode_swap(Register& reg){
    reg.set(_opcode_swap(reg.value()));
}

void CPU::opcode_swap(const Address& addr){
    mmu.writeByte(addr, _opcode_swap(mmu.readByte(addr)));
}

// XOR
uint8_t CPU::_opcode_xor(uint8_t val){
    uint8_t reg = a.value();
    uint8_t result = reg ^ val;

    setFlagZero(result == 0);
    setFlagSubtract(false);
    setFlagHalfCarry(false);
    setFlagCarry(false);

    return result;
}

void CPU::opcode_xor(){
    a.set(_opcode_xor(getByteFromPC()));
}

void CPU::opcode_xor(const Address& addr){
    a.set(_opcode_xor(mmu.readByte(addr)));
}

void CPU::opcode_xor(const Register& reg){
    a.set(_opcode_xor(reg.value()));
}