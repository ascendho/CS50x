#include <stdint.h>
#include <stdio.h>
#include "Instr.h"

/// @brief ALU 执行算术/逻辑运算，根据指令类型计算结果
/// @param inst 指向译码结果结构体的指针
/// @param input1 ALU 第一个输入参数
/// @param input2 ALU 第二个输入参数
/// @return ALU 计算结果
uint64_t alu_cal(const instr_t *inst, uint64_t input1, uint64_t input2)
{
    int64_t s1 = *(int64_t *)&input1; // 视为有符号数
    int64_t s2 = *(int64_t *)&input2; // 视为有符号数

    switch (inst->opcode)
    {
        // Load/Store/分支/跳转/lui 指令：需计算有效地址或直接加法
        case 0x03: // Load
        case 0x23: // Store
        case 0x63: // Branch
        case 0x6f: // jal
        case 0x67: // jalr
        case 0x37: // lui
            return input1 + input2;

        case 0x17: // auipc：PC + 立即数
            return input1 + input2;

        // R 型/I 型指令：算术/逻辑运算（由 funct3/funct7 细分）
        case 0x33: // R-Type
        case 0x13: // I-Type
            switch (inst->funct3)
            {
                case 0x0: // add/sub/addi
                    switch (inst->funct7)
                    {
                        case 0x20: return input1 - input2; // sub
                        default:    return input1 + input2; // add/addi
                    }
                case 0x1: // sll/slli（逻辑左移）
                    input2 = input2 & 0x1f; // 取移位量低 5 位
                    return input1 << input2;
                case 0x2: // slt/slti（有符号小于）
                    return (s1 < s2) ? 1 : 0;
                case 0x3: // sltu/sltiu（无符号小于）
                    return (input1 < input2) ? 1 : 0;
                case 0x4: // xor/xori（异或）
                    return input1 ^ input2;
                case 0x5: // srl/sra/srli/srai（右移）
                    input2 = input2 & 0x1f; // 取移位量低 5 位
                    switch (inst->funct7)
                    {
                        case 0x00: return input1 >> input2; // srl/srli（逻辑右移）
                        case 0x20: return s1 >> input2;    // sra/srai（算术右移）
                        default:   goto UNKNOWN;
                    }
                case 0x6: // or/ori（或）
                    return input1 | input2;
                case 0x7: // and/andi（与）
                    return input1 & input2;
                default: goto UNKNOWN;
            }
        default: goto UNKNOWN;
    }

UNKNOWN:
    printf("Unknown instruction to ALU.\n");
    exit(0);
}