#include <stdio.h>

unsigned int fastModExpo(unsigned int base, unsigned int exponent, unsigned int modulus)
{
    unsigned int result = 1;
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

unsigned int FME(unsigned int base, unsigned int exponent, unsigned int modulus)
{
    unsigned int result = fastModExpo(base, exponent, modulus);

    printf("%u^%u mod %u = %u\n", base, exponent, modulus, result);

    return result;
}
