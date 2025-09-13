#include <stdint.h>
typedef uint64_t (*va2pa_t)(uint64_t);
/// @brief 取指：根据 Pc，将指令数据从内存加载到 IR 中
/// @param pc 程序计数器 PC 的值
/// @param mem 内存数组指针
/// @param va2pa_load 虚拟地址到物理地址的映射函数
/// @param npc NPC（下一条指令地址）的指针
/// @return 应当写入 IR 寄存器的指令数据（32 位）
uint32_t instruction_fetch(uint64_t pc, const uint8_t *mem, 
                          va2pa_t va2pa_load, uint64_t *npc)
{
    // 更新 NPC（默认下一条指令地址为 PC + 4）
    *npc = pc + 4;
    // 虚拟地址翻译 -> 物理地址 -> 读取指令内存（按 32 位读取）
    return *(((uint32_t *)(&mem[va2pa_load(pc)])));
}