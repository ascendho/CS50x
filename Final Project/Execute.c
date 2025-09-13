#include <stdint.h>
#include <stdlib.h>
#include "Instr.h"

// 外部依赖：由其他源文件实现
extern uint64_t alu_cal(const instr_t *inst, uint64_t input1, uint64_t input2);
extern uint64_t branch_cal(const instr_t *inst);

/// @brief MUX - 多路选择器：选择ALU的两个输入
/// @param inst 指向译码结果的指针
/// @param npc NPC的数值
/// @param input1 MUX选择的结果：RS1数值、NPC
/// @param input2 MUX选择的结果：RS2数值、IMM
static void select_inputs(const instr_t *inst, uint64_t npc, 
                          uint64_t *input1, uint64_t *input2)
{
    // 立即数扩展到64位有符号数，方便作为ALU输入
    int64_t simm = (int64_t)(inst->imm);
    switch (inst->opcode)
    {
        case 0x33: // R 类型：rs1, rs2
            *input1 = inst->rs1_val;
            *input2 = inst->rs2_val;
            break;
        case 0x03: // Load：rs1, imm
        case 0x13: // I 类型：rs1, imm
        case 0x67: // jalr：rs1, offset
            *input1 = inst->rs1_val;
            *input2 = *(uint64_t *)(&simm);
            break;
        case 0x63: // B 类型：npc, imm
        case 0x17: // U 类型（auipc）：npc, imm
        case 0x6f: // J 类型：npc, imm
            *input1 = npc;
            *input2 = *(uint64_t *)(&simm);
            break;
        case 0x37: // U 类型（lui）：--, imm
            *input1 = 0;
            *input2 = (uint64_t *)(&simm);
            break;
        default:
            break;
    }
}

/// @brief 执行：完成ALU、条件分支的计算
/// @param inst 指向指令译码的指针
/// @param npc NPC的数值
/// @param condition 指向条件分支结果的指针
/// @return ALU计算结果
uint64_t instruction_execute(const instr_t *inst, uint64_t npc, uint64_t *condition)
{
    // 存放MUX选择结果，亦即ALU输入的临时寄存器
    uint64_t input1, input2;
    select_inputs(inst, npc, &input1, &input2);
    // ALU根据opcode、funct3/7进行计算
    uint64_t output = alu_cal(inst, input1, input2);
    // 根据opcode、funct3/7计算条件分支是否成立
    if (inst->opcode == 0x63)
        *condition = branch_cal(inst);
    else
        *condition = 0;
    return output;
}