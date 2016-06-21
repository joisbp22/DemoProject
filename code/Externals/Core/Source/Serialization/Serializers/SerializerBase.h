#ifndef CORE_SERIALIZER_BASE_H
#define CORE_SERIALIZER_BASE_H

#include "Serialization/Serializable.h"

enum class SerializerType
{
	eInvalid = 0,
	eXML,
	eClone
};

class SerializerBase
{
public:
	virtual ~SerializerBase();

	virtual SerializerType GetType() const;

	void SetThis(std::weak_ptr<SerializerBase> thisPointer);

	std::shared_ptr<Serializable> ConstructSerializableObject(const Factory<Serializable>& factory, const std::string& objectName);

	std::shared_ptr<Serializable> ConstructSerializableObject(const Factory<Serializable>& factory, unsigned int objectType);

protected:
	SerializerBase();

	std::weak_ptr<SerializerBase> m_This;

private:

	std::weak_ptr<Serializable> m_CurrentlyDeserializedObject;
};

#endif // CORE_SERIALIZER_BASE_H
