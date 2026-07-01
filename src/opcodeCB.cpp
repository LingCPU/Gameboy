#include "cpu.h"

void CPU::opcode_CB_00(){ // RLC B
    opcode_rlc(b);
}

void CPU::opcode_CB_01(){ // RLC C
    opcode_rlc(c);
}

void CPU::opcode_CB_02(){ // RLC D
    opcode_rlc(d);
}

void CPU::opcode_CB_03(){ // RLC E
    opcode_rlc(e);
}

void CPU::opcode_CB_04(){ // RLC H
    opcode_rlc(h);
}

void CPU::opcode_CB_05(){ // RLC L
    opcode_rlc(l);
}

void CPU::opcode_CB_06(){ // RLC (HL)
    opcode_rlc(Address(getHL()));
}

void CPU::opcode_CB_07(){ // RLC A
    opcode_rlc(a);
}

void CPU::opcode_CB_08(){ // RRC B
    opcode_rrc(b);
}

void CPU::opcode_CB_09(){ // RRC C
    opcode_rrc(c);
}

void CPU::opcode_CB_0A(){ // RRC D
    opcode_rrc(d);
}

void CPU::opcode_CB_0B(){ // RRC E
    opcode_rrc(e);
}

void CPU::opcode_CB_0C(){ // RRC H
    opcode_rrc(h);
}

void CPU::opcode_CB_0D(){ // RRC L
    opcode_rrc(l);
}

void CPU::opcode_CB_0E(){ // RRC (HL)
    opcode_rrc(Address(getHL()));
}

void CPU::opcode_CB_0F(){ // RRC A
    opcode_rrc(a);
}

void CPU::opcode_CB_10(){ // RL B
    opcode_rl(b);
}

void CPU::opcode_CB_11(){ // RL C
    opcode_rl(c);
}

void CPU::opcode_CB_12(){ // RL D
    opcode_rl(d);
}

void CPU::opcode_CB_13(){ // RL E
    opcode_rl(e);
}

void CPU::opcode_CB_14(){ // RL H
    opcode_rl(h);
}

void CPU::opcode_CB_15(){ // RL L
    opcode_rl(l);
}

void CPU::opcode_CB_16(){ // RL (HL)
    opcode_rl(Address(getHL()));
}

void CPU::opcode_CB_17(){ // RL A
    opcode_rl(a);
}

void CPU::opcode_CB_18(){ // RR B
    opcode_rr(b);
}

void CPU::opcode_CB_19(){ // RR C
    opcode_rr(c);
}

void CPU::opcode_CB_1A(){ // RR D
    opcode_rr(d);
}

void CPU::opcode_CB_1B(){ // RR E
    opcode_rr(e);
}

void CPU::opcode_CB_1C(){ // RR H
    opcode_rr(h);
}

void CPU::opcode_CB_1D(){ // RR L
    opcode_rr(l);
}

void CPU::opcode_CB_1E(){ // RR (HL)
    opcode_rr(Address(getHL()));
}

void CPU::opcode_CB_1F(){ // RR A
    opcode_rr(a);
}

void CPU::opcode_CB_20(){ // SLA B
    opcode_sla(b);
}

void CPU::opcode_CB_21(){ // SLA C
    opcode_sla(c);
}

void CPU::opcode_CB_22(){ // SLA D
    opcode_sla(d);
}

void CPU::opcode_CB_23(){ // SLA E
    opcode_sla(e);
}

void CPU::opcode_CB_24(){ // SLA H
    opcode_sla(h);
}

void CPU::opcode_CB_25(){ // SLA L
    opcode_sla(l);
}

void CPU::opcode_CB_26(){ // SLA (HL)
    opcode_sla((Address(getHL())));
}

void CPU::opcode_CB_27(){ // SLA A
    opcode_sla(a);
}

void CPU::opcode_CB_28(){ // SRA B
    opcode_sra(b);
}

void CPU::opcode_CB_29(){ // SRA C
    opcode_sra(c);
}

void CPU::opcode_CB_2A(){ // SRA D
    opcode_sra(d);
}

void CPU::opcode_CB_2B(){ // SRA E
    opcode_sra(e);
}

void CPU::opcode_CB_2C(){ // SRA H
    opcode_sra(h);
}

