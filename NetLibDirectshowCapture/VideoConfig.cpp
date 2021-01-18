#include "pch.h"
#include "NetLibDirectshowCapture.h"

namespace NetLibDirectshowCapture
{
    VideoCapturedEventArgs::VideoCapturedEventArgs(VideoConfig^ config, array<Byte>^ data, long long start,
        long long stop, long rotation)
    {
        Config = config;
        Data = data;
        StartTime = start;
        StopTime = stop;
        Rotation = rotation;
    }

    VideoConfig::VideoConfig()
    {
        _videoProc = gcnew VideoProc(this, &NetLibDirectshowCapture::VideoConfig::native_video_handler);
        IntPtr managedPointer = Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(_videoProc);
        TypePointerNativeVideoProc nativePointer = static_cast<TypePointerNativeVideoProc>(managedPointer.ToPointer());
        _native->callback = nativePointer;
    }

    void NetLibDirectshowCapture::VideoConfig::native_video_handler(const DShow::VideoConfig& config,
        unsigned char* data, size_t size, long long startTime, long long stopTime, long rotation)
    {
        array<Byte>^ copy = gcnew array<Byte>(static_cast<int>(size));
        System::Runtime::InteropServices::Marshal::Copy(IntPtr(data), copy, 0, static_cast<int>(size));
        VideoCapturedEventArgs^ args = gcnew VideoCapturedEventArgs(this, copy, startTime, stopTime, rotation);
        OnVideoCaptured(this, args);
    }

    System::String^ VideoConfig::Name::get()
    {
        return gcnew System::String(_native->name.c_str());
    }

    void VideoConfig::Name::set(System::String^ value)
    {
        _native->name = msclr::interop::marshal_as<std::wstring>(value);
    }

    System::String^ VideoConfig::Path::get()
    {
        return gcnew System::String(_native->path.c_str());
    }

    void VideoConfig::Path::set(System::String^ value)
    {
        _native->path = msclr::interop::marshal_as<std::wstring>(value);
    }

    bool VideoConfig::UseDefaultConfig::get()
    {
        return _native->useDefaultConfig;
    }

    void VideoConfig::UseDefaultConfig::set(bool value)
    {
        _native->useDefaultConfig = value;
    }

    int VideoConfig::Cx::get()
    {
        return _native->cx;
    }

    void VideoConfig::Cx::set(int value)
    {
        _native->cx = value;
    }

    int VideoConfig::CyAbs::get()
    {
        return _native->cy_abs;
    }

    void VideoConfig::CyAbs::set(int value)
    {
        _native->cy_abs = value;
    }

    bool VideoConfig::CyFlip::get()
    {
        return _native->cy_flip;
    }

    void VideoConfig::CyFlip::set(bool value)
    {
        _native->cy_flip = value;
    }

    long long VideoConfig::FrameInterval::get()
    {
        return _native->frameInterval;
    }

    void VideoConfig::FrameInterval::set(long long value)
    {
        _native->frameInterval = value;
    }

    VideoFormat VideoConfig::InternalFormat::get()
    {
        return static_cast<VideoFormat>(_native->internalFormat);
    }

    void VideoConfig::InternalFormat::set(VideoFormat value)
    {
        _native->internalFormat = static_cast<DShow::VideoFormat>(value);
    }

    VideoFormat VideoConfig::Format::get()
    {
        return static_cast<VideoFormat>(_native->format);
    }

    void VideoConfig::Format::set(VideoFormat value)
    {
        _native->format = static_cast<DShow::VideoFormat>(value);
    }
}
