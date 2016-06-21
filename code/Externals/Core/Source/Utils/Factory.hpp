#ifndef CORE_FACTORY_HPP
#define CORE_FACTORY_HPP

#include <assert.h>

template<class T>
FactoryConstructableObject<T>::FactoryConstructableObject()
	: m_ConstructFunction(nullptr)
	, m_ObjectType(0)
	, m_ObjectClassName("")
{

}

template<class T>
FactoryConstructableObject<T>::FactoryConstructableObject(ConstructFunction constructFunction, unsigned int objectType, const std::string& objectClassName)
	: m_ConstructFunction(constructFunction)
	, m_ObjectType(objectType)
	, m_ObjectClassName(objectClassName)
{

}

template<class T>
FactoryConstructableObject<T>::~FactoryConstructableObject()
{

}

template<class T>
typename FactoryConstructableObject<T>::ConstructFunction FactoryConstructableObject<T>::GetConstructFunction() const
{
	return m_ConstructFunction;
}

template<class T>
unsigned int FactoryConstructableObject<T>::GetObjectType() const
{
	return m_ObjectType;
}

template<class T>
const std::string& FactoryConstructableObject<T>::GetObjectName() const
{
	return m_ObjectClassName;
}

template<class T>
FactoryConstructedObject<T>::FactoryConstructedObject()
	: m_ConstructedObject(nullptr)
	, m_ObjectType(0)
	, m_ObjectClassName("")
{

}

template<class T>
FactoryConstructedObject<T>::FactoryConstructedObject(std::shared_ptr<T> constructedObject, unsigned int objectType, const std::string& objectClassName)
	: m_ConstructedObject(constructedObject)
	, m_ObjectType(objectType)
	, m_ObjectClassName(objectClassName)
{
	if (m_ConstructedObject == nullptr || m_ObjectType == 0 || m_ObjectClassName.empty())
	{
		if (m_ConstructedObject != nullptr)
		{
			m_ConstructedObject.reset();
		}

		m_ObjectType = 0;

		m_ObjectClassName.clear();
	}
}

template<class T>
FactoryConstructedObject<T>::~FactoryConstructedObject()
{

}

template<class T>
bool FactoryConstructedObject<T>::IsValid() const
{
	return m_ConstructedObject != nullptr;
}

template<class T>
std::shared_ptr<T> FactoryConstructedObject<T>::GetConstructedObject() const
{
	return m_ConstructedObject;
}

template<class T>
unsigned int FactoryConstructedObject<T>::GetObjectType() const
{
	return m_ObjectType;
}

template<class T>
const std::string& FactoryConstructedObject<T>::GetObjectName() const
{
	return m_ObjectClassName;
}

template<class T>
unsigned int Factory<T>::s_TypeIndex = 0;

template<class T>
Factory<T>::Factory()
{

}

template<class T>
Factory<T>::~Factory()
{

}

template<class T>
FactoryConstructedObject<T> Factory<T>::Construct(unsigned int objectType) const
{
	ConstructableByTypeMap::const_iterator it = m_ConstructableByTypeMap.find(objectType);
	if (it != m_ConstructableByTypeMap.end())
	{
		const FactoryConstructableObject<T>& constructable = it->second;
		FactoryConstructableObject<T>::ConstructFunction constructFunction = constructable.GetConstructFunction();
		std::shared_ptr<T> newObject = (*constructFunction)();
		newObject->m_This = newObject;
		FactoryConstructedObject<T> newConstructedObject (newObject, constructable.GetObjectType(), constructable.GetObjectName());
		return newConstructedObject;
	}

	return FactoryConstructedObject<T>();
}

template<class T>
FactoryConstructedObject<T> Factory<T>::Construct(const std::string& objectName) const
{
	ConstructableByStringMap::const_iterator it = m_ConstructableByStringMap.find(objectName);
	if (it != m_ConstructableByStringMap.end())
	{
		const FactoryConstructableObject<T>& constructable = it->second;
		FactoryConstructableObject<T>::ConstructFunction constructFunction = constructable.GetConstructFunction();
		std::shared_ptr<T> newObject = (*constructFunction)();
		newObject->m_This = newObject;
		FactoryConstructedObject<T> newConstructedObject(newObject, constructable.GetObjectType(), constructable.GetObjectName());
		return newConstructedObject;
	}

	return FactoryConstructedObject<T>();
}

template<class T>
template<class ConstructableType>
void Factory<T>::AddConstructable(const std::string& objectClassName)
{
	++s_TypeIndex;
	ConstructableType::s_Type = s_TypeIndex;
	FactoryConstructableObject<T> newConstructable(&ConstructableType::Construct, s_TypeIndex, objectClassName);
	m_ConstructableByTypeMap[s_TypeIndex] = newConstructable;
	m_ConstructableByStringMap[objectClassName] = newConstructable;
}

#endif // CORE_FACTORY_HPP