void CPU::opcode_CB_2D(){ // SRA L
    opcode_sra(l);
}

void CPU::opcode_CB_2E(){ // SRA (HL)
    opcode_sra(Address(getHL()));
}

void CPU::opcode_CB_2F(){ // SRA A
    opcode_sra(a);
}

void CPU::opcode_CB_30(){ // SWAP B
    opcode_swap(b);
}

void CPU::opcode_CB_31(){ // SWAP C
    opcode_swap(c);
}

void CPU::opcode_CB_32(){ // SWAP D
    opcode_swap(d);
}

void CPU::opcode_CB_33(){ // SWAP E
    opcode_swap(e);
}

void CPU::opcode_CB_34(){ // SWAP H
    opcode_swap(h);
}

void CPU::opcode_CB_35(){ // SWAP L
    opcode_swap(l);
}

void CPU::opcode_CB_36(){ // SWAP (HL)
    opcode_swap(Address(getHL()));
}

void CPU::opcode_CB_37(){ // SWAP A
    opcode_swap(a);
}

void CPU::opcode_CB_38(){ // SRL B
    opcode_srl(b);
}

void CPU::opcode_CB_39(){ // SRL C
    opcode_srl(c);
}

void CPU::opcode_CB_3A(){ // SRL D
    opcode_srl(d);
}

void CPU::opcode_CB_3B(){ // SRL E
    opcode_srl(e);
}

void CPU::opcode_CB_3C(){ // SRL H
    opcode_srl(h);
}

void CPU::opcode_CB_3D(){ // SRL L
    opcode_srl(l);
}

void CPU::opcode_CB_3E(){ // SRL (HL)
    opcode_srl(Address(getHL()));
}

void CPU::opcode_CB_3F(){ // SRL A
    opcode_srl(a);
}

void CPU::opcode_CB_40(){ // BIT 0, B
    opcode_bit(0, b);
}

void CPU::opcode_CB_41(){ // BIT 0, C
    opcode_bit(0, c);
}

void CPU::opcode_CB_42(){ // BIT 0, D
    opcode_bit(0, d);
}

void CPU::opcode_CB_43(){ // BIT 0, E
    opcode_bit(0, e);
}

void CPU::opcode_CB_44(){ // BIT 0, H
    opcode_bit(0, h);
}

void CPU::opcode_CB_45(){ // BIT 0, L
    opcode_bit(0, l);
}

void CPU::opcode_CB_46(){ // BIT 0, (HL)
    opcode_bit(0, Address(getHL()));
}

void CPU::opcode_CB_47(){ // BIT 0, A
    opcode_bit(0, a);
}

void CPU::opcode_CB_48(){ // BIT 1, B
    opcode_bit(1, b);
}

void CPU::opcode_CB_49(){ // BIT 1, C
    opcode_bit(1, c);
}

void CPU::opcode_CB_4A(){ // BIT 1, D
    opcode_bit(1, d);
}

void CPU::opcode_CB_4B(){ // BIT 1, E
    opcode_bit(1, e);
}

void CPU::opcode_CB_4C(){ // BIT 1, H
    opcode_bit(1, h);
}

void CPU::opcode_CB_4D(){ // BIT 1, L
    opcode_bit(1, l);
}

void CPU::opcode_CB_4E(){ // BIT 1, (HL)
    opcode_bit(1, Address(getHL()));
}

void CPU::opcode_CB_4F(){ // BIT 1, A
    opcode_bit(1, a);
}

void CPU::opcode_CB_50(){ // BIT 2, B
    opcode_bit(2, b);
}

void CPU::opcode_CB_51(){ // BIT 2, C
    opcode_bit(2, c);
}

void CPU::opcode_CB_52(){ // BIT 2, D
    opcode_bit(2, d);
}

void CPU::opcode_CB_53(){ // BIT 2, E
    opcode_bit(2, e);
}

void CPU::opcode_CB_54(){ // BIT 2, H
    opcode_bit(2, h);
}

void CPU::opcode_CB_55(){ // BIT 2, L
    opcode_bit(2, l);
}

void CPU::opcode_CB_56(){ // BIT 2, (HL)
    opcode_bit(2, Address(getHL()));
}

void CPU::opcode_CB_57(){ // BIT 2, A
    opcode_bit(2, a);
}

