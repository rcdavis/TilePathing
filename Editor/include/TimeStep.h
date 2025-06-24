#pragma once

#include "Core.h"

class TimeStep
{
public:
    constexpr TimeStep(const f32 time = 0.0f) :
        mTime(time) {}
    constexpr TimeStep(const TimeStep& ts) :
        mTime(ts.mTime) {}

    constexpr operator f32() const { return mTime; }
    constexpr f32 GetSeconds() const { return mTime; }
    constexpr f32 GetMilliseconds() const { return mTime * 1000.0f; }

    constexpr TimeStep& operator=(const TimeStep& ts)
    {
        if (this == &ts)
            return *this;

        mTime = ts.mTime;
        return *this;
    }

    constexpr TimeStep& operator+=(const TimeStep& rhs)
    {
        mTime += rhs.mTime;
        return *this;
    }

    constexpr friend TimeStep operator+(TimeStep lhs, const TimeStep& rhs)
    {
        return TimeStep(lhs.mTime + rhs.mTime);
    }

    constexpr friend TimeStep operator-(TimeStep lhs, const TimeStep& rhs)
    {
        return TimeStep(lhs.mTime - rhs.mTime);
    }

private:
    f32 mTime = 0.0f;
};
