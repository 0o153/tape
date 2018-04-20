#include <stdlib.h>
#include "../tape.h"


#define N 1000000

#define UNIT_SZ sizeof(struct unit_t)


int main() {
    Unit mem[N];

    for (int i = 0; i < N; i++) {
        Unit x = malloc(UNIT_SZ);
        x->data = i;
        free(x);
        x = malloc(UNIT_SZ);
        mem[i] = x;
    }

    for (int i = 0; i < N; i++) {
        free(mem[i]);
    }

    return 0;
}
