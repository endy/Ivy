///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _IVYPERF_H_
#define _IVYPERF_H_

#include <iostream>
#include <string>
#include <vector>

#include "IvyTypes.h"

#ifndef _WIN32
#define STUB_PERF 1
#endif // _WIN32

///////////////////////////////////////////////////////////////////////////////////////////////////
/// PerfData Class
///////////////////////////////////////////////////////////////////////////////////////////////////
class PerfData
{
public:
    PerfData();

    virtual ~PerfData();

    void push();
    double pop();

private:
    UINT64 m_data[256];
    UINT m_top; 

    // Perf counter frequency
#ifdef _WIN32
    LARGE_INTEGER m_perfCounterFreq;
#endif

};

class FramerateTracker
{
public:
    FramerateTracker();
    virtual ~FramerateTracker();

    void Reset();

    void BeginFrame();
    void EndFrame();

    DOUBLE AvgFrameTime() const { return m_averageFrameTime; }
    DOUBLE MaxFrameTime() const { return m_maxFrameTime; }
    DOUBLE MinFrameTime() const { return m_minFrameTime; }
    FLOAT  LastFrameTime() const { return m_lastFrameTime; }

private:

    void AddFrameTime(DOUBLE frameTime);

    DOUBLE m_averageFrameTime;
    DOUBLE m_maxFrameTime;
    DOUBLE m_minFrameTime;
    FLOAT  m_lastFrameTime;

    std::vector<DOUBLE> m_frameTimes;
    DOUBLE m_trackedElapsedTime;
    DOUBLE m_totalElapsedTime;

    DOUBLE m_framePeriod;

    UINT64 m_frameStartTick;
    LARGE_INTEGER m_perfCounterFreq;
};

inline void WriteOut(std::string out)
{
    std::cout << out << std::endl;
}

#endif // _IVYPERF_H_

