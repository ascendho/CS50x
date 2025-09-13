#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// 物理内存：大小定义
#define MEM_SIZE (65536)

/* static变量仅当前源文件可见，不对外暴露 */
static uint8_t mem_static[MEM_SIZE];
// 通用寄存器组 x0 - x31
static uint64_t xreg_static[32];

/* 以下变量暴露给其他源文件 */
const uint64_t MEMORY_SIZE = MEM_SIZE;
// 程序计数器
uint64_t pcnt;
// 内存、寄存器以指针形式供外部访问
uint8_t *mem = mem_static;
uint64_t *xreg = xreg_static;

/// @brief 打印所有通用寄存器的值
void dump_registers()
{
    for (int i = 0; i < 32; ++i)
    {
        printf("x[%2d] %016" PRIx64 " %22" PRIu64 "\n", i, xreg[i], xreg[i]);
    }
}