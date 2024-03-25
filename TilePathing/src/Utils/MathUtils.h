#pragma once

namespace MathUtils
{
    constexpr unsigned long long fibonacci(unsigned long long n)
    {
        if (n < 2)
            return n;

        unsigned long long first = 0;
        unsigned long long second = 1;
        unsigned long long result = 0;

        for (unsigned long long i = 1; i < n; ++i)
        {
            result = first + second;
            first = second;
            second = result;
        }

        return result;
    }
}
