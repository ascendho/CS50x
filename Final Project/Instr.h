#include <stdint.h>

typedef struct INSTR_STRUCT {
    uint32_t opcode;   // 指令操作码
    uint32_t funct3;   // 次级功能码（若不存在则为0）
    uint32_t funct7;   // 扩展功能码（若不存在则为0）
    uint32_t rd;       // 目标寄存器 RD 的编号
    uint64_t rs1_val;  // 源寄存器 RS1 的数值
    uint64_t rs2_val;  // 源寄存器 RS2 的数值
    int32_t imm;       // 经32位符号扩展后的立即数
} instr_t;