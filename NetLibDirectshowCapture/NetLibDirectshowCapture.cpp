#include "pch.h"
#include "NetLibDirectshowCapture.h"

namespace NetLibDirectshowCapture
{
    Device::Device(bool initGraph) :
        ManagedObjectBase<DShow::Device>(initGraph ? DShow::InitGraph::True : DShow::InitGraph::False),
        _isRunning(false)
    {
    }

    bool Device::Valid()
    {
        return _native->Valid();
    }

    bool Device::ResetGraph()
    {
        return _native->ResetGraph();
    }

    void Device::ShutdownGraph()
    {
        _native->ShutdownGraph();
    }

    bool Device::Running::get()
    {
        return _isRunning;
    }

    VideoConfig^ Device::VideoConfiguration::get()
    {
        return _videoConfiguration;
    }

    void Device::VideoConfiguration::set(VideoConfig^ value)
    {
        if (_isRunning)
        {
            throw gcnew System::InvalidOperationException("Cannot change config when capture is running.");
        }
        _videoConfiguration = value;
    }

    AudioConfig^ Device::AudioConfiguration::get()
    {
        return _audioConfiguration;
    }

    void Device::AudioConfiguration::set(AudioConfig^ value)
    {
        if (_isRunning)
        {
            throw gcnew System::InvalidOperationException("Cannot change config when capture is running.");
        }
        _audioConfiguration = value;
    }

    bool Device::ConnectFilters()
    {
        return _native->ConnectFilters();
    }

    void Device::Start()
    {
        if (_isRunning)
        {
            return;
        }
        if (_audioConfiguration == nullptr || _videoConfiguration == nullptr)
        {
            throw gcnew System::InvalidOperationException("Cannot start without any configurations set.");
        }
        const DShow::Result result = _native->Start();
        switch (result)
        {
        case DShow::Result::Error:
            throw gcnew System::UnauthorizedAccessException("Error when opening capture device.");
        case DShow::Result::InUse:
            throw gcnew  System::IO::IOException("Device is in use.");
        default:
            break;
        }
    }

    void Device::Stop()
    {
        _native->Stop();
    }

    DeviceId^ Device::VideoDeviceId::get()
    {
        DShow::DeviceId id;
        _native->GetVideoDeviceId(id);
        return gcnew DeviceId(id);
    }

    DeviceId^ Device::AudioDeviceId::get()
    {
        DShow::DeviceId id;
        _native->GetAudioDeviceId(id);
        return gcnew DeviceId(id);
    }

    void Device::OpenDialog(IntPtr hwnd, DialogType type)
    {
        _native->OpenDialog(hwnd.ToPointer(), static_cast<DShow::DialogType>(type));
    }

    System::Collections::Generic::List<VideoDevice^>^ Device::EnumVideoDevices()
    {
        std::vector<DShow::VideoDevice> result;
        if (!DShow::Device::EnumVideoDevices(result))
        {
            throw gcnew System::UnauthorizedAccessException("Cannot enumerating devices, please check access privilege.");
        }
        System::Collections::Generic::List<VideoDevice^>^ ret
            = gcnew System::Collections::Generic::List<VideoDevice^>(static_cast<int>(result.size()));
        for (auto& device : result)
        {
            ret->Add(gcnew VideoDevice(device));
        }
        return ret;
    }

    System::Collections::Generic::List<AudioDevice^>^ Device::EnumAudioDevices()
    {
        std::vector<DShow::AudioDevice> result;
        if (!DShow::Device::EnumAudioDevices(result))
        {
            throw gcnew System::UnauthorizedAccessException("Cannot enumerating devices, please check access privilege.");
        }
        System::Collections::Generic::List<AudioDevice^>^ ret
            = gcnew System::Collections::Generic::List<AudioDevice^>(static_cast<int>(result.size()));
        for (auto& device : result)
        {
            ret->Add(gcnew AudioDevice(device));
        }
        return ret;
    }
}
