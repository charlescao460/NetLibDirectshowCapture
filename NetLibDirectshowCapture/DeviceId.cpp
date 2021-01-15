#include "pch.h"
#include "NetLibDirectshowCapture.h"

namespace NetLibDirectshowCapture
{
    System::String^ DeviceId::Name::get()
    {
        return gcnew System::String(_native->name.c_str());
    }

    void DeviceId::Name::set(System::String^ value)
    {

        _native->name = msclr::interop::marshal_as<std::wstring>(value);
    }

    System::String^ DeviceId::Path::get()
    {
        return gcnew System::String(_native->path.c_str());
    }

    void DeviceId::Path::set(System::String^ value)
    {
        _native->path = msclr::interop::marshal_as<std::wstring>(value);
    }
}
