#include <stdio.h>

long long mod_pow(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;

    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }

    return result;
}

int main() {
    long long C = 1111;
    long long N = 2701;
    long long e = 19;

    long long p = 37;
    long long q = 73;
    long long phi = (p - 1) * (q - 1);
    long long d = 955;

    long long M = mod_pow(C, d, N);

    printf("p = %lld \n", p);
    printf("q = %lld \n", q);
    printf("phi(N) = %lld \n", phi);
    printf("e = %lld \n", e);
    printf("d = %lld \n", d);
    printf("M = C^d mod N = %lld \n", M);

    return 0;
}
