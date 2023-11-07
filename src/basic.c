#include "common.h"

number test(number n) {
    if (n == 0) return 1;
    n += 1;
    number prime = 0;
    number num = 0;
    do {
        l_fail:
        num += 1;
        // check if n is a prime
        for (number i = 2; i < num; ++i) {
            if (num % i == 0)
                goto l_fail;
        }
        prime += 1;
    } while (prime < n);
    return num;
}