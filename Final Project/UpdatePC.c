#include <stdint.h>
#include "Instr.h"

/// @brief 更新程序计数器（PC）：处理顺序执行、条件分支、无条件跳转
/// @param inst 译码结果结构体指针
/// @param nextpc 顺序执行的下一条地址（PC+4）
/// @param target ALU 计算的跳转目标地址
/// @param condition 条件分支是否成立（1 为成立，0 为不成立）
/// @return 下一条指令的虚拟地址
uint64_t update_pc(const instr_t *inst, uint64_t nextpc, uint64_t target, uint64_t condition)
{
    switch (inst->opcode)
    {
        case 0x63: // 条件分支指令（B 类型）：分支成立则跳转到 target，否则顺序执行
            return condition == 1 ? target : nextpc;
        case 0x67: // jalr 指令（无条件跳转）：跳转到 target
        case 0x6f: // jal 指令（无条件跳转）：跳转到 target
            return target;
        default: // 普通指令：顺序执行（PC+4）
            return nextpc;
    }
}