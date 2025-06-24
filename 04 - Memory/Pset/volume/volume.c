// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // 字符串转换为float
    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    // 无符号类型，8位（一字节）
    uint8_t header[HEADER_SIZE];

    // 单纯的复制拷贝工作，理论上也可以用int8_t，但是不推荐
    fread(header, HEADER_SIZE, 1, input);
    fwrite(header, HEADER_SIZE, 1, output);

    // TODO: Read samples from input file and write updated data to output file
    // 有符号类型，16位（两字节），
    // 在栈上（而不是堆）分配内存，由系统自动管理，不需要用malloc手动分配内存
    int16_t buffer;
    while (fread(&buffer, sizeof(int16_t), 1, input))
    {
        // 参与运算，必须用有符号类型，无符号类型在解析数值时会发生错误
        buffer *= factor;
        fwrite(&buffer, sizeof(int16_t), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
