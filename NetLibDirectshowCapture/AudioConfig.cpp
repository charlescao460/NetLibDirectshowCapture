#include "pch.h"
#include "NetLibDirectshowCapture.h"

namespace NetLibDirectshowCapture
{
    NetLibDirectshowCapture::AudioCapturedEventArgs::AudioCapturedEventArgs(AudioConfig^ config, array<Byte>^ data, long long start, long long stop)
    {
        Config = config;
        Data = data;
        StartTime = start;
        StopTime = stop;
    }

    NetLibDirectshowCapture::AudioConfig::AudioConfig()
    {
        _audioProc = gcnew AudioProc(this, &NetLibDirectshowCapture::AudioConfig::native_audio_handler);
        IntPtr managedPointer = Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(_audioProc);
        TypePointerNativeAudioProc nativePointer = static_cast<TypePointerNativeAudioProc>(managedPointer.ToPointer());
        _native->callback = nativePointer;
    }

    void NetLibDirectshowCapture::AudioConfig::native_audio_handler(const DShow::AudioConfig& config, unsigned char* data, size_t size,
        long long startTime, long long stopTime)
    {
        array<Byte>^ copy = gcnew array<Byte>(static_cast<int>(size));
        System::Runtime::InteropServices::Marshal::Copy(IntPtr(data), copy, 0, static_cast<int>(size));
        AudioCapturedEventArgs^ args = gcnew AudioCapturedEventArgs(this, copy, startTime, stopTime);
        OnAudioCaptured(this, args);
    }

    System::String^ AudioConfig::Name::get()
    {
        return gcnew System::String(_native->name.c_str());
    }

    void AudioConfig::Name::set(System::String^ value)
    {
        _native->name = msclr::interop::marshal_as<std::wstring>(value);
    }

    System::String^ AudioConfig::Path::get()
    {
        return gcnew System::String(_native->path.c_str());
    }

    void AudioConfig::Path::set(System::String^ value)
    {
        _native->path = msclr::interop::marshal_as<std::wstring>(value);
    }

    bool AudioConfig::UseDefaultConfig::get()
    {
        return _native->useDefaultConfig;
    }

    void AudioConfig::UseDefaultConfig::set(bool value)
    {
        _native->useDefaultConfig = value;
    }

    bool AudioConfig::UseVideoDevice::get()
    {
        return _native->useVideoDevice;
    }

    void AudioConfig::UseVideoDevice::set(bool value)
    {
        _native->useVideoDevice = value;
    }

    bool AudioConfig::UseSeparateAudioFilter::get()
    {
        return _native->useSeparateAudioFilter;
    }

    void AudioConfig::UseSeparateAudioFilter::set(bool value)
    {
        _native->useSeparateAudioFilter = value;
    }

    int AudioConfig::SampleRate::get()
    {
        return _native->sampleRate;
    }

    void AudioConfig::SampleRate::set(int value)
    {
        _native->sampleRate = value;
    }

    int AudioConfig::Channels::get()
    {
        return _native->channels;
    }

    void AudioConfig::Channels::set(int value)
    {
        _native->channels = value;
    }

    AudioFormat AudioConfig::Format::get()
    {
        return static_cast<AudioFormat>(_native->format);
    }

    void AudioConfig::Format::set(AudioFormat value)
    {
        _native->format = static_cast<DShow::AudioFormat>(value);
    }

    AudioMode AudioConfig::Mode::get()
    {
        return static_cast<AudioMode>(_native->mode);
    }

    void AudioConfig::Mode::set(AudioMode value)
    {
        _native->mode = static_cast<DShow::AudioMode>(value);
    }
}
