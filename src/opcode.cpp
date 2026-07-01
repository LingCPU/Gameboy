#include "cpu.h"

void CPU::opcode_00(){ // NOP
    opcode_nop();
}

void CPU::opcode_01(){ // LD BC, d16
    opcode_ldPair(b, c);
}

void CPU::opcode_02(){ // LD (BC), A
    opcode_ld(Address(getBC()), a);
}

void CPU::opcode_03(){ // INC BC
    opcode_inc(b, c);
}

void CPU::opcode_04(){ // INC B
    opcode_inc(b);
}

void CPU::opcode_05(){ // DEC B
    opcode_dec(b);
}

void CPU::opcode_06(){ // LD B, d8
    opcode_ld(b);
}

void CPU::opcode_07(){ // RLCA
    opcode_rlca();
}

void CPU::opcode_08(){ // LD (a16), SP
    uint16_t nn = getWordFromPC();
    opcode_ld_sp(Address(nn));
}

void CPU::opcode_09(){ // ADD HL, BC
    opcode_add(b, c);
}

void CPU::opcode_0A(){ // LD A, (BC)
    opcode_ld(a, Address(getBC()));
}

void CPU::opcode_0B(){ // DEC BC
    opcode_dec(b, c);
}

void CPU::opcode_0C(){ // INC C
    opcode_inc(c);
}

void CPU::opcode_0D(){ // DEC C
    opcode_dec(c);
}

void CPU::opcode_0E(){ // LD C, d8
    opcode_ld(c);
}

void CPU::opcode_0F(){ // RRCA
    opcode_rrca();
}

void CPU::opcode_10(){ // STOP 0
    opcode_stop();
}

void CPU::opcode_11(){ // LD DE, d16
    opcode_ldPair(d, e);
}

void CPU::opcode_12(){ // LD (DE), A
    opcode_ld(Address(getDE()), a);
}

void CPU::opcode_13(){ // INC DE
    opcode_inc(d, e);
}

void CPU::opcode_14(){ // INC D 
    opcode_inc(d);
}

void CPU::opcode_15(){ // DEC D
    opcode_dec(d);
}

void CPU::opcode_16(){ // LD D, d8
    opcode_ld(d);
}

void CPU::opcode_17(){ // RLA
    opcode_rla();
}

void CPU::opcode_18(){ // JR r8
    opcode_jr();
}

void CPU::opcode_19(){ // ADD HL, DE
    opcode_add(d, e);
}

void CPU::opcode_1A(){ // LD A, (DE)
    opcode_ld(a, Address(getDE()));
}

void CPU::opcode_1B(){ // DEC DE
    opcode_dec(d, e);
}

void CPU::opcode_1C(){ // INC E
    opcode_inc(e);
}

void CPU::opcode_1D(){ // DEC E
    opcode_dec(e);
}

void CPU::opcode_1E(){ // LD E, d8
    opcode_ld(e);
}

void CPU::opcode_1F(){ // RRA
    opcode_rra();
}

void CPU::opcode_20(){ // JR NZ, r8
    opcode_jr(Condition::NZ);
}

void CPU::opcode_21(){ // LD HL, d16
    opcode_ldPair(h, l);
}

void CPU::opcode_22(){ // LD (HL+), A
    opcode_ld(Address(getHL()), a);
    opcode_inc(h, l);
}

void CPU::opcode_23(){ // INC HL
    opcode_inc(h, l);
}   

void CPU::opcode_24(){ // INC H
    opcode_inc(h);
}

void CPU::opcode_25(){ // DEC H
    opcode_dec(h);
}

void CPU::opcode_26(){ // LD H, d8
    opcode_ld(h);
}

void CPU::opcode_27(){ // DAA
    opcode_daa();
}

void CPU::opcode_28(){ // JR Z, r8
    opcode_jr(Condition::Z);
}

void CPU::opcode_29(){ // ADD HL, HL
    opcode_add(h, l);
}

void CPU::opcode_2A(){ // LD A, (HL+)
    opcode_ld(a, Address(getHL()));
    opcode_inc(h, l);
}

void CPU::opcode_2B(){ // DEC HL
    opcode_dec(h, l);
}

void CPU::opcode_2C(){ // INC L
    opcode_inc(l);
}

void CPU::opcode_2D(){ // DEC L
    opcode_dec(l);
}

void CPU::opcode_2E(){ // LD L, d8
    opcode_ld(l);
}

void CPU::opcode_2F(){ // CPL
    opcode_cpl();
}

void CPU::opcode_30(){ // JR NC, r8
    opcode_jr(Condition::NC);
}

