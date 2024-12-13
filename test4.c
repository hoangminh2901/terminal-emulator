#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

int main() {
    syscall(9999); // Invalid system call number causes bad system call
    return 0;
}
