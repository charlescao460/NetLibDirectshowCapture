#include "pch.h"
#include "NetLibDirectshowCapture.h"

namespace NetLibDirectshowCapture
{
    System::String^ Config::Name::get()
    {
        return gcnew System::String(_native->name.c_str());
    }

    void Config::Name::set(System::String^ value)
    {
        _native->name = msclr::interop::marshal_as<std::wstring>(value);
    }

    System::String^ Config::Path::get()
    {
        return gcnew System::String(_native->path.c_str());
    }

    void Config::Path::set(System::String^ value)
    {
        _native->path = msclr::interop::marshal_as<std::wstring>(value);
    }

    bool Config::UseDefaultConfig::get()
    {
        return _native->useDefaultConfig;
    }

    void Config::UseDefaultConfig::set(bool value)
    {
        _native->useDefaultConfig = value;
    }
}
