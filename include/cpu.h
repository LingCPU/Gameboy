#ifndef CPU_H
#define CPU_H
// Z80 8-bit chip

#include <cstdint>
#include "clock.h"
#include "mmu.h"
#include "register.h"
#include "address.h"

enum class Condition{
    NZ, 
    Z,
    NC,
    C
};

class CPU{
public:
    CPU();

    void executeOpcode(const uint8_t opcode);
    void executeRegularOpcode(const uint8_t opcode);
    void executeCBOpcode();

private:
    MMU mmu;
    Clock clock;

    // 8-bit Registers:
    Register a, b, c, d, e, f, h, l;

    // Combined Registers:
    uint16_t getAF() const { return pairVal(a, f); }
    uint16_t getBC() const { return pairVal(b, c); }
    uint16_t getDE() const { return pairVal(d, e); }
    uint16_t getHL() const { return pairVal(h, l); }

    void setAF(uint16_t word) { setPair(a, f, word); }
    void setBC(uint16_t word) { setPair(b, c, word); }
    void setDE(uint16_t word) { setPair(d, e, word); }
    void setHL(uint16_t word) { setPair(h, l, word); }

    // Program Counter and Stack Pointer registers
    uint16_t pc;
    uint16_t sp;
    void incrementSP() { sp++; }
    void decrementSP() { sp--; }
    // SP only needs to be incremented/decremented when pushing/popping from stack

    uint8_t getByteFromPC();
    uint16_t getWordFromPC();

    // Flags
    bool flagZero() const;
    bool flagSubtract() const;
    bool flagHalfCarry() const;
    bool flagCarry() const;

    void setFlagZero(bool value);
    void setFlagSubtract(bool value);
    void setFlagHalfCarry(bool value);
    void setFlagCarry(bool value);
    
    
    // Opcode helpers:
    // ADC
    void opcode_adc(); // adc only ever loads into register A
    void opcode_adc(const Register& reg);
    void opcode_adc(const Address& addr);

    // ADD
    void opcode_add(); // add A, d8
    void opcode_add(const Register& reg); // add only ever loads into register A from another 8bit register
    void opcode_add(const Address& addr);
    void opcode_add(const Register& high, const Register& low); // add HL, rr
    void opcode_add_sp(); // add SP, r8
    void opcode_add_hl(); // add HL, SP

    // AND
    void opcode_and();
    void opcode_and(Register& reg);
    void opcode_and(const Address& addr);

    // BIT -TODO: fix type
    void opcode_bit(const int bit, Register& reg);
    void opcode_bit(const int bit, const Address& addr);

    // CALL
    void opcode_call();
    void opcode_call(Condition cond);

    // CCF
    void opcode_ccf();

    // CP
    void opcode_cp();
    void opcode_cp(Register& reg);
    void opcode_cp(const Address& addr);

    // CPL
    void opcode_cpl();

    // DAA
    void opcode_daa();

    // DEC
    void opcode_dec(); // for sp
    void opcode_dec(Register& reg);
    void opcode_dec(Register& high, Register& low);
    void opcode_dec(const Address& addr);

    // DI
    void opcode_di();
    
    // EI
    void opcode_ei();

    // HALT
    void opcode_halt();

    // INC
    void opcode_inc(); // for sp
    void opcode_inc(Register& reg);
    void opcode_inc(Register& high, Register& low);
    void opcode_inc(const Address& addr);

    // JP
    void opcode_jp(const Address& addr); // JP (HL)
    void opcode_jp(Condition cond);
    void opcode_jp();
    
    // JR
    void opcode_jr();
    void opcode_jr(Condition cond);

    // LD - TODO : something for F8 and F9, LD HL, SP+r8 and LD SP, HL, and for ld sp
    void opcode_ld(Register& reg); 
    void opcode_ld(Register& dest, const Register& src); 
    void opcode_ld(Register& reg, const Address& addr);
    void opcode_ldPair(Register& high, Register& low); 
    void opcode_ld_sp(const Address& addr); // ld a16 sp
    void opcode_ld_sp(Register& high, Register& low);
    void opcode_ld_sp(); 
    void opcode_ld(const Address& addr); // LD (HL), d8
    void opcode_ld(const Address& addr, const Register& reg); 

