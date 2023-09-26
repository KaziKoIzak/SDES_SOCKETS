#include <stdio.h>

long long fastModExpo(long long base, long long exponent, long long modulus)
{
    long long result = 1;
    base = base % modulus;

    while (exponent > 0)
    {
        if (exponent % 2 == 1)
        {
            result = (result * base) % modulus;
        }
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }

    return result;
}

long long FME(long long base, long long exponent, long long modulus)
{
    base = 181;
    exponent = 107;
    modulus = 257;

    long long result = fastModExpo(base, exponent, modulus);

    printf("%lld^%lld mod %lld = %lld\n", base, exponent, modulus, result);

    return result;
}
