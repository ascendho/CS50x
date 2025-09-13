#include <stdint.h>
#include "Instr.h"

// 外部依赖：由其他源文件实现
typedef uint64_t (*va2pa_t)(uint64_t);
extern uint64_t load_data(uint64_t vaddr, uint32_t funct3, const uint8_t *mem, va2pa_t va2pa_load);
extern void store_data(uint64_t val, uint64_t vaddr, uint32_t funct3, uint8_t *mem, va2pa_t va2pa_store);
extern uint64_t update_pc(const instr_t *inst, uint64_t npc, uint64_t target, int condition);

/// @brief 模拟“访存（MEM）”阶段：处理 Load/Store 指令，更新程序计数器
/// @param inst 译码结果结构体指针
/// @param alu_output ALU 计算结果（如有效地址）
/// @param condition 条件分支结果
/// @param npc 下一条指令的默认地址（PC+4）
/// @param mem 物理内存指针
/// @param va2pa_load 虚拟地址转物理地址（Load 用）的函数指针
/// @param va2pa_store 虚拟地址转物理地址（Store 用）的函数指针
/// @param lmd 存储 Load 结果的寄存器指针
/// @return 下一条指令的虚拟地址
uint64_t memory_access(const instr_t *inst, uint64_t alu_output, int condition,
                       uint64_t npc, uint8_t *mem, va2pa_t va2pa_load, va2pa_t va2pa_store,
                       uint64_t *lmd)
{
    switch (inst->opcode)
    {
        case 0x03: // Load 指令：从内存读取数据到 LMD
            *lmd = load_data(alu_output, inst->funct3, mem, va2pa_load);
            break;
        case 0x23: // Store 指令：将寄存器数据写入内存
            store_data(inst->rs2_val, alu_output, inst->funct3, mem, va2pa_store);
            break;
        default: // 非 Load/Store 指令，不操作内存
            break;
    }
    // 调用 update_pc 决定下一条指令的地址
    return update_pc(inst, npc, alu_output, condition);
}