    void opcode_ld_c_a(); // ld (C), A
    void opcode_ld_a_c(); // ld A, (C)
    void opcode_ld_hl(); // ld HL, SP+r8

    void opcode_ld_addr(); // ld A, (a16)
    void opcode_ld_addr(Register& reg); // ld (a16), A

    // LDH
    void opcode_ldh(); // ldh A, (a8)
    void opcode_ldh(Register& reg); // ldh (a8), A

    // NOP
    void opcode_nop();

    // OR
    void opcode_or();
    void opcode_or(const Register& reg);
    void opcode_or(const Address& addr);

    // POP
    void opcode_pop(const Register& high, const Register& low);

    // PUSH
    void opcode_push(const Register& high, const Register& low);

    // RL
    void opcode_rl(const Address& addr);
    void opcode_rl(Register& reg);
    // RLA
    void opcode_rla();

    // RLC
    void opcode_rlc(const Address& addr);
    void opcode_rlc(Register& reg);

    // RLCA
    void opcode_rlca();

    // RR
    void opcode_rr(const Address& addr);
    void opcode_rr(Register& reg);

    // RRA
    void opcode_rra();

    // RRC
    void opcode_rrc(const Address& addr);
    void opcode_rrc(Register& reg);

    // RRCA
    void opcode_rrca();

    // RST -TODO: fix type
    void opcode_rst(const int offset);

    // RES
    void opcode_res(const int bit, Register& reg);
    void opcode_res(const int bit, const Address& addr);

    // RET
    void opcode_ret();
    void opcode_ret(Condition cond);

    // RETI
    void opcode_reti();

    // SBC // everything goes into A
    void opcode_sbc(); // for immediate val into A
    void opcode_sbc(Register& reg); 
    void opcode_sbc(const Address& addr);

    // SCF
    void opcode_scf();

    // SET
    void opcode_set(const int bit, const Address& addr);
    void opcode_set(const int bit, Register& reg);

    // SLA
    void opcode_sla(Register& reg);
    void opcode_sla(const Address& addr);

    // SRA
    void opcode_sra(Register& reg);
    void opcode_sra(const Address& addr);

    // SRL 
    void opcode_srl(Register& reg);
    void opcode_srl(const Address& addr);

    // STOP
    void opcode_stop();

    // SUB
    void opcode_sub();
    void opcode_sub(Register& reg);
    void opcode_sub(const Address& addr);

    // SWAP
    void opcode_swap(Register& reg);
    void opcode_swap(const Address& addr);

    // XOR
    void opcode_xor();
    void opcode_xor(const Address& addr);
    void opcode_xor(const Register& reg);