void CPU::opcode_31(){ // LD SP, d16
    opcode_ld_sp();
}

void CPU::opcode_32(){ // LD (HL-), A
    opcode_ld(Address(getHL()), a);
    opcode_dec(h, l);
}

void CPU::opcode_33(){ // INC SP
    opcode_inc();
}

void CPU::opcode_34(){ // INC (HL)
    opcode_inc(Address(getHL()));
}

void CPU::opcode_35(){ // DEC (HL)
    opcode_dec(Address(getHL()));
}

void CPU::opcode_36(){ // LD (HL), d8
    opcode_ld(Address(getHL()));
}

void CPU::opcode_37(){ // SCF
    opcode_scf();
}

void CPU::opcode_38(){ // JR C, r8
    opcode_jr(Condition::C);
}

void CPU::opcode_39(){ // ADD HL, SP
    opcode_add_hl();
}

void CPU::opcode_3A(){ // LD A, (HL-)
    opcode_ld(a, Address(getHL()));
    opcode_dec(h, l);
}

void CPU::opcode_3B(){ // DEC SP
    opcode_dec();
}

void CPU::opcode_3C(){ // INC A
    opcode_inc(a);
}

void CPU::opcode_3D(){ // DEC A
    opcode_dec(a);
}

void CPU::opcode_3E(){ // LD A, d8
    opcode_ld(a);
}

void CPU::opcode_3F(){ // CCF
    opcode_ccf();
}

void CPU::opcode_40(){ // LD B, B
    opcode_ld(b, b);
}

void CPU::opcode_41(){ // LD B, C
    opcode_ld(b, c);
}

void CPU::opcode_42(){ // LD B, D
    opcode_ld(b, d);
}

void CPU::opcode_43(){ // LD B, E
    opcode_ld(b, e);
}

void CPU::opcode_44(){ // LD B, H
    opcode_ld(b, h);
}

void CPU::opcode_45(){ // LD B, L
    opcode_ld(b, l);
}

void CPU::opcode_46(){ // LD B, (HL)
    opcode_ld(b, Address(getHL()));
}

void CPU::opcode_47(){ // LD B, A
    opcode_ld(b, a);
}

void CPU::opcode_48(){ // LD C, B
    opcode_ld(c, b);
}

void CPU::opcode_49(){ // LD C, C
    opcode_ld(c, c);
}

void CPU::opcode_4A(){ // LD C, D
    opcode_ld(c, d);
}

void CPU::opcode_4B(){ // LD C, E
    opcode_ld(c, e);
}

void CPU::opcode_4C(){ // LD C, H
    opcode_ld(c, h);
}

void CPU::opcode_4D(){ // LD C, L
    opcode_ld(c, l);
}

void CPU::opcode_4E(){ // LD C, (HL)
    opcode_ld(c, Address(getHL()));
}

void CPU::opcode_4F(){ // LD C, A
    opcode_ld(c, a);
}

void CPU::opcode_50(){ // LD D, B
    opcode_ld(d, b);
}

void CPU::opcode_51(){ // LD D, C
    opcode_ld(d, c);
}

void CPU::opcode_52(){ // LD D, D
    opcode_ld(d, d);
}

void CPU::opcode_53(){ // LD D, E
    opcode_ld(d, e);
}

void CPU::opcode_54(){ // LD D, H
    opcode_ld(d, h);
}

void CPU::opcode_55(){ // LD D, L
    opcode_ld(d, l);
}

void CPU::opcode_56(){ // LD D, (HL)
    opcode_ld(d, Address(getHL()));
}

void CPU::opcode_57(){ // LD D, A
    opcode_ld(d, a);
}

void CPU::opcode_58(){ // LD E, B
    opcode_ld(e, b);
}

void CPU::opcode_59(){ // LD E, C
    opcode_ld(e, c);
}

void CPU::opcode_5A(){ // LD E, D
    opcode_ld(e, d);
}

void CPU::opcode_5B(){ // LD E, E
    opcode_ld(e, e);
}

void CPU::opcode_5C(){ // LD E, H
    opcode_ld(e, h);
}

void CPU::opcode_5D(){ // LD E, L
    opcode_ld(e, l);
}

void CPU::opcode_5E(){ // LD E, (HL)
    opcode_ld(e, Address(getHL()));
}

void CPU::opcode_5F(){ // LD E, A
    opcode_ld(e, a);
}

void CPU::opcode_60(){ // LD H, B
    opcode_ld(h, b);
}

void CPU::opcode_61(){ // LD H, C
    opcode_ld(h, c);
}

