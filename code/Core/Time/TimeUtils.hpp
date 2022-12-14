#ifndef TIMEUTILS_HPP
#define TIMEUTILS_HPP

#include "Core/BasicTypes.hpp"
#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"
#include <chrono>


class TimeMark: public ObjectBase
{
    GENERATE_METADATA(TimeMark)

public:

	COPY(TimeMark)
	{
		DO_COPY(mDeltaTimeMillis)
		DO_COPY(mStartTime)
		DO_COPY(mLastTime)
	}

    void init();
    void start();
    void end();
    f32 getElapsedTimeMillis();
    f32 getElapsedTimeSeconds();
    f32 getDeltaTimeMillis();
    f32 getDeltaTimeSeconds();

private:
	f32 mDeltaTimeMillis = 0.0f;
	std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> mLastTime;
	bool mIsStarted = false;

public:
	GET(IsStarted)
};

class Time: public ObjectBase, public Singleton<Time>
{
	GENERATE_METADATA(Time)	

public:
	void init(){mInternalTimeMark.init();}
	void startFrame() { mInternalTimeMark.start(); }
	void endFrame() { mInternalTimeMark.end(); }
	f32 getElapsedTimeMillis() { return mInternalTimeMark.getElapsedTimeMillis(); }
	f32 getElapsedTimeSeconds() { return mInternalTimeMark.getElapsedTimeSeconds(); }
	f32 getDeltaTimeMillis() { return mInternalTimeMark.getDeltaTimeMillis(); }
	f32 getDeltaTimeSeconds() { return mInternalTimeMark.getDeltaTimeSeconds(); }

private:
	TimeMark mInternalTimeMark;
};


#endif