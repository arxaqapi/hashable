#include <math.h>

#include "prime.h"

/**
 * @brief Returns wethever x is prime or not
 * 
 * @param x the number to verify
 * @return uint32_t: 1 - prime | 0 - not prime | -1 - undefined (i.e. x < 2)
 */
uint32_t
is_prime(const uint32_t x)
{
    if (x < 2)
        return -1;
    if (x < 4)
        return 1;
    if (x % 2 == 0)
        return 0;

    for (uint32_t i = 3; i <= floor(sqrt((double) x)); i += 2)
    {
        // can be divised, means that it is a multiple of i, so not prime
        if(x % i == 0)
            return 0;
    }
    return 1;
}

/**
 * @brief Returns the next prime greater or equal than x
 * 
 * @param x The base number to get the next prime from
 * @return uint32_t the next prime
 */
uint32_t
next_prime(uint32_t x)
{
    while (is_prime(x) != 1)
    {
        x ++;
    }
    return x;
}