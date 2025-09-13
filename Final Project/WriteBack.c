#include <stdint.h>
#include "Instr.h"

/// @brief 将 ALU 结果或 LMD 写回目标寄存器
/// @param inst 指向译码指令结果的指针
/// @param reg 指向寄存器组的指针
/// @param alu_output ALU 计算结果
/// @param lmd 内存中读取到的数据
/// @param nextpc 下一条指令地址（PC + 4）
void write_back(const instr_t *inst, uint64_t *reg,
                const uint64_t alu_output, const uint64_t lmd, const uint64_t nextpc)
{
    if (inst->rd == 0)
    {
        // x0 寄存器硬连线为 0，不写入任何数据
        return;
    }
    switch (inst->opcode)
    {
        // R 型、I 型（立即数运算）、U 型（auipc/lui）：写回 ALU 结果
        case 0x33: // R 型：op rd, rs1, rs2
        case 0x13: // I 型：op rd, rs1, imm
        case 0x17: // U 型：auipc rd, imm
        case 0x37: // U 型：lui rd, imm
            reg[inst->rd] = alu_output;
            break;
        // J 型（jal）、I 型（jalr）：写回下一条指令地址（PC + 4）
        case 0x6f: // J 型：jal rd, imm
        case 0x67: // I 型：jalr rd, offset(rs1)
            reg[inst->rd] = nextpc;
            break;
        // Load 指令：写回内存加载的数据（LMD）
        case 0x03:
            reg[inst->rd] = lmd;
            break;
        default:
            break;
    }
}