void CPU::opcode_CB_58(){ // BIT 3, B
    opcode_bit(3, b);
}

void CPU::opcode_CB_59(){ // BIT 3, C
    opcode_bit(3, c);
}

void CPU::opcode_CB_5A(){ // BIT 3, D
    opcode_bit(3, d);
}

void CPU::opcode_CB_5B(){ // BIT 3, E
    opcode_bit(3, e);
}

void CPU::opcode_CB_5C(){ // BIT 3, H
    opcode_bit(3, h);
}

void CPU::opcode_CB_5D(){ // BIT 3, L
    opcode_bit(3, l);
}

void CPU::opcode_CB_5E(){ // BIT 3, (HL)
    opcode_bit(3, Address(getHL()));
}

void CPU::opcode_CB_5F(){ // BIT 3, A
    opcode_bit(3, a);
}

void CPU::opcode_CB_60(){ // BIT 4, B
    opcode_bit(4, b);
}

void CPU::opcode_CB_61(){ // BIT 4, C
    opcode_bit(4, c);
}

void CPU::opcode_CB_62(){ // BIT 4, D
    opcode_bit(4, d);
}

void CPU::opcode_CB_63(){ // BIT 4, E
    opcode_bit(4, e);
}

void CPU::opcode_CB_64(){ // BIT 4, H
    opcode_bit(4, h);
}

void CPU::opcode_CB_65(){ // BIT 4, L
    opcode_bit(4, l);
}

void CPU::opcode_CB_66(){ // BIT 4, (HL)
    opcode_bit(4, Address(getHL()));
}

void CPU::opcode_CB_67(){ // BIT 4, A
    opcode_bit(4, a);
}

void CPU::opcode_CB_68(){ // BIT 5, B
    opcode_bit(5, b);
}

void CPU::opcode_CB_69(){ // BIT 5, C
    opcode_bit(5, c);
}

void CPU::opcode_CB_6A(){ // BIT 5, D
    opcode_bit(5, d);
}

void CPU::opcode_CB_6B(){ // BIT 5, E
    opcode_bit(5, e);
}

void CPU::opcode_CB_6C(){ // BIT 5, H
    opcode_bit(5, h);
}

void CPU::opcode_CB_6D(){ // BIT 5, L
    opcode_bit(5, l);
}

void CPU::opcode_CB_6E(){ // BIT 5, (HL)
    opcode_bit(5, Address(getHL()));
}

void CPU::opcode_CB_6F(){ // BIT 5, A
    opcode_bit(5, a);
}

void CPU::opcode_CB_70(){ // BIT 6, B
    opcode_bit(6, b);
}

void CPU::opcode_CB_71(){ // BIT 6, C
    opcode_bit(6, c);
}

void CPU::opcode_CB_72(){ // BIT 6, D
    opcode_bit(6, d);
}

void CPU::opcode_CB_73(){ // BIT 6, E
    opcode_bit(6, e);
}

void CPU::opcode_CB_74(){ // BIT 6, H
    opcode_bit(6, h);
}

void CPU::opcode_CB_75(){ // BIT 6, L
    opcode_bit(6, l);
}

void CPU::opcode_CB_76(){ // BIT 6, (HL)
    opcode_bit(6, Address(getHL()));
}

void CPU::opcode_CB_77(){ // BIT 6, A
    opcode_bit(6, a);
}

void CPU::opcode_CB_78(){ // BIT 7, B
    opcode_bit(7, b);
}

void CPU::opcode_CB_79(){ // BIT 7, C
    opcode_bit(7, c);
}

void CPU::opcode_CB_7A(){ // BIT 7, D
    opcode_bit(7, d);
}

void CPU::opcode_CB_7B(){ // BIT 7, E
    opcode_bit(7, e);
}

void CPU::opcode_CB_7C(){ // BIT 7, H
    opcode_bit(7, h);
}

void CPU::opcode_CB_7D(){ // BIT 7, L
    opcode_bit(7, l);
}

void CPU::opcode_CB_7E(){ // BIT 7, (HL)
    opcode_bit(7, Address(getHL()));
}

void CPU::opcode_CB_7F(){ // BIT 7, A
    opcode_bit(7, a);
}

void CPU::opcode_CB_80(){ // RES 0, B
    opcode_res(0, b);
}

