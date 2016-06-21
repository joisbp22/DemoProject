#include "SerializerClone.h"

SerializerClone::SerializerClone(std::weak_ptr<const ISerializable> clonedObject)
	: m_CurrentlyClonedObject (clonedObject)
{
}

SerializerClone::~SerializerClone()
{

}

SerializerType SerializerClone::GetType() const
{
	return SerializerType::eClone;
}

std::weak_ptr<const ISerializable> SerializerClone::GetCurrentlyClonedObject() const
{
	return m_CurrentlyClonedObject;
}

