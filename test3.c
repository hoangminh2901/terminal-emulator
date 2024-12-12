#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

int main() {
    struct rlimit rl;
    rl.rlim_cur = 1024; // Set file size limit to 1 KB
    rl.rlim_max = 1024;
    setrlimit(RLIMIT_FSIZE, &rl);

    FILE *fp = fopen("large_file.txt", "w");
    if (fp == NULL) {
        perror("File open error");
        return 1;
    }

    for (int i = 0; i < 2000; i++) {
        fprintf(fp, "This line exceeds the file size limit.\n");
    }
    fclose(fp);
    return 0;
}
