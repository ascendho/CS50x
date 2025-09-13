#include <stdint.h>
#include <stdlib.h>
#include "Instr.h"

/// @brief 根据寄存器数值计算条件分支是否成立
/// @param inst 指向译码结果结构体的指针
/// @return 条件成立返回 1，否则返回 0
uint64_t branch_cal(const instr_t *inst)
{
    uint64_t u1 = inst->rs1_val;
    uint64_t u2 = inst->rs2_val;
    int64_t s1 = *(int64_t *)&u1;  // 转换为有符号数
    int64_t s2 = *(int64_t *)&u2;  // 转换为有符号数

    switch (inst->funct3)
    {
        case 0x0: return s1 == s2; // beq（相等则分支）
        case 0x1: return s1 != s2; // bne（不相等则分支）
        case 0x4: return s1 < s2;  // blt（有符号小于则分支）
        case 0x5: return s1 >= s2; // bge（有符号大于等于则分支）
        case 0x6: return u1 < u2;  // bltu（无符号小于则分支）
        case 0x7: return u1 >= u2; // bgeu（无符号大于等于则分支）
        default: exit(1); // 未知 funct3，异常退出
    }
}