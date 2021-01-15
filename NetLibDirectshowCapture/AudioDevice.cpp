#include "pch.h"
#include "NetLibDirectshowCapture.h"

namespace NetLibDirectshowCapture
{
    AudioInfo::AudioInfo(const DShow::AudioInfo& other) : ManagedObjectBase<DShow::AudioInfo>(other)
    {
    }

    System::String^ AudioDevice::Name::get()
    {
        return gcnew System::String(_native->name.c_str());
    }

    void AudioDevice::Name::set(System::String^ value)
    {

        _native->name = msclr::interop::marshal_as<std::wstring>(value);
    }

    System::String^ AudioDevice::Path::get()
    {
        return gcnew System::String(_native->path.c_str());
    }

    void AudioDevice::Path::set(System::String^ value)
    {
        _native->path = msclr::interop::marshal_as<std::wstring>(value);
    }

    System::Collections::Generic::List<AudioInfo^>^ AudioDevice::Capabilities::get()
    {
        System::Collections::Generic::List<AudioInfo^>^ ret
            = gcnew System::Collections::Generic::List<AudioInfo^>(static_cast<int>(_native->caps.size()));
        for (auto& cap : _native->caps)
        {
            ret->Add(gcnew AudioInfo(cap));
        }
        return ret;
    }

    void AudioDevice::Capabilities::set(System::Collections::Generic::List<AudioInfo^>^ value)
    {
        _native->caps.clear();
        _native->caps.resize(value->Count);
        for each (AudioInfo ^ cap in value)
        {
            _native->caps.push_back(*cap->GetInstance()); // Copied immediately after dereference
        }
    }
}