void CPU::opcode_CB_81(){ // RES 0, C
    opcode_res(0, c);
}

void CPU::opcode_CB_82(){ // RES 0, D
    opcode_res(0, d);
}

void CPU::opcode_CB_83(){ // RES 0, E
    opcode_res(0, e);
}

void CPU::opcode_CB_84(){ // RES 0, H
    opcode_res(0, h);
}

void CPU::opcode_CB_85(){ // RES 0, L
    opcode_res(0, l);
}

void CPU::opcode_CB_86(){ // RES 0, (HL)
    opcode_res(0, Address(getHL()));
}

void CPU::opcode_CB_87(){ // RES 0, A
    opcode_res(0, a);
}

void CPU::opcode_CB_88(){ // RES 1, B
    opcode_res(1, b);
}

void CPU::opcode_CB_89(){ // RES 1, C
    opcode_res(1, c);
}

void CPU::opcode_CB_8A(){ // RES 1, D
    opcode_res(1, d);
}

void CPU::opcode_CB_8B(){ // RES 1, E
    opcode_res(1, e);
}

void CPU::opcode_CB_8C(){ // RES 1, H
    opcode_res(1, h);
}

void CPU::opcode_CB_8D(){ // RES 1, L
    opcode_res(1, l);
}

void CPU::opcode_CB_8E(){ // RES 1, (HL)
    opcode_res(1, Address(getHL()));
}

void CPU::opcode_CB_8F(){ // RES 1, A
    opcode_res(1, a);
}

void CPU::opcode_CB_90(){ // RES 2, B
    opcode_res(2, b);
}

void CPU::opcode_CB_91(){ // RES 2, C
    opcode_res(2, c);
}

void CPU::opcode_CB_92(){ // RES 2, D
    opcode_res(2, d);
}

void CPU::opcode_CB_93(){ // RES 2, E
    opcode_res(2, e);
}

void CPU::opcode_CB_98(){ // RES 2, H
    opcode_res(2, h);
}

void CPU::opcode_CB_95(){ // RES 2, L
    opcode_res(2, l);
}

void CPU::opcode_CB_96(){ // RES 2, (HL)
    opcode_res(2, Address(getHL()));
}

void CPU::opcode_CB_97(){ // RES 2, A
    opcode_res(2, a);
}

void CPU::opcode_CB_98(){ // RES 3, B
    opcode_res(3, b);
}

void CPU::opcode_CB_99(){ // RES 3, C
    opcode_res(3, c);
}

void CPU::opcode_CB_9A(){ // RES 3, D
    opcode_res(3, d);
}

void CPU::opcode_CB_9B(){ // RES 3, E
    opcode_res(3, e);
}

void CPU::opcode_CB_9C(){ // RES 3, H
    opcode_res(3, h);
}

void CPU::opcode_CB_9D(){ // RES 3, L
    opcode_res(3, l);
}

void CPU::opcode_CB_9E(){ // RES 3, (HL)
    opcode_res(3, Address(getHL()));
}

void CPU::opcode_CB_9F(){ // RES 3, A
    opcode_res(3, a);
}

void CPU::opcode_CB_A0(){ // RES 4, B
    opcode_res(4, b);
}

void CPU::opcode_CB_A1(){ // RES 4, C
    opcode_res(4, c);
}

void CPU::opcode_CB_A2(){ // RES 4, D
    opcode_res(4, d);
}

void CPU::opcode_CB_A3(){ // RES 4, E
    opcode_res(4, e);
}

void CPU::opcode_CB_A4(){ // RES 4, H
    opcode_res(4, h);
}

void CPU::opcode_CB_A5(){ // RES 4, L
    opcode_res(4, l);
}

void CPU::opcode_CB_A6(){ // RES 4, (HL)
    opcode_res(4, Address(getHL()));
}

void CPU::opcode_CB_A7(){ // RES 4, A
    opcode_res(4, a);
}

void CPU::opcode_CB_A8(){ // RES 5, B
    opcode_res(5, b);
}

void CPU::opcode_CB_A9(){ // RES 5, C
    opcode_res(5, c);
}

void CPU::opcode_CB_AA(){ // RES 5, D
    opcode_res(5, d);
}

void CPU::opcode_CB_AB(){ // RES 5, E
    opcode_res(5, e);
}

