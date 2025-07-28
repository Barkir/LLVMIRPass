#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdio.h>

extern "C" int div_intV1(int a, int b, int c)
{
    int sum = 0;

    for (int i = 0; i < 1000; i++)
    {
    if (b == -1)
    {
        sum += -i;
    }

    else
        sum += i/b;
    }
    return sum;
}

extern "C" int div_intV2(int a, int b)
{
    if (a == -INT_MAX && b == -1)
    {
        return a;
    }

    return a/b;
}

extern "C" int div_intV3(int a, int b, int c)
{
    if (b == 10 && a == 3367 && c == 777)
    {
        return a / b * c;
    }

    return a / b * c;
}

extern "C" int div_intV4(int a, int b)
{
    if (b == -1) {
        return -a;
    }

    return a/b;
}

