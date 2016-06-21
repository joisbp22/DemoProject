#ifndef CORE_SERIALIZER_XML_H
#define CORE_SERIALIZER_XML_H

#include "SerializerBase.h"

class TiXmlElement;

class SerializerXML : public SerializerBase
{
public:
	SerializerXML(const TiXmlElement* XMLNode);
	virtual ~SerializerXML() override;

	virtual SerializerType GetType() const override;

	bool DeserializeVariable(const std::string& variableName, float& floatValue);
	bool DeserializeVariable(const std::string& variableName, unsigned int& unsignedIntValue);
	bool DeserializeVariable(const std::string& variableName, int& intValue);
	bool DeserializeVariable(const std::string& variableName, std::string& stringValue);

	template <class T>
	bool DeserializeVariable(const std::string& variableName, T& serializable);

	template <typename T, size_t N>
	bool DeserializeVariableArray(const std::string& variableName, T(&array)[N]);

	template <typename T>
	bool DeserializeVariable(const std::string& variableName, std::vector<T>& vector);

	template <class T>
	bool DeserializeVariable(const std::string& variableName, std::shared_ptr<T>& sharedPointer);

	template <class T>
	bool DeserializeVariable(const std::string& variableName, std::weak_ptr<T>& weakPointer);

private:

	const TiXmlElement* m_CurrentlyParsedXMLNode;
	std::weak_ptr<Serializable> m_CurrentlyDeserializedObject;
};

#include "SerializerXML.hpp"

#endif // CORE_SERIALIZER_H
