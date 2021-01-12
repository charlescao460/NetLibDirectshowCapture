#pragma once
#include "ManagedObjectBase.h"

// Port from dshowcapture.hpp
namespace NetLibDirectshowCapture
{
    enum class InitGraph
    {
        False,
        True,
    };

    enum class DialogType
    {
        ConfigVideo,
        ConfigAudio,
        ConfigCrossbar,
        ConfigCrossbar2,
    };

    public enum class VideoFormat
    {
        Any,
        Unknown,

        /* raw formats */
        ARGB = 100,
        XRGB,

        /* planar YUV formats */
        I420 = 200,
        NV12,
        YV12,
        Y800,

        /* packed YUV formats */
        YVYU = 300,
        YUY2,
        UYVY,
        HDYC,

        /* encoded formats */
        MJPEG = 400,
        H264,
    };

    public enum class AudioFormat
    {
        Any,
        Unknown,

        /* raw formats */
        Wave16bit = 100,
        WaveFloat,

        /* encoded formats */
        AAC = 200,
        AC3,
        MPGA, /* MPEG 1 */
    };

    enum class AudioMode
    {
        Capture,
        DirectSound,
        WaveOut,
    };

    enum class Result
    {
        Success,
        InUse,
        Error,
    };

    public ref class VideoInfo : ManagedObjectBase<DShow::VideoInfo>
    {
    public:
        property int MinCx
        {
            int get();
            void set(int value);
        }
        property int MinCy
        {
            int get();
            void set(int value);
        }
        property int MaxCx
        {
            int get();
            void set(int value);
        }
        property int MaxCy
        {
            int get();
            void set(int value);
        }
        property int GranularityCx
        {
            int get();
            void set(int value);
        }
        property int GranularityCy
        {
            int get();
            void set(int value);
        }
        property long long MinInterval
        {
            long long get();
            void set(long long value);
        }
        property long long MaxInterval
        {
            long long get();
            void set(long long value);
        }
        property VideoFormat Format
        {
            VideoFormat get();
            void set(VideoFormat value);
        }
    };

    public ref struct AudioInfo
    {
        int MinChannels, MaxChannels;
        int ChannelsGranularity;
        int MinSampleRate, MaxSampleRate;
        int SampleRateGranularity;
        AudioFormat Format;
    };

    public ref struct DeviceId
    {
        System::String^ Name;
        System::String^ Path;
    };

    public ref struct VideoDevice : DeviceId
    {
        bool AudioAttached = false;
        bool SeparateAudioFilter = false;
        System::Collections::Generic::List<VideoInfo^> Caps;
    };

    public ref struct AudioDevice : DeviceId
    {
        System::Collections::Generic::List<AudioInfo^> Caps;
    };

    public ref struct Config : DeviceId
    {
        /** Use the device's desired default config */
        bool UseDefaultConfig = true;
    };

    public ref struct VideoConfig : Config
    {
        delegate void VideoProc(VideoConfig^ config, System::Array^ data, unsigned long long startTime, unsigned long long stopTime, long rotation);

        VideoProc^ callback;

        /** Desired width/height of video. */
        int Cx = 0, CyAbs = 0;

        /** Whether or not cy was negative. */
        bool CyFlip = false;

        /** Desired frame interval (in 100-nanosecond units) */
        long long FrameInterval = 0;

        /** Internal video format. */
        VideoFormat InternalFormat = VideoFormat::Any;

        /** Desired video format. */
        VideoFormat Format = VideoFormat::Any;
    };

    public ref struct AudioConfig : Config
    {
        delegate void AudioProc(AudioConfig^ config, System::Array^ data, unsigned long long startTime, unsigned long long stopTime);

        AudioProc^ Callback;

        /**
             * Use the audio attached to the video device
             *
             * (name/path memeber variables will be ignored)
             */
        bool UseVideoDevice = false;

        /** Use separate filter for audio */
        bool UseSeparateAudioFilter = false;

        /** Desired sample rate */
        int SampleRate = 0;

        /** Desired channels */
        int Channels = 0;

        /** Desired audio format */
        AudioFormat Format = AudioFormat::Any;

        /** Audio playback mode */
        AudioMode Mode = AudioMode::Capture;
    };
}