void CPU::opcode_62(){ // LD H, D
    opcode_ld(h, d);
}

void CPU::opcode_63(){ // LD H, E
    opcode_ld(h, e);
}

void CPU::opcode_64(){ // LD H, H
    opcode_ld(h, h);
}

void CPU::opcode_65(){ // LD H, L
    opcode_ld(h, l);
}

void CPU::opcode_66(){ // LD H, (HL)
    opcode_ld(h, Address(getHL()));
}

void CPU::opcode_67(){ // LD H, A
    opcode_ld(h, a);
}

void CPU::opcode_68(){ // LD L, B
    opcode_ld(l, b);
}

void CPU::opcode_69(){ // LD L, C
    opcode_ld(l, c);
}

void CPU::opcode_6A(){ // LD L, D
    opcode_ld(l, d);
}

void CPU::opcode_6B(){ // LD L, E
    opcode_ld(l, e);
}

void CPU::opcode_6C(){ // LD L, H
    opcode_ld(l, h);
}

void CPU::opcode_6D(){ // LD L, L
    opcode_ld(l, l);
}

void CPU::opcode_6E(){ // LD L, (HL)
    opcode_ld(l, Address(getHL()));
}

void CPU::opcode_6F(){ // LD L, A
    opcode_ld(l, a);
}

void CPU::opcode_70(){ // LD (HL), B
    opcode_ld(Address(getHL()), b);
}

void CPU::opcode_71(){ // LD (HL), C
    opcode_ld(Address(getHL()), c);
}

void CPU::opcode_72(){ // LD (HL), D
    opcode_ld(Address(getHL()), d);
}

void CPU::opcode_73(){ // LD (HL), E
    opcode_ld(Address(getHL()), e);
}

void CPU::opcode_74(){ // LD (HL), H
    opcode_ld(Address(getHL()), h);
}

void CPU::opcode_75(){ // LD (HL), L
    opcode_ld(Address(getHL()), l);
}

void CPU::opcode_76(){ // HALT
    opcode_halt();
}

void CPU::opcode_77(){ // LD (HL), A
    opcode_ld(Address(getHL()), a);
}

void CPU::opcode_78(){ // LD A, B
    opcode_ld(a, b);
}

void CPU::opcode_79(){ // LD A, C
    opcode_ld(a, c);
}

void CPU::opcode_7A(){ // LD A, D
    opcode_ld(a, d);
}

void CPU::opcode_7B(){ // LD A, E
    opcode_ld(a, e);
}

void CPU::opcode_7C(){ // LD A, H
    opcode_ld(a, h);
}

void CPU::opcode_7D(){ // LD A, L
    opcode_ld(a, l);
}

void CPU::opcode_7E(){ // LD A, (HL)
    opcode_ld(a, Address(getHL()));
}

void CPU::opcode_7F(){ // LD A, A
    opcode_ld(a, a);
}

void CPU::opcode_80(){ // ADD A, B
    opcode_add(b);
}

void CPU::opcode_81(){ // ADD A, C
    opcode_add(c);
}

void CPU::opcode_82(){ // ADD A, D
    opcode_add(d);
}

void CPU::opcode_83(){ // ADD A, E
    opcode_add(e);
}

void CPU::opcode_84(){ // ADD A, H
    opcode_add(h);
}

void CPU::opcode_85(){ // ADD A, L
    opcode_add(l);
}

void CPU::opcode_86(){ // ADD A, (HL)
    opcode_add(Address(getHL()));
}

void CPU::opcode_87(){ // ADD A, A
    opcode_add(a);
}

void CPU::opcode_88(){ // ADC A, B 
    opcode_adc(b);
}

void CPU::opcode_89(){ // ADC A, C
    opcode_adc(c);
}

void CPU::opcode_8A(){ // ADC A, D
    opcode_adc(d);
}

void CPU::opcode_8B(){ // ADC A, E
    opcode_adc(e);
}

void CPU::opcode_8C(){ // ADC A, H
    opcode_adc(h);
}

void CPU::opcode_8D(){ // ADC A, L
    opcode_adc(l);
}

void CPU::opcode_8E(){ // ADC A, (HL)
    opcode_adc(Address(getHL()));
}

void CPU::opcode_8F(){ // ADC A, A
    opcode_adc(a);
}

void CPU::opcode_90(){ // SUB B  
    opcode_sub(b);
}

void CPU::opcode_91(){ // SUB C
    opcode_sub(c);
}

void CPU::opcode_92(){ // SUB D
    opcode_sub(d);
}

