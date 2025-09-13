#include <stdint.h>
#include "Instr.h"

/// @brief 译码：将IR寄存器的指令数据解析为各字段
/// @param ir IR寄存器中的指令数据（64位）
/// @param reg 指向通用寄存器数组的指针
/// @param inst 指向译码结果结构体的指针
void instruction_decode(const uint64_t ir, const uint64_t *reg, instr_t *inst)
{
    // 提取固定字段（opcode、funct3、funct7）
    inst->opcode = ir & 0x7f;
    inst->funct3 = (ir >> 12) & 0x7;
    inst->funct7 = (ir >> 25) & 0x7f;

    // 读取源寄存器 RS1、RS2 的值
    inst->rs1_val = reg[(ir >> 15) & 0x1f];
    inst->rs2_val = reg[(ir >> 20) & 0x1f];

    // 符号扩展立即数（基于指令格式区分）
    int32_t signed_ir = *(int32_t *)&ir;

    switch (inst->opcode)
    {
    case 0x03: // Load 指令（I 型）
    case 0x13: // 立即数算术逻辑运算（I 型）
    case 0x67: // jalr 跳转（I 型）
        inst->imm = signed_ir >> 20;
        break;
    case 0x23: // Store 指令（S 型）
        inst->imm = (signed_ir >> 20) & 0xfffffe0;
        inst->imm |= (signed_ir >> 7) & 0x0000001f;
        break;
    case 0x63: // 条件分支（B 型）
        inst->imm = (signed_ir >> 19) & 0xfffe000;
        inst->imm |= (signed_ir >> 4) & 0x0000800;
        inst->imm |= (signed_ir << 4) & 0xfffffe0;
        inst->imm |= (signed_ir >> 7) & 0x0000001e;
        break;
    case 0x37: // lui（U 型）
    case 0x17: // auipc（U 型）
        inst->imm = signed_ir & 0xfffff000;
        break;
    case 0x6f: // jal（J 型）
        inst->imm = (signed_ir >> 11) & 0xfff0000;
        inst->imm |= (signed_ir >> 1) & 0x000ff000;
        inst->imm |= (signed_ir << 4) & 0x00000800;
        inst->imm |= (signed_ir >> 20) & 0x000007fe;
        break;
    default:
        inst->imm = 0;
        break;
    }
}