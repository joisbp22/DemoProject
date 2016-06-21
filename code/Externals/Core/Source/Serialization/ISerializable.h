#ifndef CORE_ISERIALIZABLE_H
#define CORE_ISERIALIZABLE_H

#include <memory>

class SerializerBase;

class ISerializable
{
protected:
	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) = 0;

	ISerializable();
	virtual ~ISerializable();
};

#endif // CORE_SERIALIZABLE_H