void CPU::opcode_CB_AC(){ // RES 5, H
    opcode_res(5, h);
}

void CPU::opcode_CB_AD(){ // RES 5, L
    opcode_res(5, l);
}

void CPU::opcode_CB_AE(){ // RES 5, (HL)
    opcode_res(5, Address(getHL()));
}

void CPU::opcode_CB_AF(){ // RES 5, A
    opcode_res(5, a);
}

void CPU::opcode_CB_B0(){ // RES 6, B
    opcode_res(6, b);
}

void CPU::opcode_CB_B1(){ // RES 6, C
    opcode_res(6, c);
}

void CPU::opcode_CB_B2(){ // RES 6, D
    opcode_res(6, d);
}

void CPU::opcode_CB_B3(){ // RES 6, E
    opcode_res(6, e);
}

void CPU::opcode_CB_B4(){ // RES 6, H
    opcode_res(6, h);
}

void CPU::opcode_CB_B5(){ // RES 6, L
    opcode_res(6, l);
}

void CPU::opcode_CB_B6(){ // RES 6, (HL)
    opcode_res(6, Address(getHL()));
}

void CPU::opcode_CB_B7(){ // RES 6, A
    opcode_res(6, a);
}

void CPU::opcode_CB_B8(){ // RES 7, B
    opcode_res(7, b);
}

void CPU::opcode_CB_B9(){ // RES 7, C
    opcode_res(7, c);
}

void CPU::opcode_CB_BA(){ // RES 7, D
    opcode_res(7, d);
}

void CPU::opcode_CB_BB(){ // RES 7, E
    opcode_res(7, e);
}

void CPU::opcode_CB_BC(){ // RES 7, H
    opcode_res(7, h);
}

void CPU::opcode_CB_BD(){ // RES 7, L
    opcode_res(7, l);
}

void CPU::opcode_CB_BE(){ // RES 7, (HL)
    opcode_res(7, Address(getHL()));
}

void CPU::opcode_CB_BF(){ // RES 7, A
    opcode_res(7, a);
}

void CPU::opcode_CB_C0(){ // SET 0, B
    opcode_set(0, b);
}

void CPU::opcode_CB_C1(){ // SET 0, C
    opcode_set(0, c);
}

void CPU::opcode_CB_C2(){ // SET 0, D
    opcode_set(0, d);
}

void CPU::opcode_CB_C3(){ // SET 0, E
    opcode_set(0, e);
}

void CPU::opcode_CB_C4(){ // SET 0, H
    opcode_set(0, h);
}

void CPU::opcode_CB_C5(){ // SET 0, L
    opcode_set(0, l);
}

void CPU::opcode_CB_C6(){ // SET 0, (HL)
    opcode_set(0, Address(getHL()));
}

void CPU::opcode_CB_C7(){ // SET 0, A
    opcode_set(0, a);
}

void CPU::opcode_CB_C8(){ // SET 1, B
    opcode_set(1, b);
}

void CPU::opcode_CB_C9(){ // SET 1, C
    opcode_set(1, c);
}

void CPU::opcode_CB_CA(){ // SET 1, D
    opcode_set(1, d);
}

void CPU::opcode_CB_CB(){ // SET 1, E
    opcode_set(1, e);
}

void CPU::opcode_CB_CC(){ // SET 1, H
    opcode_set(1, h);
}

void CPU::opcode_CB_CD(){ // SET 1, L
    opcode_set(1, l);
}

void CPU::opcode_CB_CE(){ // SET 1, (HL)
    opcode_set(1, Address(getHL()));
}

void CPU::opcode_CB_CF(){ // SET 1, A
    opcode_set(1, a);
}

void CPU::opcode_CB_D0(){ // SET 2, B
    opcode_set(2, b);
}

void CPU::opcode_CB_D1(){ // SET 2, C
    opcode_set(2, c);
}

void CPU::opcode_CB_D2(){ // SET 2, D
    opcode_set(2, d);
}

void CPU::opcode_CB_D3(){ // SET 2, E
    opcode_set(2, e);
}

void CPU::opcode_CB_D8(){ // SET 2, H
    opcode_set(2, h);
}

void CPU::opcode_CB_D5(){ // SET 2, L
    opcode_set(2, l);
}

