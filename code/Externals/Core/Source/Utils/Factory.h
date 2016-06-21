#ifndef CORE_FACTORY_H
#define CORE_FACTORY_H

#include <string>
#include <unordered_map>
#include <memory>

template<class T>
class FactoryConstructableObject
{
public:
	typedef std::shared_ptr<T>(*ConstructFunction)();

						FactoryConstructableObject();
						FactoryConstructableObject(ConstructFunction constructFunction, unsigned int objectType, const std::string& objectClassName);
						~FactoryConstructableObject();

	ConstructFunction	GetConstructFunction() const;
	unsigned int		GetObjectType() const;
	const std::string&	GetObjectName() const;

private:

	ConstructFunction	m_ConstructFunction;
	unsigned int		m_ObjectType;
	std::string			m_ObjectClassName;
};

template<class T>
class FactoryConstructedObject
{
public:

						FactoryConstructedObject();
						FactoryConstructedObject(std::shared_ptr<T> constructedObject, unsigned int objectType, const std::string& objectClassName);
						~FactoryConstructedObject();

	bool				IsValid() const;

	std::shared_ptr<T>	GetConstructedObject() const;
	unsigned int		GetObjectType() const;
	const std::string&	GetObjectName() const;

private:
	std::shared_ptr<T>	m_ConstructedObject;
	unsigned int	m_ObjectType;
	std::string		m_ObjectClassName;
};

template<class T>
class Factory
{
public:
										Factory();
										~Factory();

	FactoryConstructedObject<T>			Construct(unsigned int objectType) const;

	FactoryConstructedObject<T>			Construct(const std::string& objectName) const;

	template<class ConstructableType>
	void								AddConstructable(const std::string& objectClassName);

private:
	typedef std::unordered_map<unsigned int, FactoryConstructableObject<T>>	ConstructableByTypeMap;
	typedef std::unordered_map<std::string, FactoryConstructableObject<T>>	ConstructableByStringMap;

	static unsigned int			s_TypeIndex;

	ConstructableByTypeMap		m_ConstructableByTypeMap;
	ConstructableByStringMap	m_ConstructableByStringMap;
};

#define DECLARE_FACTORY_INTERFACE(FactoryType) \
		friend class Factory<FactoryType>; \
	public: \
		virtual bool IsTypeOf(unsigned int type) const; \
		virtual unsigned int GetTypeVirtual(); \
		static unsigned int GetType(); \
	protected: \
		std::weak_ptr<FactoryType> m_This; \
	private: \
		static unsigned int s_Type;

#define IMPLEMENT_FACTORY_INTERFACE(FactoryType) \
	unsigned int FactoryType::s_Type = 0; \
	unsigned int FactoryType::GetTypeVirtual() { return s_Type; } \
	unsigned int FactoryType::GetType() { return s_Type; } \
	bool		 FactoryType::IsTypeOf(unsigned int type) const { return (s_Type == type); }

#define DECLARE_FACTORY_CONSTRUCTABLE(ConstructableClass, BaseClass, FactoryType) \
		friend class Factory<FactoryType>; \
	public: \
		typedef BaseClass Super; \
		virtual bool IsTypeOf(unsigned int type) const override; \
		virtual unsigned int GetTypeVirtual(); \
		static unsigned int GetType(); \
	private: \
		static std::shared_ptr<FactoryType> Construct(); \
		static unsigned int s_Type;

#define IMPLEMENT_FACTORY_CONSTRUCTABLE(ConstructableClass, FactoryType) \
	unsigned int					ConstructableClass::s_Type = 0; \
	std::shared_ptr<FactoryType>	ConstructableClass::Construct()		{ return std::make_shared<ConstructableClass>(); } \
	unsigned int					ConstructableClass::GetTypeVirtual() { return s_Type; } \
	unsigned int					ConstructableClass::GetType()		{ return s_Type; } \
	bool							ConstructableClass::IsTypeOf(unsigned int type) const { bool isTypeOf = Super::IsTypeOf(type); return isTypeOf || (s_Type == type); }

#define REGISTER_FACTORY_CONSTRUCTABLE(ConstructableClass, factoryVariable) \
	factoryVariable.AddConstructable<ConstructableClass>(#ConstructableClass);

#include "Factory.hpp"

#endif // CORE_MODULE_FACTORY_H