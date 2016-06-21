#ifndef CORE_SERIALIZER_XML_HPP
#define CORE_SERIALIZER_XML_HPP

#include "tinyxml.h"
#include <assert.h>

#include "Serialization/SerializationModule.h"

template <class T>
bool SerializerXML::DeserializeVariable(const std::string& variableName, T& serializable)
{
	bool success = true;

	assert(m_CurrentlyParsedXMLNode != nullptr);
	if (m_CurrentlyParsedXMLNode != nullptr)
	{
		const TiXmlElement* variableXMLNode = m_CurrentlyParsedXMLNode->FirstChildElement(variableName);
		if (variableXMLNode != nullptr)
		{
			const TiXmlElement* cachedXMLNode = m_CurrentlyParsedXMLNode;
			m_CurrentlyParsedXMLNode = variableXMLNode;

			success = serializable.Deserialize(m_This.lock());

			m_CurrentlyParsedXMLNode = cachedXMLNode;
		}
	}
	else
	{
		success = false;
	}

	return success;
}

template <typename T, size_t N>
bool SerializerXML::DeserializeVariableArray(const std::string& variableName, T(&array)[N])
{
	bool success = true;

	assert(m_CurrentlyParsedXMLNode != nullptr);
	if (m_CurrentlyParsedXMLNode != nullptr)
	{
		const TiXmlElement* variableXMLNode = m_CurrentlyParsedXMLNode->FirstChildElement(variableName);
		if (variableXMLNode != nullptr)
		{
			size_t index = 0;
			const TiXmlElement* elementXMLNode = variableXMLNode->FirstChildElement("ArrayElement");
			while (elementXMLNode != nullptr)
			{
				if (index >= N)
				{
					break;
				}

				T newValue;

				const TiXmlElement* cachedXMLNode = m_CurrentlyParsedXMLNode;
				m_CurrentlyParsedXMLNode = elementXMLNode;

				bool elementDeserializationSuccess = DeserializeVariable("ElementVariable", newValue);
				assert(elementDeserializationSuccess == true);
				if (elementDeserializationSuccess == true)
					array[index] = newValue;

				m_CurrentlyParsedXMLNode = cachedXMLNode;

				success &= elementDeserializationSuccess;

				++index;
				elementXMLNode = elementXMLNode->NextSiblingElement("ArrayElement");
			}
		}
	}
	else
	{
		success = false;
	}

	return success;
}

template <typename T>
bool SerializerXML::DeserializeVariable(const std::string& variableName, std::vector<T>& vector)
{
	bool success = true;

	assert(m_CurrentlyParsedXMLNode != nullptr);
	if (m_CurrentlyParsedXMLNode != nullptr)
	{
		const TiXmlElement* variableXMLNode = m_CurrentlyParsedXMLNode->FirstChildElement(variableName);
		if (variableXMLNode != nullptr)
		{
			const TiXmlElement* elementXMLNode = variableXMLNode->FirstChildElement("VectorElement");
			while (elementXMLNode != nullptr)
			{
				T newValue;

				const TiXmlElement* cachedXMLNode = m_CurrentlyParsedXMLNode;
				m_CurrentlyParsedXMLNode = elementXMLNode;

				bool elementDeserializationSuccess = DeserializeVariable("ElementVariable", newValue);
				assert(elementDeserializationSuccess == true);
				if (elementDeserializationSuccess == true)
					vector.push_back(newValue);

				m_CurrentlyParsedXMLNode = cachedXMLNode;

				success &= elementDeserializationSuccess;

				elementXMLNode = elementXMLNode->NextSiblingElement("VectorElement");
			}
		}
	}
	else
	{
		success = false;
	}

	return success;
}

template <class T>
bool SerializerXML::DeserializeVariable(const std::string& variableName, std::shared_ptr<T>& sharedPointer)
{
	bool success = true;

	assert(m_CurrentlyParsedXMLNode != nullptr);
	if (m_CurrentlyParsedXMLNode != nullptr)
	{
		const TiXmlElement* variableXMLNode = m_CurrentlyParsedXMLNode->FirstChildElement(variableName);
		if (variableXMLNode != nullptr)
		{
			success = false;

			std::string pointerType = variableXMLNode->Attribute("type");

			std::shared_ptr<SerializationModule> serializationModule = GET_MODULE(SerializationModule).lock();
			if (serializationModule != nullptr)
			{
				std::shared_ptr<Serializable> newValue = ConstructSerializableObject(serializationModule->GetSerializableFactory(), pointerType);

#ifdef _DEBUG
				const char* name = variableXMLNode->Attribute("name");
				if (name != nullptr)
					newValue->m_Name = name;
#endif

				unsigned int tempValue = 0;
				int result = variableXMLNode->QueryUnsignedAttribute("id", &tempValue);
				assert(result == TIXML_SUCCESS);
				if (result == TIXML_SUCCESS)
				{
					newValue->m_InstanceID = tempValue;
				}

				const TiXmlElement* cachedXMLNode = m_CurrentlyParsedXMLNode;
				m_CurrentlyParsedXMLNode = variableXMLNode;

				std::weak_ptr<Serializable> cachedObject = m_CurrentlyDeserializedObject;
				m_CurrentlyDeserializedObject = newValue;

				success = newValue->Deserialize(m_This.lock());
				assert(success == true);
				if (success == true)
				{
					std::shared_ptr<SerializationModule> serializationModule = GET_MODULE(SerializationModule).lock();
					if (serializationModule != nullptr)
					{
						serializationModule->OnSerializableLoaded(newValue->m_InstanceID, newValue);
					}
					sharedPointer = std::dynamic_pointer_cast<T>(newValue);
				}

				m_CurrentlyParsedXMLNode = cachedXMLNode;

				m_CurrentlyDeserializedObject = cachedObject;
			}
		}
	}
	else
	{
		success = false;
	}

	return success;
}

template <class T>
bool SerializerXML::DeserializeVariable(const std::string& variableName, std::weak_ptr<T>& weakPointer)
{
	bool success = true;

	assert(m_CurrentlyParsedXMLNode != nullptr);
	if (m_CurrentlyParsedXMLNode != nullptr)
	{
		const TiXmlElement* variableXMLNode = m_CurrentlyParsedXMLNode->FirstChildElement(variableName);
		if (variableXMLNode != nullptr)
		{
			success = false;

			unsigned int targetInstanceID = 0;
			int result = variableXMLNode->QueryUnsignedAttribute("targetInstanceID", &targetInstanceID);
			assert(result == TIXML_SUCCESS);
			if (result == TIXML_SUCCESS)
			{
				std::shared_ptr<SerializationModule> serializationModule = GET_MODULE(SerializationModule).lock();
				if (serializationModule != nullptr)
				{
					std::shared_ptr<Serializable> ownerPointerShared = m_CurrentlyDeserializedObject.lock();
					if (ownerPointerShared != nullptr)
					{
						ownerPointerShared->m_Dependencies[targetInstanceID] = (std::weak_ptr<Serializable>*)&weakPointer;
						serializationModule->AddDependency(targetInstanceID, ownerPointerShared);

						success = true;
					}
				}
			}
		}
	}
	else
	{
		success = false;
	}

	return success;
}

#endif // CORE_SERIALIZER_XML_HPP
