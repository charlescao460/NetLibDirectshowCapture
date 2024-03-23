#include "pch.h"
#include "NetLibDirectshowCapture.h"

namespace NetLibDirectshowCapture
{
    Device::Device(bool initGraph) :
        ManagedObjectBase<DShow::Device>(initGraph ? DShow::InitGraph::True : DShow::InitGraph::False),
        _isRunning(false)
    {
    }

    Device::Device() :Device::Device(false)
    {}

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
        if (!_native->SetVideoConfig(_videoConfiguration->GetInstance()))
        {
            throw gcnew System::InvalidOperationException("Cannot set videoConfig.");
        }
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
		if (value->BindedDevice != nullptr && value->BindedDevice != this)
		{
			throw gcnew System::InvalidOperationException("Config has been already binded to another device.");
		}
        _audioConfiguration = value;
		_audioConfiguration->BindedDevice = this;
        if (!_native->SetAudioConfig(_audioConfiguration->GetInstance()))
        {
            throw gcnew System::InvalidOperationException("Cannot set audioConfig.");
        }
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
        if (_audioConfiguration == nullptr && _videoConfiguration == nullptr)
        {
            throw gcnew System::InvalidOperationException("Cannot start without any configurations set.");
        }
        const DShow::Result result = _native->Start();
        switch (result)
        {
        case DShow::Result::Error:
            throw gcnew System::UnauthorizedAccessException("Error when opening capture device.");
        case DShow::Result::InUse:
            throw gcnew System::IO::IOException("Device is in use.");
        default:
            break;
        }
        _isRunning = true;
    }

    void Device::Stop()
    {
        _native->Stop();
        _isRunning = false;
    }

    DeviceId^ Device::VideoDeviceId::get()
    {
        DShow::DeviceId id;
        if (!_native->GetVideoDeviceId(id))
        {
            return nullptr;
        }
        return gcnew DeviceId(id);
    }

    DeviceId^ Device::AudioDeviceId::get()
    {
        DShow::DeviceId id;
        if (!_native->GetAudioDeviceId(id))
        {
            return nullptr;
        }
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

    void DSLogger::native_logger(DShow::LogType type, const wchar_t* msg, void* param)
    {
        if (_callBack == nullptr)
        {
            return;
        }
        _callBack(static_cast<LogType>(type), gcnew System::String(msg));
    }

    void DSLogger::CallBack::set(LogCallBackDelegate^ value)
    {
        _callBack = value;
        if (value == nullptr)
        {
            DShow::SetLogCallback(nullptr, nullptr);
        }
        else
        {
            _nativeCallBack = gcnew DSNativeLoggerCallBackDelegate(&DSLogger::native_logger);
            IntPtr managedPointer = Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(_nativeCallBack);
            DShow::LogCallback nativePointer = static_cast<DShow::LogCallback>(managedPointer.ToPointer());
            DShow::SetLogCallback(nativePointer, nullptr);
        }
    }

}
