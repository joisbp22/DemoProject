#include "SerializerBase.h"

SerializerBase::SerializerBase()
{
}

SerializerBase::~SerializerBase()
{

}

SerializerType SerializerBase::GetType() const
{
	return SerializerType::eInvalid;
}

void SerializerBase::SetThis(std::weak_ptr<SerializerBase> thisPointer)
{
	m_This = thisPointer;
}

std::shared_ptr<Serializable> SerializerBase::ConstructSerializableObject(const Factory<Serializable>& factory, const std::string& objectName)
{
	std::shared_ptr<Serializable> newObject = factory.Construct(objectName).GetConstructedObject();
	return newObject;
}

std::shared_ptr<Serializable> SerializerBase::ConstructSerializableObject(const Factory<Serializable>& factory, unsigned int objectType)
{
	std::shared_ptr<Serializable> newObject = factory.Construct(objectType).GetConstructedObject();
	return newObject;
}
