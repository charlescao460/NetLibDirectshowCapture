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

    public enum class DialogType
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
        RGB24,

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

    public enum class AudioMode
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
        DeviceId();
        DeviceId(const DShow::DeviceId& other);

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
        VideoDevice();
        VideoDevice(const DShow::VideoDevice& other);

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

    public ref class AudioDevice : public IDeviceId, ManagedObjectBase<DShow::AudioDevice>
    {
    public:
        AudioDevice();
        AudioDevice(const DShow::AudioDevice& other);

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

    ref class Device;
    ref class VideoConfig;

    public ref class VideoCapturedEventArgs : public System::EventArgs
    {
    public:
        property VideoConfig^ Config;
        /// <summary>
        /// Pointer to native array.
        /// </summary>
        property IntPtr Ptr;
        /// <summary>
        /// Array of managed buffer. Note that this array is not the exact size of frame buffer.
        /// Do not reference this array after exiting event handler.
        /// </summary>
        property array<Byte>^ Array;
        property int Length;
        property long long StartTime;
        property long long StopTime;
        property long Rotation;

        VideoCapturedEventArgs(VideoConfig^ config, IntPtr ptr, array<Byte>^ arr, int arrSize, long long start, long long stop, long rotation);
    };

    typedef void(__stdcall* TypePointerNativeVideoProc)(const DShow::VideoConfig&, unsigned char*, size_t, long long, long long, long);

    public ref class VideoConfig : public IConfig, public ManagedObjectBase<DShow::VideoConfig>
    {
    private:
        delegate void VideoProc(const DShow::VideoConfig& config, unsigned char* data,
            size_t size, long long startTime, long long stopTime,
            long rotation);
        VideoProc^ _videoProc;
        void native_video_handler(const DShow::VideoConfig& config, unsigned char* data,
            size_t size, long long startTime, long long stopTime,
            long rotation);
    internal:
        Device^ BindedDevice;
    public:

        VideoConfig();

        delegate void VideoCapturedEventHandler(System::Object^ source, VideoCapturedEventArgs^ args);

        /// <summary>
        /// Raised when native has received video bytes. It will send a copy to managed code.
        /// This event must be assigned before calling Start().
        /// </summary>
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

    ref class AudioConfig;

    public ref class AudioCapturedEventArgs : public System::EventArgs
    {
    public:
        property AudioConfig^ Config;
        /// <summary>
        /// Pointer to native array.
        /// </summary>
        property IntPtr Ptr;
        /// <summary>
        /// Array of managed buffer. Note that this array is not the exact size of frame buffer.
        /// Do not reference this array after exiting event handler.
        /// </summary>
        property array<Byte>^ Array;
        property int Length;
        property long long StartTime;
        property long long StopTime;

        AudioCapturedEventArgs(AudioConfig^ config, IntPtr ptr, array<Byte>^ data, int arrSize, long long start, long long stop);
    };

    typedef void(__stdcall* TypePointerNativeAudioProc)(const DShow::AudioConfig&, unsigned char*, size_t, long long, long long);

    public ref class AudioConfig : public IConfig, public ManagedObjectBase<DShow::AudioConfig>
    {
    private:
        delegate void AudioProc(const DShow::AudioConfig& config, unsigned char* data,
            size_t size, long long startTime, long long stopTime);
        AudioProc^ _audioProc;

        void native_audio_handler(const DShow::AudioConfig& config, unsigned char* data,
            size_t size, long long startTime, long long stopTime);

    internal:
        Device^ BindedDevice;

    public:
        AudioConfig();

        /// <summary>
        /// Raised when native has received audio bytes. It will send a copy to managed code.
        /// This event must be assigned before calling Start().
        /// </summary>
        delegate void AudioCapturedEventHandler(System::Object^ source, AudioCapturedEventArgs^ args);

        event AudioCapturedEventHandler^ OnAudioCaptured;

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
        /// Use the audio attached to the video device
        /// (name/path memeber variables will be ignored)
        /// </summary>
        property bool UseVideoDevice
        {
            bool get();
            void set(bool value);
        }

        /// <summary>
        /// Use separate filter for audio 
        /// </summary>
        property bool UseSeparateAudioFilter
        {
            bool get();
            void set(bool value);
        }

        /// <summary>
        /// Desired sample rate 
        /// </summary>
        property int SampleRate
        {
            int get();
            void set(int value);
        }

        /// <summary>
        /// Desired channels 
        /// </summary>
        property int Channels
        {
            int get();
            void set(int value);
        }

        /// <summary>
        /// Desired audio format
        /// </summary>
        property AudioFormat Format
        {
            AudioFormat get();
            void set(AudioFormat value);
        }

        /// <summary>
        /// Audio playback mode
        /// </summary>
        property AudioMode Mode
        {
            AudioMode get();
            void set(AudioMode value);
        }
    };

    public ref class Device : public ManagedObjectBase<DShow::Device>
    {
    private:
        VideoConfig^ _videoConfiguration;
        AudioConfig^ _audioConfiguration;
        int _VideoManagedBufferSize;
        int _AudioManagedBufferSize;
    protected:
        bool _isRunning;
    internal:
        array<Byte>^ VideoManagedBuffer;
        array<Byte>^ AudioManagedBuffer;
    public:
        Device(bool initGraph, int videoBufferSize, int audioBufferSize);

        /// <summary>
        /// Default constructor with initGraph=false, and 100MiB video buffer, 10MiB audio buffer
        /// </summary>
        Device();

        bool Valid();

        bool ResetGraph();

        void ShutdownGraph();

        property bool Running
        {
            bool get();
        };

        /// <summary>
        /// Configuration of video capture devices. Note that its On-capture event must have a handler.
        /// And after set, it should not be modified without calling setter again.
        /// Setter will throw if trying to set it when device is running.
        /// </summary>
        /// <exception cref="System::InvalidOperationException">If set during running</exception>
        property VideoConfig^ VideoConfiguration
        {
            VideoConfig^ get();
            void set(VideoConfig^ value);
        }

        /// <summary>
        /// Configuration of video capture devices. Note that its On-capture event must have a handler.
        /// And after set, it should not be modified without calling setter again.
        /// Setter will throw if trying to set it when device is running.
        /// </summary>
        /// <exception cref="System::InvalidOperationException">If set during running</exception>
        property AudioConfig^ AudioConfiguration
        {
            AudioConfig^ get();
            void set(AudioConfig^ value);
        }

        /// <summary>
        /// Connects all the configured filters together.
        /// Call SetVideoConfig and/or SetAudioConfig before using.
        /// </summary>
        /// <returns>If succeed</returns>
        bool ConnectFilters();

        /// <summary>
        /// Start capturing. Before calling this, a valid video config and/or video config must be set.
        /// </summary>
        /// <exception cref="System::InvalidOperationException">If no valid video or audio config is set.</exception>
        /// <exception cref="System::UnauthorizedAccessException">If there is an error when opening the devices.</exception>
        /// <exception cref="System::IO::IOException">If the device is in use.</exception>
        void Start();

        /// <summary>
        /// Stop capturing.
        /// </summary>
        void Stop();

        property DeviceId^ VideoDeviceId
        {
            DeviceId^ get();
        }

        property DeviceId^ AudioDeviceId
        {
            DeviceId^ get();
        }

        /// <summary>
        /// Opens a DirectShow dialog associated with this device
        /// </summary>
        /// <param name="hwnd">Window handle</param>
        /// <param name="type">The dialog type</param>
        void OpenDialog(IntPtr hwnd, DialogType type);

        /// <summary>
        /// Enum all video devices on this computer. 
        /// </summary>
        /// <exception cref="System::UnauthorizedAccessException">When there is an error enumerating device.</exception>
        /// <returns>All video devices on this computer</returns>
        static System::Collections::Generic::List<VideoDevice^>^ EnumVideoDevices();

        /// <summary>
        /// Enum all audio devices on this computer. 
        /// </summary>
        /// <exception cref="System::UnauthorizedAccessException">When there is an error enumerating device.</exception>
        /// <returns>All audio devices on this computer</returns>
        static System::Collections::Generic::List<AudioDevice^>^ EnumAudioDevices();
    };

    public enum class LogType
    {
        Error,
        Warning,
        Info,
        Debug,
    };

    [System::Runtime::InteropServices::UnmanagedFunctionPointerAttribute(System::Runtime::InteropServices::CallingConvention::Cdecl)]
    delegate void DSNativeLoggerCallBackDelegate(DShow::LogType type, const wchar_t* msg, void* param);
    public delegate void LogCallBackDelegate(LogType type, System::String^ msg);

    /// <summary>
    /// Static CLR class for configuring logger
    /// </summary>
    public ref class DSLogger abstract sealed
    {
    private:
        static DSNativeLoggerCallBackDelegate^ _nativeCallBack;
        static void native_logger(DShow::LogType type, const wchar_t* msg, void* param);
        static LogCallBackDelegate^ _callBack;
    public:
        static property LogCallBackDelegate^ CallBack
        {
            void set(LogCallBackDelegate^ value);
        };
    };

    class ImageTranscoder
    {
    private:

        static int XrgbToBgr24(void* nativeSrc, int nativeLength, array<Byte>^ managedDst, int width, int height);

    public:
        /// <summary>
        /// Transcode native arrya to managed BGR24 array, return the number of bytes written to managed.
        /// Currently only XRGB.
        /// </summary>
        /// <param name="nativeSrc">Pointer to native array</param>
        /// <param name="nativeLength">Length of native array</param>
        /// <param name="managedDst">Managed array to output BGR24 data</param>
        /// <param name="width">Image width</param>
        /// <param name="height">Image height</param>
        /// <param name="format">Frame format</param>
        /// <returns>The number of bytes written to managed array</returns>
        /// <exception cref="System::NotImplementedException">When input format is not supported.</exception>
        static int TranscodeToBgr24(void* nativeSrc, int nativeLength, array<Byte>^ managedDst, int width, int height, VideoFormat format);
    };
}
