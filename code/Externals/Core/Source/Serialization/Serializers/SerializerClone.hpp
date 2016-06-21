#ifndef CORE_SERIALIZER_CLONE_HPP
#define CORE_SERIALIZER_CLONE_HPP

#include <assert.h>

#include "Serialization/SerializationModule.h"

template <class T>
bool SerializerClone::DeserializeVariable(const T& serializableToCopy, T& serializable)
{
	bool success = true;

	serializable = serializableToCopy;

	return success;
}

template <typename T, size_t N>
bool SerializerClone::DeserializeVariableArray(const T(&arrayToCopy)[N], T(&array)[N])
{
	bool success = true;

	for (unsigned int i = 0; i < N; ++i)
	{
		T newValue;

		bool elementDeserializationSuccess = DeserializeVariable(arrayToCopy[i], newValue);
		assert(elementDeserializationSuccess == true);
		if (elementDeserializationSuccess == true)
			array[i] = newValue;

		success &= elementDeserializationSuccess;
	}

	return success;
}

template <typename T>
bool SerializerClone::DeserializeVariable(const std::vector<T>& vectorToCopy, std::vector<T>& vector)
{
	bool success = true;

	for (const auto it : vectorToCopy)
	{
		T newValue;

		bool elementDeserializationSuccess = DeserializeVariable(it, newValue);
		assert(elementDeserializationSuccess == true);
		if (elementDeserializationSuccess == true)
			vector.push_back(newValue);

		success &= elementDeserializationSuccess;
	}

	return success;
}

template <class T>
bool SerializerClone::DeserializeVariable(std::shared_ptr<T> sharedPointerToCopy, std::shared_ptr<T>& sharedPointer)
{
	bool success = true;

	std::shared_ptr<SerializationModule> serializationModule = GET_MODULE(SerializationModule).lock();
	if (serializationModule != nullptr)
	{
		std::shared_ptr<Serializable> newValue = ConstructSerializableObject(serializationModule->GetSerializableFactory(), sharedPointerToCopy->GetTypeVirtual());

#ifdef _DEBUG
		newValue->m_Name = sharedPointerToCopy->m_Name;
#endif

		newValue->m_InstanceID = 0xffff;

		std::weak_ptr<const ISerializable> cachedObject = m_CurrentlyClonedObject;
		m_CurrentlyClonedObject = sharedPointerToCopy;

		success = newValue->Deserialize(m_This.lock());
		assert(success == true);
		if (success == true)
		{
			sharedPointer = std::dynamic_pointer_cast<T>(newValue);
		}

		m_CurrentlyClonedObject = cachedObject;
	}

	return success;
}

template <class T>
bool SerializerClone::DeserializeVariable(std::weak_ptr<T> weakPointerToCopy, std::weak_ptr<T>& weakPointer)
{
	bool success = true;

	// if we're not copying pointers to anything else than prefabs or things that weren't duplicated, we might be in trouble here
	weakPointer = weakPointerToCopy;

	return success;
}

#endif // CORE_SERIALIZER_CLONE_HPP
