#pragma once

#include <memory>
#include <assert.h>
#include "Property.h"

namespace UniDx
{

template<class T>
class Singleton
{
public:
    static ReadOnlyProperty< T* > instance;

    static void create()
    {
		assert(instance_ == nullptr);
        instance_ = std::make_unique<T>();
    }

	static void Delete()
	{
		instance_ = nullptr;
	}

protected:
    Singleton() {}

    static std::unique_ptr<T> instance_;
};

template<class T>
inline std::unique_ptr<T> Singleton<T>::instance_ = nullptr;

template<class T>
inline ReadOnlyProperty< T* > Singleton<T>::instance([]() { return instance_.get(); });

}