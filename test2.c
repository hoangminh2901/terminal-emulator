// Illegal Instruction
#include <stdio.h>

int main() {
    void (*illegal_function)() = (void(*)())0xdeadbeef; // Invalid memory address
    illegal_function();
    return 0;
}
