///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "IvyPerf.h"

#include <cstdio>
#include <cstring>


PerfData::PerfData()
{
    m_top = 0;
    memset(m_data, 0, 256);
#ifndef STUB_PERF
    // Perf counter frequency
    QueryPerformanceFrequency(&m_perfCounterFreq);
#endif // STUB_PERF
}

PerfData::~PerfData()
{

}

void PerfData::push()
{
#ifndef STUB_PERF
    LARGE_INTEGER tick;
    QueryPerformanceCounter(&tick);

    m_data[m_top++] = tick.QuadPart;

    // = __rdtsc();
#endif // STUB_PERF
}

DOUBLE PerfData::pop()
{
#ifndef STUB_PERF
    LARGE_INTEGER tick;
    QueryPerformanceCounter(&tick);

    UINT64 e = tick.QuadPart - m_data[--m_top];

    return e / (DOUBLE)m_perfCounterFreq.QuadPart;
#else 
    return 0.0;
#endif // STUB_PERF
}

FramerateTracker::FramerateTracker()
{
    m_framePeriod = 1.0;
    QueryPerformanceFrequency(&m_perfCounterFreq);

    Reset();
}

FramerateTracker::~FramerateTracker()
{

}

void FramerateTracker::Reset()
{
    m_averageFrameTime = 0.0;
    m_maxFrameTime = 0.0;
    m_minFrameTime = 0.0;

    m_lastFrameTime = 0.0;

    m_frameTimes.clear();
    m_trackedElapsedTime = 0.0;

    m_totalElapsedTime = 0.0;
}

void FramerateTracker::BeginFrame()
{
    if (m_frameStartTick == 0)
    {
        LARGE_INTEGER tick;
        QueryPerformanceCounter(&tick);
        m_frameStartTick = tick.QuadPart;
    }
}

void FramerateTracker::EndFrame()
{
    LARGE_INTEGER currentTick;
    QueryPerformanceCounter(&currentTick);

    m_lastFrameTime = (FLOAT)(currentTick.QuadPart - m_frameStartTick) / m_perfCounterFreq.QuadPart;
    m_frameStartTick = currentTick.QuadPart;

    AddFrameTime(m_lastFrameTime);
}

void FramerateTracker::AddFrameTime(DOUBLE frameTime)
{
    m_totalElapsedTime += frameTime;
    m_trackedElapsedTime += frameTime;

    m_frameTimes.push_back(frameTime);

    if (m_trackedElapsedTime >= m_framePeriod)
    {
        DOUBLE frameTimeSum = m_frameTimes[0];

        m_minFrameTime = m_frameTimes[0];
        m_maxFrameTime = m_frameTimes[0];

        // Start iterating with Frame=1 since stats were initialized with Frame=0
        for (UINT i = 1; i < m_frameTimes.size(); ++i)
        {
            frameTimeSum += m_frameTimes[i];

            if (m_frameTimes[i] < m_minFrameTime)
            {
                m_minFrameTime = m_frameTimes[i];
            }

            if (m_frameTimes[i] > m_maxFrameTime)
            {
                m_maxFrameTime = m_frameTimes[i];
            }
        }

        m_averageFrameTime = frameTimeSum / static_cast<DOUBLE>(m_frameTimes.size());

        printf("FPS = %d, Avg Frame Time=%f\n", m_frameTimes.size(), m_averageFrameTime);

        m_trackedElapsedTime = 0.0;
        m_frameTimes.clear();
    }
}

