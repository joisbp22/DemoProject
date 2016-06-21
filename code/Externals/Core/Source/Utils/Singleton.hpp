#ifndef CORE_SINGLETON_HPP
#define CORE_SINGLETON_HPP

#include <assert.h>

template<class T>
std::shared_ptr<T> Singleton<T>::s_Instance = nullptr;

template<class T>
template<class SubType>
std::weak_ptr<SubType> Singleton<T>::GetInstance()
{
	return std::static_pointer_cast<SubType>(s_Instance);
}

template<class T>
template<class SubType>
std::shared_ptr<SubType> Singleton<T>::ConstructSingleton()
{
	assert(s_Instance == nullptr);
	if (s_Instance != nullptr)
	{
		s_Instance.reset();
	}

	s_Instance = std::make_shared<SubType>();

	return std::static_pointer_cast<SubType>(s_Instance);
}

template<class T>
void Singleton<T>::DestroySingleton()
{
	assert(s_Instance != nullptr);
	if (s_Instance != nullptr)
	{
		s_Instance.reset();
	}
}

template<class T>
Singleton<T>::Singleton()
{
	assert(s_Instance == nullptr);
}

template<class T>
Singleton<T>::~Singleton()
{
	assert(s_Instance == nullptr);
}

#endif // CORE_SINGLETON_HPP
