//segmentation fault
#include <stdio.h>

int main() {
    int *ptr = NULL; // Null pointer
    *ptr = 42;       // Dereferencing null pointer
    return 0;
}