void CPU::opcode_93(){ // SUB E
    opcode_sub(e);
}

void CPU::opcode_94(){ // SUB H
    opcode_sub(h);
}

void CPU::opcode_95(){ // SUB L 
    opcode_sub(l);
}

void CPU::opcode_96(){ // SUB (HL)
    opcode_sub(Address(getHL()));
}

void CPU::opcode_97(){ // SUB A
    opcode_sub(a);
}

void CPU::opcode_98(){ // SBC A, B
    opcode_sbc(b);
}

void CPU::opcode_99(){ // SBC A, C
    opcode_sbc(c);
}

void CPU::opcode_9A(){ // SBC A, D
    opcode_sbc(d);
}

void CPU::opcode_9B(){ // SBC A, E
    opcode_sbc(e);
}

void CPU::opcode_9C(){ // SBC A, H
    opcode_sbc(h);
}

void CPU::opcode_9D(){ // SBC A, L
    opcode_sbc(l);
}

void CPU::opcode_9E(){ // SBC A, (HL)
    opcode_sbc(Address(getHL()));
}

void CPU::opcode_9F(){ // SBC A, A
    opcode_sbc(a);
}

void CPU::opcode_A0(){ // AND B
    opcode_and(b);
}

void CPU::opcode_A1(){ // AND C
    opcode_and(c);
}

void CPU::opcode_A2(){ // AND D
    opcode_and(d);
}

void CPU::opcode_A3(){ // AND E
    opcode_and(e);
}

void CPU::opcode_A4(){ // AND H
    opcode_and(h);
}

void CPU::opcode_A5(){ // AND L
    opcode_and(l);
}

void CPU::opcode_A6(){ // AND (HL)
    opcode_and(Address(getHL()));
}

void CPU::opcode_A7(){ // AND A
    opcode_and(a);
}

void CPU::opcode_A8(){ // XOR B
    opcode_xor(b);
}

void CPU::opcode_A9(){ // XOR C
    opcode_xor(c);
}

void CPU::opcode_AA(){ // XOR D
    opcode_xor(d);
}

void CPU::opcode_AB(){ // XOR E
    opcode_xor(e);
}

void CPU::opcode_AC(){ // XOR H
    opcode_xor(h);
}

void CPU::opcode_AD(){ // XOR L
    opcode_xor(l);
}

void CPU::opcode_AE(){ // XOR (HL)
    opcode_xor(Address(getHL()));
}

void CPU::opcode_AF(){ // XOR A
    opcode_xor(a);
}

void CPU::opcode_B0(){ // OR B
    opcode_or(b);
}

void CPU::opcode_B1(){ // OR C
    opcode_or(c);
}

void CPU::opcode_B2(){ // OR D
    opcode_or(d);
}

void CPU::opcode_B3(){ // OR E
    opcode_or(e);
}

void CPU::opcode_B4(){ // OR H
    opcode_or(h);
}

void CPU::opcode_B5(){ // OR L
    opcode_or(l);
}

void CPU::opcode_B6(){ // OR (HL)
    opcode_or(Address(getHL()));
}

void CPU::opcode_B7(){ // OR A
    opcode_or(a);
}

void CPU::opcode_B8(){ // CP B
    opcode_cp(b);
}

void CPU::opcode_B9(){ // CP C
    opcode_cp(c);
}

void CPU::opcode_BA(){ // CP D
    opcode_cp(d);
}

void CPU::opcode_BB(){ // CP E
    opcode_cp(e);
}

void CPU::opcode_BC(){ // CP H
    opcode_cp(h);
}

void CPU::opcode_BD(){ // CP L
    opcode_cp(l);
}

void CPU::opcode_BE(){ // CP (HL)
    opcode_cp(Address(getHL()));
}

void CPU::opcode_BF(){ // CP A
    opcode_cp(a);
}

void CPU::opcode_C0(){ // RET NZ
    opcode_ret(Condition::NZ);
}

void CPU::opcode_C1(){ // POP BC
    opcode_pop(b, c);
}

void CPU::opcode_C2(){ // JP NZ, a16
    opcode_jp(Condition::NZ);
}

void CPU::opcode_C3(){ // JP a16
    opcode_jp();
}

void CPU::opcode_C4(){ // CALL NZ, a16
    opcode_call(Condition::NZ);
}

void CPU::opcode_C5(){ // PUSH BC
    opcode_push(b, c);
}

void CPU::opcode_C6(){ // ADD A, d8
    opcode_add();
}

void CPU::opcode_C7(){ // RST 00H
    opcode_rst(0);
}

