#include "common.h"
#include <unistd.h>

number test(number n) {
    if (n == 0) return 2;
    if (n == 1) return 3;
    if (n == 2) return 5;
    number primes[n];
    primes[0] = 2;
    primes[1] = 3;
    primes[2] = 5;
    number *prime = primes + 3;
    number *primeend = primes + n;
    number *primecheckend = primes + 3;
    int iter = 2;
    do {
        *prime = *(prime - 1);
        l_fail: { // wait until found another prime
            *prime += iter;
            iter = 6 - iter;
            number *primecheck;
            primecheck = primes;
            do {
                if (primecheck > primecheckend) {
                    if (*primecheck * *primecheck > *prime)
                        break;
                    primecheckend = primecheck;
                }
                if (*prime % *primecheck == 0)
                    goto l_fail;
                ++primecheck;
            } while (primecheck != prime);
        }
        ++prime;
    } while (prime < primeend);
    return *(primeend - 1);
}