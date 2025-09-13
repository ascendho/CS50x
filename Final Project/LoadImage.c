#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
// 外部依赖：由其他源文件实现
extern uint8_t *mem;
extern uint64_t va2pa_s(uint64_t vaddr);
extern uint64_t *xreg;
extern uint64_t pcnt;
/// @brief 将二进制文件中的指令数据加载到内存中
/// @param file 二进制文件的路径
/// @param text_start 指令在内存中的起始虚拟地址
/// @param stack_start 栈的起始虚拟地址
/// @return 加载的指令总字节数
int load_image(const char *file, uint64_t text_start, uint64_t stack_start)
{
    // 以二进制读模式打开文件
    FILE *image = fopen(file, "rb");
    if (image == NULL)
    {
        printf("Cannot open file: '%s'\n", file);
        exit(1);
    }
    // 逐 4 字节读取指令并加载到内存
    uint32_t buf, text_size = 0, s = 0;
    while (1)
    {
        s = fread(&buf, sizeof(uint32_t), 1, image);
        if (s == 1)
        {
            // 计算虚拟地址 -> 翻译为物理地址 -> 写入内存
            uint64_t vaddr = text_start + text_size;
            *((uint32_t *)(&mem[va2pa_s(vaddr)])) = buf;
            text_size += sizeof(uint32_t);
        }
        else
            break;
    }
    // 初始化程序计数器（PC）和栈指针（x2，即 sp）
    pcnt = text_start;
    xreg[2] = stack_start;
    fclose(image);
    return text_size;
}