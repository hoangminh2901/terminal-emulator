// Division by zero causes floating-point exception
#include <stdio.h>

int main() {
    int x = 1;
    int y = 0;
    int result = x / y;
    printf("Result: %d\n", result);
    return 0;
}
