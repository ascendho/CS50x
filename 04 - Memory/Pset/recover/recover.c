#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// 块大小
const int BLOCK = 512;

// 文件名大小，包含 '/0'
const int fileNameLength = 8;

// 重命名类型
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");

    if (card == NULL)
        return 1;

    // Create a buffer for a block of data
    BYTE buffer[BLOCK];

    int count = 0;
    FILE *img = NULL;

    // While there's still data left to read from the memory card
    while (fread(buffer, 1, BLOCK, card) == BLOCK)
    {
        // 注意第4个字节的判断，用到了比特运算，以及 & 与 == 运算符的优先级
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            ((buffer[3] & 0xf0) == 0xe0))
        {
            // 关闭之前打开的文件，防止下面打开新的文件造成冲突
            if (img != NULL)
                fclose(img);

            // 文件名为8个char，因为‘/0’也占据一位
            char fileName[fileNameLength];
            sprintf(fileName, "%03i.jpg", count++);

            // 打开新文件
            img = fopen(fileName, "w");
        }

        // 写数据
        if (img != NULL)
            fwrite(buffer, 1, BLOCK, img);
    }

    // 关闭文件
    if (img != NULL)
        fclose(img);

    fclose(card);
}
