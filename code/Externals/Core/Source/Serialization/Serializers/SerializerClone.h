#ifndef CORE_SERIALIZER_CLONE_H
#define CORE_SERIALIZER_CLONE_H

#include "SerializerBase.h"

class SerializerClone : public SerializerBase
{
public:
	SerializerClone(std::weak_ptr<const ISerializable> clonedObject);
	virtual ~SerializerClone() override;

	virtual SerializerType GetType() const override;

	std::weak_ptr<const ISerializable> GetCurrentlyClonedObject() const;

	template <class T>
	bool DeserializeVariable(const T& serializableToCopy, T& serializable);

	template <typename T, size_t N>
	bool DeserializeVariableArray(const T(&arrayToCopy)[N], T(&array)[N]);

	template <typename T>
	bool DeserializeVariable(const std::vector<T>& vectorToCopy, std::vector<T>& vector);

	template <class T>
	bool DeserializeVariable(std::shared_ptr<T> sharedPointerToCopy, std::shared_ptr<T>& sharedPointer);

	template <class T>
	bool DeserializeVariable(std::weak_ptr<T> weakPointerToCopy, std::weak_ptr<T>& weakPointer);

private:

	std::weak_ptr<const ISerializable> m_CurrentlyClonedObject;
};

#include "SerializerClone.hpp"

#endif // CORE_SERIALIZER_CLONE_H
