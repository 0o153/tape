#include "../tape.h"


#define N 1000000


int main() {
    memory_t mem;
    wind(&mem, 1024 * 256);

    for (int i = 0; i < N; i++) {
        Unit x = pull(&mem);
        x->data = i;
        bail(x, &mem);
        x = pull(&mem);
    }

    wipe(&mem);

    return 0;
}