void CPU::opcode_C8(){ // RET Z
    opcode_ret(Condition::Z);
}

void CPU::opcode_C9(){ // RET
    opcode_ret();
}

void CPU::opcode_CA(){ // JP Z, a16
    opcode_jp(Condition::Z);
}

void CPU::opcode_CB(){ // PREFIX CB
    // todo
}

void CPU::opcode_CC(){ // CALL Z, a16
    opcode_call(Condition::Z);
}

void CPU::opcode_CD(){ // CALL a16
    opcode_call();
}

void CPU::opcode_CE(){ // ADC A, d8
    opcode_adc();
}

void CPU::opcode_CF(){ // RST 08H
    opcode_rst(8);
}

void CPU::opcode_D0(){ // RET NC
    opcode_ret(Condition::NC);
}

void CPU::opcode_D1(){ // POP DE
    opcode_pop(d, e);
}

void CPU::opcode_D2(){ // JP NC, a16
    opcode_jp(Condition::NC);
}

void CPU::opcode_D3(){ // Undefined
}

void CPU::opcode_D4(){ // CALL NC, a16
    opcode_call(Condition::NC);
}

void CPU::opcode_D5(){ // PUSH DE
    opcode_push(d, e);
}

void CPU::opcode_D6(){ // SUB d8
    opcode_sub();
}

void CPU::opcode_D7(){ // RST 10H
    opcode_rst(10);
}

void CPU::opcode_D8(){ // RET C
    opcode_ret(Condition::C);
}

void CPU::opcode_D9(){ // RETI
    opcode_reti();
}

void CPU::opcode_DA(){ // JP C, a16
    opcode_jp(Condition::C);
}

void CPU::opcode_DB(){ // Undefined
}

void CPU::opcode_DC(){ // CALL C, a16
    opcode_call(Condition::C);
}

void CPU::opcode_DD(){ // Undefined
}

void CPU::opcode_DE(){ // SBC A, d8
    opcode_sbc();
}

void CPU::opcode_DF(){ // RST 18H
    opcode_rst(18);
}

void CPU::opcode_E0(){ // LDH (a8), A
    opcode_ldh(a);
}

void CPU::opcode_E1(){ // POP HL
    opcode_pop(h, l);
}

void CPU::opcode_E2(){ // LD (C), A
    opcode_ld_c_a();
}

void CPU::opcode_E3(){ // Undefined
}

void CPU::opcode_E4(){ // Undefined
}

void CPU::opcode_E5(){ // PUSH HL
    opcode_push(h, l);
}

void CPU::opcode_E6(){ // AND d8
    opcode_and();
}

void CPU::opcode_E7(){ // RST 20H
    opcode_rst(20);
}

void CPU::opcode_E8(){ // ADD SP, r8
    opcode_add_sp();
}

void CPU::opcode_E9(){ // JP (HL)
    opcode_jp(Address(getHL()));
}

void CPU::opcode_EA(){ // LD (a16), A
    opcode_ld_addr(a);
}

void CPU::opcode_EB(){ // Undefined
}

void CPU::opcode_EC(){ // Undefined
}

void CPU::opcode_ED(){ // Undefined
}

void CPU::opcode_EE(){ // XOR d8
    opcode_xor();
}

void CPU::opcode_EF(){ // RST 28H
    opcode_rst(28);
}

void CPU::opcode_F0(){ // LDH A, (a8)
    opcode_ldh();
}

void CPU::opcode_F1(){ // POP AF
    opcode_pop(a, f);
}

void CPU::opcode_F2(){ // LD A, (C)
    opcode_ld_a_c();
}

void CPU::opcode_F3(){ // DI
    opcode_di();
}

void CPU::opcode_F4(){ // Undefined
}

void CPU::opcode_F5(){ // PUSH AF
    opcode_push(a, f);
}

void CPU::opcode_F6(){ // OR d8
    opcode_or();
}

void CPU::opcode_F7(){ // RST 30H
    opcode_rst(30);
}

void CPU::opcode_F8(){ // LD HL, SP+r8
    opcode_ld_hl();
}

void CPU::opcode_F9(){ // LD SP, HL
    opcode_ld_sp(h, l);
}

void CPU::opcode_FA(){ // LD A, (a16)
    opcode_ld_addr();
}

void CPU::opcode_FB(){ // EI
    opcode_ei();
}

void CPU::opcode_FC(){ // Undefined
}

void CPU::opcode_FD(){ // Undefined
}

void CPU::opcode_FE(){ // CP d8
    opcode_cp();
}

void CPU::opcode_FF(){ // RST 38H
    opcode_rst(38);
}