void CPU::opcode_CB_D6(){ // SET 2, (HL)
    opcode_set(2, Address(getHL()));
}

void CPU::opcode_CB_D7(){ // SET 2, A
    opcode_set(2, a);
}

void CPU::opcode_CB_D8(){ // SET 3, B
    opcode_set(3, b);
}

void CPU::opcode_CB_D9(){ // SET 3, C
    opcode_set(3, c);
}

void CPU::opcode_CB_DA(){ // SET 3, D
    opcode_set(3, d);
}

void CPU::opcode_CB_DB(){ // SET 3, E
    opcode_set(3, e);
}

void CPU::opcode_CB_DC(){ // SET 3, H
    opcode_set(3, h);
}

void CPU::opcode_CB_DD(){ // SET 3, L
    opcode_set(3, l);
}

void CPU::opcode_CB_DE(){ // SET 3, (HL)
    opcode_set(3, Address(getHL()));
}

void CPU::opcode_CB_DF(){ // SET 3, A
    opcode_set(3, a);
}

void CPU::opcode_CB_E0(){ // SET 4, B
    opcode_set(4, b);
}

void CPU::opcode_CB_E1(){ // SET 4, C
    opcode_set(4, c);
}

void CPU::opcode_CB_E2(){ // SET 4, D
    opcode_set(4, d);
}

void CPU::opcode_CB_E3(){ // SET 4, E
    opcode_set(4, e);
}

void CPU::opcode_CB_E4(){ // SET 4, H
    opcode_set(4, h);
}

void CPU::opcode_CB_E5(){ // SET 4, L
    opcode_set(4, l);
}

void CPU::opcode_CB_E6(){ // SET 4, (HL)
    opcode_set(4, Address(getHL()));
}

void CPU::opcode_CB_E7(){ // SET 4, A
    opcode_set(4, a);
}

void CPU::opcode_CB_E8(){ // SET 5, B
    opcode_set(5, b);
}

void CPU::opcode_CB_E9(){ // SET 5, C
    opcode_set(5, c);
}

void CPU::opcode_CB_EA(){ // SET 5, D
    opcode_set(5, d);
}

void CPU::opcode_CB_EB(){ // SET 5, E
    opcode_set(5, e);
}

void CPU::opcode_CB_EC(){ // SET 5, H
    opcode_set(5, h);
}

void CPU::opcode_CB_ED(){ // SET 5, L
    opcode_set(5, l);
}

void CPU::opcode_CB_EE(){ // SET 5, (HL)
    opcode_set(5, Address(getHL()));
}

void CPU::opcode_CB_EF(){ // SET 5, A
    opcode_set(5, a);
}

void CPU::opcode_CB_F0(){ // SET 6, B
    opcode_set(6, b);
}

void CPU::opcode_CB_F1(){ // SET 6, C
    opcode_set(6, c);
}

void CPU::opcode_CB_F2(){ // SET 6, D
    opcode_set(6, d);
}

void CPU::opcode_CB_F3(){ // SET 6, E
    opcode_set(6, e);
}

void CPU::opcode_CB_F4(){ // SET 6, H
    opcode_set(6, h);
}

void CPU::opcode_CB_F5(){ // SET 6, L
    opcode_set(6, l);
}

void CPU::opcode_CB_F6(){ // SET 6, (HL)
    opcode_set(6, Address(getHL()));
}

void CPU::opcode_CB_F7(){ // SET 6, A
    opcode_set(6, a);
}

void CPU::opcode_CB_F8(){ // SET 7, B
    opcode_set(7, b);
}

void CPU::opcode_CB_F9(){ // SET 7, C
    opcode_set(7, c);
}

void CPU::opcode_CB_FA(){ // SET 7, D
    opcode_set(7, d);
}

void CPU::opcode_CB_FB(){ // SET 7, E
    opcode_set(7, e);
}

void CPU::opcode_CB_FC(){ // SET 7, H
    opcode_set(7, h);
}

void CPU::opcode_CB_FD(){ // SET 7, L
    opcode_set(7, l);
}

void CPU::opcode_CB_FE(){ // SET 7, (HL)
    opcode_set(7, Address(getHL()));
}

void CPU::opcode_CB_FF(){ // SET 7, A
    opcode_set(7, a);
}