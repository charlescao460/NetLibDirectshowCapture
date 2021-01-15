#include "pch.h"
#include "NetLibDirectshowCapture.h"

namespace NetLibDirectshowCapture
{
    System::String^ VideoDevice::Name::get()
    {
        return gcnew System::String(_native->name.c_str());
    }

    void VideoDevice::Name::set(System::String^ value)
    {

        _native->name = msclr::interop::marshal_as<std::wstring>(value);
    }

    System::String^ VideoDevice::Path::get()
    {
        return gcnew System::String(_native->path.c_str());
    }

    void VideoDevice::Path::set(System::String^ value)
    {
        _native->path = msclr::interop::marshal_as<std::wstring>(value);
    }

    bool VideoDevice::AudioAttached::get()
    {
        return _native->audioAttached;
    }

    void VideoDevice::AudioAttached::set(bool value)
    {
        _native->audioAttached = value;
    }

    bool VideoDevice::SeparateAudioFilter::get()
    {
        return _native->separateAudioFilter;
    }

    void VideoDevice::SeparateAudioFilter::set(bool value)
    {
        _native->separateAudioFilter = value;
    }

    System::Collections::Generic::List<VideoInfo^>^ VideoDevice::Capabilities::get()
    {
        System::Collections::Generic::List<VideoInfo^>^ ret
            = gcnew System::Collections::Generic::List<VideoInfo^>(static_cast<int>(_native->caps.size()));
        for (auto& cap : _native->caps)
        {
            ret->Add(gcnew VideoInfo(cap));
        }
        return ret;
    }

    void VideoDevice::Capabilities::set(System::Collections::Generic::List<VideoInfo^>^ value)
    {
        _native->caps.clear();
        _native->caps.resize(value->Count);
        for each (VideoInfo ^ cap in value)
        {
            _native->caps.push_back(*cap->GetInstance()); // Copied immediately after dereference
        }
    }
}