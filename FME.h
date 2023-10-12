#ifndef _FME_H
#define _FME_H

#include <stdio.h>

unsigned int gcd(unsigned int a, unsigned int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

unsigned int findSmallestRelativelyPrime(unsigned int n) {
    unsigned int x = 2;

    while (gcd(n, x) != 1) {
        x++;
    }

    return x;
}

unsigned int extendedGCD(unsigned int a, unsigned int b, unsigned int *x, unsigned int *y) {
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }

    unsigned int x1, y1;
    unsigned int gcde = extendedGCD(b % a, a, &x1, &y1);

    *x = y1 - (b / a) * x1;
    *y = x1;

    return gcde;
}

unsigned int modInverse(unsigned int a, unsigned int m) {
    unsigned int x, y;
    unsigned int gcde = extendedGCD(a, m, &x, &y);

    if (gcde != 1) {
        // Modular inverse does not exist
        return -1;
    }

    // Ensure the result is positive
    return (x % m + m) % m;
}

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

#endif