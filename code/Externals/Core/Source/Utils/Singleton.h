#ifndef CORE_SINGLETON_H
#define CORE_SINGLETON_H

#include <memory>

template<class T>
class Singleton
{
public:
	template<class SubType>
	static std::weak_ptr<SubType> GetInstance();

	template<class SubType>
	static std::shared_ptr<SubType> ConstructSingleton();

	static void DestroySingleton();

protected:
	Singleton();
	virtual ~Singleton();

private:
	static std::shared_ptr<T> s_Instance;
};

#include "Singleton.hpp"

#endif // CORE_SINGLETON_H