    // Opcodes 
    void opcode_00(); void opcode_10(); void opcode_20(); void opcode_30(); void opcode_40(); void opcode_50(); void opcode_60(); void opcode_70(); void opcode_80(); void opcode_90(); void opcode_A0(); void opcode_B0(); void opcode_C0(); void opcode_D0(); void opcode_E0(); void opcode_F0();
    void opcode_01(); void opcode_11(); void opcode_21(); void opcode_31(); void opcode_41(); void opcode_51(); void opcode_61(); void opcode_71(); void opcode_81(); void opcode_91(); void opcode_A1(); void opcode_B1(); void opcode_C1(); void opcode_D1(); void opcode_E1(); void opcode_F1();
    void opcode_02(); void opcode_12(); void opcode_22(); void opcode_32(); void opcode_42(); void opcode_52(); void opcode_62(); void opcode_72(); void opcode_82(); void opcode_92(); void opcode_A2(); void opcode_B2(); void opcode_C2(); void opcode_D2(); void opcode_E2(); void opcode_F2();
    void opcode_03(); void opcode_13(); void opcode_23(); void opcode_33(); void opcode_43(); void opcode_53(); void opcode_63(); void opcode_73(); void opcode_83(); void opcode_93(); void opcode_A3(); void opcode_B3(); void opcode_C3(); void opcode_D3(); void opcode_E3(); void opcode_F3();
    void opcode_04(); void opcode_14(); void opcode_24(); void opcode_34(); void opcode_44(); void opcode_54(); void opcode_64(); void opcode_74(); void opcode_84(); void opcode_94(); void opcode_A4(); void opcode_B4(); void opcode_C4(); void opcode_D4(); void opcode_E4(); void opcode_F4();
    void opcode_05(); void opcode_15(); void opcode_25(); void opcode_35(); void opcode_45(); void opcode_55(); void opcode_65(); void opcode_75(); void opcode_85(); void opcode_95(); void opcode_A5(); void opcode_B5(); void opcode_C5(); void opcode_D5(); void opcode_E5(); void opcode_F5();
    void opcode_06(); void opcode_16(); void opcode_26(); void opcode_36(); void opcode_46(); void opcode_56(); void opcode_66(); void opcode_76(); void opcode_86(); void opcode_96(); void opcode_A6(); void opcode_B6(); void opcode_C6(); void opcode_D6(); void opcode_E6(); void opcode_F6();
    void opcode_07(); void opcode_17(); void opcode_27(); void opcode_37(); void opcode_47(); void opcode_57(); void opcode_67(); void opcode_77(); void opcode_87(); void opcode_97(); void opcode_A7(); void opcode_B7(); void opcode_C7(); void opcode_D7(); void opcode_E7(); void opcode_F7();
    void opcode_08(); void opcode_18(); void opcode_28(); void opcode_38(); void opcode_48(); void opcode_58(); void opcode_68(); void opcode_78(); void opcode_88(); void opcode_98(); void opcode_A8(); void opcode_B8(); void opcode_C8(); void opcode_D8(); void opcode_E8(); void opcode_F8();
    void opcode_09(); void opcode_19(); void opcode_29(); void opcode_39(); void opcode_49(); void opcode_59(); void opcode_69(); void opcode_79(); void opcode_89(); void opcode_99(); void opcode_A9(); void opcode_B9(); void opcode_C9(); void opcode_D9(); void opcode_E9(); void opcode_F9();
    void opcode_0A(); void opcode_1A(); void opcode_2A(); void opcode_3A(); void opcode_4A(); void opcode_5A(); void opcode_6A(); void opcode_7A(); void opcode_8A(); void opcode_9A(); void opcode_AA(); void opcode_BA(); void opcode_CA(); void opcode_DA(); void opcode_EA(); void opcode_FA();
    void opcode_0B(); void opcode_1B(); void opcode_2B(); void opcode_3B(); void opcode_4B(); void opcode_5B(); void opcode_6B(); void opcode_7B(); void opcode_8B(); void opcode_9B(); void opcode_AB(); void opcode_BB(); void opcode_CB(); void opcode_DB(); void opcode_EB(); void opcode_FB();
    void opcode_0C(); void opcode_1C(); void opcode_2C(); void opcode_3C(); void opcode_4C(); void opcode_5C(); void opcode_6C(); void opcode_7C(); void opcode_8C(); void opcode_9C(); void opcode_AC(); void opcode_BC(); void opcode_CC(); void opcode_DC(); void opcode_EC(); void opcode_FC();
    void opcode_0D(); void opcode_1D(); void opcode_2D(); void opcode_3D(); void opcode_4D(); void opcode_5D(); void opcode_6D(); void opcode_7D(); void opcode_8D(); void opcode_9D(); void opcode_AD(); void opcode_BD(); void opcode_CD(); void opcode_DD(); void opcode_ED(); void opcode_FD();
    void opcode_0E(); void opcode_1E(); void opcode_2E(); void opcode_3E(); void opcode_4E(); void opcode_5E(); void opcode_6E(); void opcode_7E(); void opcode_8E(); void opcode_9E(); void opcode_AE(); void opcode_BE(); void opcode_CE(); void opcode_DE(); void opcode_EE(); void opcode_FE();
    void opcode_0F(); void opcode_1F(); void opcode_2F(); void opcode_3F(); void opcode_4F(); void opcode_5F(); void opcode_6F(); void opcode_7F(); void opcode_8F(); void opcode_9F(); void opcode_AF(); void opcode_BF(); void opcode_CF(); void opcode_DF(); void opcode_EF(); void opcode_FF();

