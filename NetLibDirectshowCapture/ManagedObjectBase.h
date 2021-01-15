#pragma once
using namespace System;
namespace NetLibDirectshowCapture {

    template<class T>
    public ref class ManagedObjectBase
    {
    private protected:
        T* _native;

        template<class ... Types>
        ManagedObjectBase(Types ... args)
            : _native(new T(args ...))
        {
        }

        ManagedObjectBase(const T& other) : _native(new T(other))
        {
        }

        virtual ~ManagedObjectBase()
        {
            this->!ManagedObjectBase();
        }

    internal:
        T* GetInstance()
        {
            return _native;
        }

    public:
        !ManagedObjectBase()
        {
            if (_native != nullptr)
            {
                delete _native;
            }
        }
    };
}