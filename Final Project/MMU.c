#include <stdint.h>

extern const uint64_t MEMORY_SIZE;

/// @brief Load 指令专用：虚拟地址转物理地址
/// @param vaddr 虚拟地址
/// @return 物理地址
uint64_t va2pa_l(uint64_t vaddr) { return vaddr % MEMORY_SIZE; }

/// @brief Store 指令专用：虚拟地址转物理地址
/// @param vaddr 虚拟地址
/// @return 物理地址
uint64_t va2pa_s(uint64_t vaddr) { return vaddr % MEMORY_SIZE; }