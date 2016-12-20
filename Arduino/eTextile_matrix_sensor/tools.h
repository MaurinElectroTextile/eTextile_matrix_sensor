#include <math.h>

/* source:
https://graphics.stanford.edu/~seander/bithacks.html

Google scholar citation:
@article{anderson3bit,
  title={Bit twiddling hacks, 1997-2005},
  author={Anderson, Sean Eron},
  journal={URL: https://graphics.stanford.edu/~seander/bithacks.html},
}
*/

// all of these optimizations are useful - to test on different CPUs:
#define NO_OPTIM    0
#define LUT         1
#define LUT_UNIFORM 2
#define OPTIM1      3
#define OPTIM2      4
#define LOG10       5

#define METHOD 1


inline uint32_t log2optim(uint32_t v)
{
    uint32_t r = 0; // return value: log2(v)

#if METHOD == NO_OPTIM

    r = log(v);

#elif METHOD == LUT

    // source: https://graphics.stanford.edu/~seander/bithacks.html#IntegerLogLookup
    static const char LogTable256[256] = {
        #define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
        -1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
        LT(4), LT(5), LT(5), LT(6), LT(6), LT(6), LT(6),
        LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7)
    };

    register uint32_t t, tt; // temporaries

    if (tt = v >> 16) {
        r = (t = tt >> 8) ? 24 + LogTable256[t] : 16 + LogTable256[tt];
    }
    else {
        r = (t = v >> 8) ? 8 + LogTable256[t] : LogTable256[v];
    }

#elif METHOD == LUT_UNIFORM

    static const char LogTable256[256] = {
        #define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
        -1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
        LT(4), LT(5), LT(5), LT(6), LT(6), LT(6), LT(6),
        LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7)
    };

    register uint32_t t, tt; // temporaries

    if (tt = v >> 24) {
        r = 24 + LogTable256[tt];
    }
    else if (tt = v >> 16) {
        r = 16 + LogTable256[tt];
    }
    else if (tt = v >> 8) {
        r = 8 + LogTable256[tt];
    }
    else {
        r = LogTable256[v];
    }

#elif METHOD == OPTIM1

    // source: https://graphics.stanford.edu/~seander/bithacks.html#IntegerLog
    const uint32_t b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    const uint32_t S[] = {1, 2, 4, 8, 16};

    for (int i = 4; i >= 0; i--) { // unroll for speed...
        if (v & b[i]) {
            v >>= S[i];
            r |= S[i];
        }
    }

#elif METHOD == OPTIM2

    // Better as the CPU branches slowly
    register uint32_t shift;
    r =     (v > 0xFFFF) << 4; v >>= r;
    shift = (v > 0xFF  ) << 3; v >>= shift; r |= shift;
    shift = (v > 0xF   ) << 2; v >>= shift; r |= shift;
    shift = (v > 0x3   ) << 1; v >>= shift; r |= shift;

#elif METHOD == LOG10

    // source: https://graphics.stanford.edu/~seander/bithacks.html#IntegerLog10Obvious
    r = (v >= 1000000000) ? 9 : (v >= 100000000) ? 8 : (v >= 10000000) ? 7 :
        (v >= 1000000) ? 6 : (v >= 100000) ? 5 : (v >= 10000) ? 4 :
        (v >= 1000) ? 3 : (v >= 100) ? 2 : (v >= 10) ? 1 : 0;

#endif

    return r;
}
