#include <stdio.h>
#include <stdlib.h>

int codegen_func_s(int a0, int a1, int a2, int a3,
                  int a4, int a5, int a6, int a7);

int main(int argc, char *argv[]) {
    int a[8];
    int i;
    int r;

    // Initialize all 8 args to 0 to be passed to codegen_func
    for (i = 0; i < 8; i++) {
        a[i] = 0;
    }

    // Populate args with up to 8 args from command line
    for (i = 1; i < argc && i < 8; i++) {
        a[i - 1] = atoi(argv[i]);
    }

    r = codegen_func_s(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);

    printf("%d (0x%X)\n", r, r);

    return 0;
}
