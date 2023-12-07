#ifndef _RSA_H
#define _RSA_H

#include "FME.h"

unsigned int basicallyRSA(unsigned int p, unsigned int q)
{
    unsigned int totient_n = (p - 1) * (q - 1);

    unsigned int e = find_smallest_relative_prime(totient_n);

    return e;
}

int DRSA(unsigned int p, unsigned int q, unsigned int e)
{
    unsigned int totient_n = (p - 1) * (q - 1);

    int d = modInverse(e, totient_n);

    return d;
}

#endif