#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int main(int argc, const char **argv)
{
    // 命令行参数需为：CreateBin 输入文本文件 输出二进制文件
    if (argc != 3) return 0;
    FILE *infile = fopen(argv[1], "r");
    FILE *outfile = fopen(argv[2], "wb");
    char buf[128];

    // 逐行读取十六进制指令文本，转换为 uint32_t 后写入二进制文件
    while (fgets(buf, 128, infile) != NULL)
    {
        uint32_t instruction = strtoul(buf, NULL, 16);
        fwrite(&instruction, 1, 4, outfile);
    }

    fclose(infile);
    fclose(outfile);
    return 0;
}