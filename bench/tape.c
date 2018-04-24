#include "../tape.h"


#define N 1000000


int main() {
    Tape tape;
    wind(&tape, 1024 * 256);

    for (int i = 0; i < N; i++) {
        Unit x = pull(&tape);
        x->data.k = i;
        bail(x, &tape);
        x = pull(&tape);
    }

    wipe(&tape);

    return 0;
}
