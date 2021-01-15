#pragma once
#include "ManagedObjectBase.h"

using namespace System;

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

    public ref class VideoInfo : public ManagedObjectBase<DShow::VideoInfo>
    {
    public:
        VideoInfo(const DShow::VideoInfo& other);

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

    public ref class AudioInfo : public ManagedObjectBase<DShow::AudioInfo>
    {
    public:
        AudioInfo(const DShow::AudioInfo& other);

        property int MinChannels
        {
            int get();
            void set(int value);
        }
        property int MaxChannels
        {
            int get();
            void set(int value);
        }
        property int ChannelsGranularity
        {
            int get();
            void set(int value);
        }
        property int MinSampleRate
        {
            int get();
            void set(int value);
        }
        property int MaxSampleRate
        {
            int get();
            void set(int value);
        }
        property int SampleRateGranularity
        {
            int get();
            void set(int value);
        }
        property AudioFormat Format
        {
            AudioFormat get();
            void set(AudioFormat value);
        }
    };

    public interface class IDeviceId
    {
    public:
        /// <summary>
        /// Name of the device. Getter will return a COPY.
        /// </summary>
        virtual property System::String^ Name;
        /// <summary>
        /// Path of the device. Getter will return a COPY.
        /// </summary>
        virtual property System::String^ Path;
    };

    public ref class DeviceId : public IDeviceId, public ManagedObjectBase<DShow::DeviceId>
    {
    public:
        property System::String^ Name
        {
            virtual System::String^ get();
            virtual void set(System::String^ value);
        }
        property System::String^ Path
        {
            virtual System::String^ get();
            virtual void set(System::String^ value);
        }
    };

    public ref class VideoDevice : public IDeviceId, ManagedObjectBase<DShow::VideoDevice>
    {
    public:
        property System::String^ Name
        {
            virtual System::String^ get();
            virtual void set(System::String^ value);
        }
        property System::String^ Path
        {
            virtual System::String^ get();
            virtual void set(System::String^ value);
        }
        property bool AudioAttached
        {
            bool get();
            void set(bool value);
        }
        property bool SeparateAudioFilter
        {
            bool get();
            void set(bool value);
        }
        /// <summary>
        /// A list of all capabilities. Note that the getter here will return a COPY.
        /// </summary>
        property System::Collections::Generic::List<VideoInfo^>^ Capabilities
        {
            System::Collections::Generic::List<VideoInfo^>^ get();
            void set(System::Collections::Generic::List<VideoInfo^>^ value);
        }
    };

    public ref struct AudioDevice : public IDeviceId, ManagedObjectBase<DShow::AudioDevice>
    {
    public:
        property System::String^ Name
        {
            virtual System::String^ get();
            virtual void set(System::String^ value);
        }
        property System::String^ Path
        {
            virtual System::String^ get();
            virtual void set(System::String^ value);
        }
        /// <summary>
        /// A list of all capabilities. Note that the getter here will return a COPY.
        /// </summary>
        property System::Collections::Generic::List<AudioInfo^>^ Capabilities
        {
            System::Collections::Generic::List<AudioInfo^>^ get();
            void set(System::Collections::Generic::List<AudioInfo^>^ value);
        }
    };

    public interface class IConfig : public IDeviceId
    {
    public:
        virtual property bool UseDefaultConfig;
    };

    public ref class Config : public IConfig, public ManagedObjectBase<DShow::Config>
    {
    public:
        property System::String^ Name
        {
            virtual System::String^ get();
            virtual void set(System::String^ value);
        }
        property System::String^ Path
        {
            virtual System::String^ get();
            virtual void set(System::String^ value);
        }
        /// <summary>
        /// Use the device's desired default config
        /// </summary>
        virtual property bool UseDefaultConfig
        {
            virtual bool get();
            virtual void set(bool value);
        }
    };

    ref class VideoConfig;

    public ref class VideoCapturedEventArgs : public System::EventArgs
    {
    public:
        property VideoConfig^ Config;
        property array<Byte>^ Data;
        property long long StartTime;
        property long long StopTime;
        property long Rotation;

        VideoCapturedEventArgs(VideoConfig^ config, array<Byte>^ data, long long start, long long stop, long rotation);
    };

    typedef void (*TypePointerNativeVideoProc)(const DShow::VideoConfig&, unsigned char*, size_t, long long, long long, long);

    public ref class VideoConfig : public IConfig, public ManagedObjectBase<DShow::VideoConfig>
    {
    private:
        delegate void VideoProc(const DShow::VideoConfig& config, unsigned char* data,
            size_t size, long long startTime, long long stopTime,
            long rotation);
        void native_video_handler(const DShow::VideoConfig& config, unsigned char* data,
            size_t size, long long startTime, long long stopTime,
            long rotation);
    public:

        VideoConfig();

        delegate void VideoCapturedEventHandler(System::Object^ source, VideoCapturedEventArgs^ args);

        event VideoCapturedEventHandler^ OnVideoCaptured;

        property System::String^ Name
        {
            virtual System::String^ get();
            virtual void set(System::String^ value);
        }
        property System::String^ Path
        {
            virtual System::String^ get();
            virtual void set(System::String^ value);
        }
        virtual property bool UseDefaultConfig
        {
            virtual bool get();
            virtual void set(bool value);
        }

        /// <summary>
        /// Desired width of video. 
        /// </summary>
        property int Cx
        {
            int get();
            void set(int value);
        }
        /// <summary>
        /// Desired height of video. 
        /// </summary>
        property int CyAbs
        {
            int get();
            void set(int value);
        }
        /// <summary>
        /// Whether or not cy was negative.
        /// </summary>
        property bool CyFlip
        {
            bool get();
            void set(bool value);
        }
        /// <summary>
        /// Desired frame interval (in 100-nanosecond units) 
        /// </summary>
        property long long FrameInterval
        {
            long long get();
            void set(long long value);
        }
        /// <summary>
        /// Internal video format.
        /// </summary>
        property VideoFormat InternalFormat
        {
            VideoFormat get();
            void set(VideoFormat value);
        }
        /// <summary>
        /// Desired video format.
        /// </summary>
        property VideoFormat Format
        {
            VideoFormat get();
            void set(VideoFormat value);
        }
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
