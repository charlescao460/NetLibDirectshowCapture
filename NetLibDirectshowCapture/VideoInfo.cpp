#include "pch.h"
#include "NetLibDirectshowCapture.h"

namespace NetLibDirectshowCapture
{
    VideoInfo::VideoInfo(const DShow::VideoInfo& other) : ManagedObjectBase<DShow::VideoInfo>(other)
    {
    }

    int VideoInfo::MinCx::get()
    {
        return _native->minCX;
    }

    void VideoInfo::MinCx::set(int value)
    {
        _native->minCX = value;
    }

    int VideoInfo::MinCy::get()
    {
        return _native->minCY;
    }

    void VideoInfo::MinCy::set(int value)
    {
        _native->minCY = value;
    }

    int VideoInfo::MaxCx::get()
    {
        return _native->maxCX;
    }

    void VideoInfo::MaxCx::set(int value)
    {
        _native->maxCX = value;
    }

    int VideoInfo::MaxCy::get()
    {
        return _native->maxCY;
    }

    void VideoInfo::MaxCy::set(int value)
    {
        _native->maxCY = value;
    }

    int VideoInfo::GranularityCx::get()
    {
        return _native->granularityCX;
    }

    void VideoInfo::GranularityCx::set(int value)
    {
        _native->granularityCX = value;
    }

    int VideoInfo::GranularityCy::get()
    {
        return _native->granularityCY;
    }

    void VideoInfo::GranularityCy::set(int value)
    {
        _native->granularityCY = value;
    }

    long long VideoInfo::MinInterval::get()
    {
        return _native->minInterval;
    }

    void VideoInfo::MinInterval::set(long long value)
    {
        _native->minInterval = value;
    }

    long long VideoInfo::MaxInterval::get()
    {
        return _native->maxInterval;
    }

    void VideoInfo::MaxInterval::set(long long value)
    {
        _native->maxInterval = value;
    }

    VideoFormat VideoInfo::Format::get()
    {
        return static_cast<VideoFormat>(_native->format);
    }

    void VideoInfo::Format::set(VideoFormat value)
    {
        _native->format = static_cast<DShow::VideoFormat>(value);
    }
}