    // CB Opcodes
    void opcode_CB_00(); void opcode_CB_10(); void opcode_CB_20(); void opcode_CB_30(); void opcode_CB_40(); void opcode_CB_50(); void opcode_CB_60(); void opcode_CB_70(); void opcode_CB_80(); void opcode_CB_90(); void opcode_CB_A0(); void opcode_CB_B0(); void opcode_CB_C0(); void opcode_CB_D0(); void opcode_CB_E0(); void opcode_CB_F0();
    void opcode_CB_01(); void opcode_CB_11(); void opcode_CB_21(); void opcode_CB_31(); void opcode_CB_41(); void opcode_CB_51(); void opcode_CB_61(); void opcode_CB_71(); void opcode_CB_81(); void opcode_CB_91(); void opcode_CB_A1(); void opcode_CB_B1(); void opcode_CB_C1(); void opcode_CB_D1(); void opcode_CB_E1(); void opcode_CB_F1();
    void opcode_CB_02(); void opcode_CB_12(); void opcode_CB_22(); void opcode_CB_32(); void opcode_CB_42(); void opcode_CB_52(); void opcode_CB_62(); void opcode_CB_72(); void opcode_CB_82(); void opcode_CB_92(); void opcode_CB_A2(); void opcode_CB_B2(); void opcode_CB_C2(); void opcode_CB_D2(); void opcode_CB_E2(); void opcode_CB_F2();
    void opcode_CB_03(); void opcode_CB_13(); void opcode_CB_23(); void opcode_CB_33(); void opcode_CB_43(); void opcode_CB_53(); void opcode_CB_63(); void opcode_CB_73(); void opcode_CB_83(); void opcode_CB_93(); void opcode_CB_A3(); void opcode_CB_B3(); void opcode_CB_C3(); void opcode_CB_D3(); void opcode_CB_E3(); void opcode_CB_F3();
    void opcode_CB_04(); void opcode_CB_14(); void opcode_CB_24(); void opcode_CB_34(); void opcode_CB_44(); void opcode_CB_54(); void opcode_CB_64(); void opcode_CB_74(); void opcode_CB_84(); void opcode_CB_94(); void opcode_CB_A4(); void opcode_CB_B4(); void opcode_CB_C4(); void opcode_CB_D4(); void opcode_CB_E4(); void opcode_CB_F4();
    void opcode_CB_05(); void opcode_CB_15(); void opcode_CB_25(); void opcode_CB_35(); void opcode_CB_45(); void opcode_CB_55(); void opcode_CB_65(); void opcode_CB_75(); void opcode_CB_85(); void opcode_CB_95(); void opcode_CB_A5(); void opcode_CB_B5(); void opcode_CB_C5(); void opcode_CB_D5(); void opcode_CB_E5(); void opcode_CB_F5();
    void opcode_CB_06(); void opcode_CB_16(); void opcode_CB_26(); void opcode_CB_36(); void opcode_CB_46(); void opcode_CB_56(); void opcode_CB_66(); void opcode_CB_76(); void opcode_CB_86(); void opcode_CB_96(); void opcode_CB_A6(); void opcode_CB_B6(); void opcode_CB_C6(); void opcode_CB_D6(); void opcode_CB_E6(); void opcode_CB_F6();
    void opcode_CB_07(); void opcode_CB_17(); void opcode_CB_27(); void opcode_CB_37(); void opcode_CB_47(); void opcode_CB_57(); void opcode_CB_67(); void opcode_CB_77(); void opcode_CB_87(); void opcode_CB_97(); void opcode_CB_A7(); void opcode_CB_B7(); void opcode_CB_C7(); void opcode_CB_D7(); void opcode_CB_E7(); void opcode_CB_F7();
    void opcode_CB_08(); void opcode_CB_18(); void opcode_CB_28(); void opcode_CB_38(); void opcode_CB_48(); void opcode_CB_58(); void opcode_CB_68(); void opcode_CB_78(); void opcode_CB_88(); void opcode_CB_98(); void opcode_CB_A8(); void opcode_CB_B8(); void opcode_CB_C8(); void opcode_CB_D8(); void opcode_CB_E8(); void opcode_CB_F8();
    void opcode_CB_09(); void opcode_CB_19(); void opcode_CB_29(); void opcode_CB_39(); void opcode_CB_49(); void opcode_CB_59(); void opcode_CB_69(); void opcode_CB_79(); void opcode_CB_89(); void opcode_CB_99(); void opcode_CB_A9(); void opcode_CB_B9(); void opcode_CB_C9(); void opcode_CB_D9(); void opcode_CB_E9(); void opcode_CB_F9();
    void opcode_CB_0A(); void opcode_CB_1A(); void opcode_CB_2A(); void opcode_CB_3A(); void opcode_CB_4A(); void opcode_CB_5A(); void opcode_CB_6A(); void opcode_CB_7A(); void opcode_CB_8A(); void opcode_CB_9A(); void opcode_CB_AA(); void opcode_CB_BA(); void opcode_CB_CA(); void opcode_CB_DA(); void opcode_CB_EA(); void opcode_CB_FA();
    void opcode_CB_0B(); void opcode_CB_1B(); void opcode_CB_2B(); void opcode_CB_3B(); void opcode_CB_4B(); void opcode_CB_5B(); void opcode_CB_6B(); void opcode_CB_7B(); void opcode_CB_8B(); void opcode_CB_9B(); void opcode_CB_AB(); void opcode_CB_BB(); void opcode_CB_CB(); void opcode_CB_DB(); void opcode_CB_EB(); void opcode_CB_FB();
    void opcode_CB_0C(); void opcode_CB_1C(); void opcode_CB_2C(); void opcode_CB_3C(); void opcode_CB_4C(); void opcode_CB_5C(); void opcode_CB_6C(); void opcode_CB_7C(); void opcode_CB_8C(); void opcode_CB_9C(); void opcode_CB_AC(); void opcode_CB_BC(); void opcode_CB_CC(); void opcode_CB_DC(); void opcode_CB_EC(); void opcode_CB_FC();
    void opcode_CB_0D(); void opcode_CB_1D(); void opcode_CB_2D(); void opcode_CB_3D(); void opcode_CB_4D(); void opcode_CB_5D(); void opcode_CB_6D(); void opcode_CB_7D(); void opcode_CB_8D(); void opcode_CB_9D(); void opcode_CB_AD(); void opcode_CB_BD(); void opcode_CB_CD(); void opcode_CB_DD(); void opcode_CB_ED(); void opcode_CB_FD();
    void opcode_CB_0E(); void opcode_CB_1E(); void opcode_CB_2E(); void opcode_CB_3E(); void opcode_CB_4E(); void opcode_CB_5E(); void opcode_CB_6E(); void opcode_CB_7E(); void opcode_CB_8E(); void opcode_CB_9E(); void opcode_CB_AE(); void opcode_CB_BE(); void opcode_CB_CE(); void opcode_CB_DE(); void opcode_CB_EE(); void opcode_CB_FE();
    void opcode_CB_0F(); void opcode_CB_1F(); void opcode_CB_2F(); void opcode_CB_3F(); void opcode_CB_4F(); void opcode_CB_5F(); void opcode_CB_6F(); void opcode_CB_7F(); void opcode_CB_8F(); void opcode_CB_9F(); void opcode_CB_AF(); void opcode_CB_BF(); void opcode_CB_CF(); void opcode_CB_DF(); void opcode_CB_EF(); void opcode_CB_FF();
};

#endif