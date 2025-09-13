#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

/// @brief 检查32位无符号数相加(uint32_t+uint32_t)是否溢出
/// @param x 32位无符号数
/// @param y 32位无符号数
/// @return 如果无符号数溢出，返回1。不溢出，返回0
int uint32_overflow(uint32_t x, uint32_t y)
{
    return (x + y) < x;
}

/// @brief 检查32位有符号数相加(int32_t+int32_t)是否溢出
/// @param x 32位有符号数
/// @param y 32位有符号数
/// @return 如果有符号数溢出，返回1。不溢出，返回0
int int32_overflow(int32_t x, int32_t y)
{
    return (x >= 0 && y >= 0 && (x + y) < 0) ||
           (x < 0 && y < 0 && (x + y) >= 0);
}

/// @brief 主函数
/// @param argc 命令行参数个数
/// @param argv 命令行字符串参数
/// @return 主程序退出状态
int main(int argc, const char **argv)
{
    // 按照16进制读取两个字节，作为内存上储存的比特串
    uint32_t x = strtol(argv[1], NULL, 16);
    uint32_t y = strtol(argv[2], NULL, 16);
    // 将x, y视为有符号数进行有符号数
    int32_t *xp = (int32_t *)&x;
    int32_t *yp = (int32_t *)&y;
    
    // 检查无符号数是否溢出
    if (uint32_overflow(x, y) == 1)
        printf("Unsigned: %"PRIu32" Overflow\n", x + y);
    else
        printf("Unsigned: %"PRIu32" No overflow\n", x + y);
    
    // 检查有符号数是否溢出  
    if (int32_overflow(*xp, *yp) == 1)
        printf("Signed: %"PRId32" Overflow\n", *xp + *yp);
    else
        printf("Signed: %"PRId32" No overflow\n", *xp + *yp);

    return 0;
}
