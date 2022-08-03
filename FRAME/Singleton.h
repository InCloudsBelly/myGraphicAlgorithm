//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma once
#include <memory>

template <typename T>
class _Singleton
{
public:
	virtual ~_Singleton() = default;
	static std::shared_ptr<T> getOrCreateInstance()
	{
		if (!m_pInstance)
			m_pInstance.reset(new T());    //Can not use std::make_shared<T>(), because private constructor of T cant be accessed in memory(shared_ptr object) 
		return m_pInstance;
	}

protected:								   //protected, not private, because T's constuctor will call this _Singleton() constructor 
	_Singleton() = default;

private:
	static std::shared_ptr<T> m_pInstance;
};

template <typename T>
std::shared_ptr<T> _Singleton<T>::m_pInstance = nullptr;