#ifndef CORE_SERIALIZABLE_H
#define CORE_SERIALIZABLE_H

#include "ISerializable.h"
#include <memory>
#include <string>

#include "Utils/Factory.h"

class TiXmlElement;

class Serializable : public ISerializable
{
	friend class SerializerXML;
	friend class SerializerClone;

	DECLARE_FACTORY_INTERFACE(Serializable)
public:
	virtual ~Serializable();

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer);

	void ResolveLink(unsigned int targetInstanceID, std::weak_ptr<Serializable> targetInstance);

protected:

	Serializable();

private:

	std::unordered_map<unsigned int, std::weak_ptr<Serializable>*> m_Dependencies;

	unsigned int m_InstanceID;

#ifdef _DEBUG
	std::string m_Name;
#endif
};

#define DECLARE_SERIALIZABLE(ClassName, BaseClass) \
	DECLARE_FACTORY_CONSTRUCTABLE(ClassName, BaseClass, Serializable) \
	private:

#define IMPLEMENT_SERIALIZABLE(ClassName) \
	IMPLEMENT_FACTORY_CONSTRUCTABLE(ClassName, Serializable)

#endif // CORE_SERIALIZABLE_H
