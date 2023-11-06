#include "common.h"

number test(number n) {
    number prime = 0;
    number num = 1;
    while (prime < n) {
        // check if n is a prime
        for (number i = 2; i < num; ++i) {
            if (num % i == 0)
                goto l_fail;
        }
        prime += 1;
        l_fail:
        num += 1;
    }
    return num - 1;
}