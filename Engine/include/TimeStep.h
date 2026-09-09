#pragma once

class TimeStep {
public:
	constexpr TimeStep(const float time = 0.0f) :
		mTime(time) {}
	constexpr TimeStep(const TimeStep& ts) :
		mTime(ts.mTime) {}

	constexpr operator float() const { return mTime; }
	constexpr float GetSeconds() const { return mTime; }
	constexpr float GetMilliseconds() const { return mTime * 1000.0f; }

	constexpr TimeStep& operator=(const TimeStep& ts) {
		if (this == &ts)
			return *this;

		mTime = ts.mTime;
		return *this;
	}

	constexpr TimeStep& operator+=(const TimeStep& rhs) {
		mTime += rhs.mTime;
		return *this;
	}

	constexpr friend TimeStep operator+(TimeStep lhs, const TimeStep& rhs) {
		return TimeStep(lhs.mTime + rhs.mTime);
	}

	constexpr friend TimeStep operator-(TimeStep lhs, const TimeStep& rhs) {
		return TimeStep(lhs.mTime - rhs.mTime);
	}

private:
	float mTime = 0.0f;
};
