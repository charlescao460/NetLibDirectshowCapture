#include "pch.h"
#include "NetLibDirectshowCapture.h"

namespace NetLibDirectshowCapture
{
    int AudioInfo::MinChannels::get()
    {
        return _native->minChannels;
    }

    void AudioInfo::MinChannels::set(int value)
    {
        _native->minChannels = value;
    }

    int AudioInfo::MaxChannels::get()
    {
        return _native->maxChannels;
    }

    void AudioInfo::MaxChannels::set(int value)
    {
        _native->maxChannels = value;
    }

    int AudioInfo::ChannelsGranularity::get()
    {
        return _native->channelsGranularity;
    }

    void AudioInfo::ChannelsGranularity::set(int value)
    {
        _native->channelsGranularity = value;
    }

    int AudioInfo::MinSampleRate::get()
    {
        return _native->minSampleRate;
    }

    void AudioInfo::MinSampleRate::set(int value)
    {
        _native->minSampleRate = value;
    }

    int AudioInfo::MaxSampleRate::get()
    {
        return _native->maxSampleRate;
    }

    void AudioInfo::MaxSampleRate::set(int value)
    {
        _native->maxSampleRate = value;
    }

    int AudioInfo::SampleRateGranularity::get()
    {
        return _native->sampleRateGranularity;
    }

    void AudioInfo::SampleRateGranularity::set(int value)
    {
        _native->sampleRateGranularity = value;
    }

    AudioFormat AudioInfo::Format::get()
    {
        return static_cast<AudioFormat>(_native->format);
    }

    void AudioInfo::Format::set(AudioFormat value)
    {
        _native->format = static_cast<DShow::AudioFormat>(value);
    }
}
