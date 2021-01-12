#pragma once
using namespace System;
namespace NetLibDirectshowCapture {

    template<class T>
    public ref class ManagedObjectBase
    {
    protected:
        T* _native;

    public:
        template<class ... Types>
        ManagedObjectBase(Types ... args)
            : _native(new T(args ...))
        {
        }

        virtual ~ManagedObjectBase()
        {
            this->!ManagedObjectBase();
        }

        !ManagedObjectBase()
        {
            if (_native != nullptr)
            {
                delete _native;
            }
        }

        T* GetInstance()
        {
            return _native;
        }
    };
}