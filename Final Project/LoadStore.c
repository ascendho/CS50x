#include <stdint.h>
// va2pa 的函数指针类型，用于函数指针参数传递
typedef uint64_t (*va2pa_t)(uint64_t);

/// @brief 将数据写入内存有效地址
/// @param val 待写入内存的数据
/// @param vaddr 内存有效地址（虚拟地址）
/// @param funct3 用于确定写入数据的宽度（b/h/w 等）
/// @param mem 指向物理内存的指针
/// @param va2pa_store 用于 Store 的虚拟地址转物理地址翻译函数
void store_data(uint64_t val, uint64_t vaddr, uint32_t funct3,
                uint8_t *mem, va2pa_t va2pa_store)
{
    // MMU 将虚拟地址翻译为物理地址
    uint64_t paddr = va2pa_store(vaddr);
    // 计算数据宽度（1<<(funct3&3) 得到 1/2/4 字节）
    int width = 1 << (funct3 & 3);
    // 以字节数组形式按顺序写入物理内存（小端机器）
    uint8_t *byte_arr = (uint8_t *)&val;
    for (int i = 0; i < width; ++i)
    {
        mem[paddr + i] = byte_arr[i];
    }
}

/// @brief 从内存有效地址中加载数据
/// @param vaddr 内存有效地址（虚拟地址）
/// @param funct3 用于确定加载数据的宽度（b/h/w 等）
/// @param mem 指向物理内存的指针
/// @param va2pa_load 用于 Load 的虚拟地址转物理地址翻译函数
/// @return 内存中加载的数据（待写入 LMD 寄存器）
uint64_t load_data(uint64_t vaddr, uint32_t funct3, const uint8_t *mem,
                   va2pa_t va2pa_load)
{
    // MMU 将虚拟地址翻译为物理地址
    uint64_t paddr = va2pa_load(vaddr);
    // 计算数据宽度
    int width = 1 << (funct3 & 3);
    // 判断是否为无符号加载（用于符号扩展）
    int udata = (funct3 >> 2) & 1;
    // 以字节数组形式按顺序加载物理内存（小端机器）
    uint8_t buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < width; ++i)
    {
        buffer[i] = mem[paddr + i];
    }
    // 有符号数的符号扩展（若需扩展）
    if (udata == 0 && (buffer[width - 1] & 0x80) == 0x80)
    {
        for (int i = width; i < 8; ++i)
        {
            buffer[i] = 0xff;
        }
    }
    return *(uint64_t *)&buffer;
}