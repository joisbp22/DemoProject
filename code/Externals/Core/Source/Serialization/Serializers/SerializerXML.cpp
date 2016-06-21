#include "SerializerXML.h"

SerializerXML::SerializerXML(const TiXmlElement* XMLNode)
	: m_CurrentlyParsedXMLNode(XMLNode)
{
}

SerializerXML::~SerializerXML()
{

}

SerializerType SerializerXML::GetType() const
{
	return SerializerType::eXML;
}

bool SerializerXML::DeserializeVariable(const std::string& variableName, float& floatValue)
{
	bool success = true;

	assert(m_CurrentlyParsedXMLNode != nullptr);
	if (m_CurrentlyParsedXMLNode != nullptr)
	{
		const TiXmlElement* variableXMLNode = m_CurrentlyParsedXMLNode->FirstChildElement(variableName);
		if (variableXMLNode != nullptr)
		{
			success = false;

			float tempValue = 0.0f;
			int result = variableXMLNode->QueryFloatAttribute("value", &tempValue);
			assert(result == TIXML_SUCCESS);
			if (result == TIXML_SUCCESS)
			{
				floatValue = tempValue;
				success = true;
			}
		}
	}
	else
	{
		success = false;
	}

	return success;
}

bool SerializerXML::DeserializeVariable(const std::string& variableName, unsigned int& unsignedIntValue)
{
	bool success = true;

	assert(m_CurrentlyParsedXMLNode != nullptr);
	if (m_CurrentlyParsedXMLNode != nullptr)
	{
		const TiXmlElement* variableXMLNode = m_CurrentlyParsedXMLNode->FirstChildElement(variableName);
		if (variableXMLNode != nullptr)
		{
			success = false;

			unsigned int tempValue = 0;
			int result = variableXMLNode->QueryUnsignedAttribute("value", &tempValue);
			assert(result == TIXML_SUCCESS);
			if (result == TIXML_SUCCESS)
			{
				unsignedIntValue = tempValue;
				success = true;
			}
		}
	}
	else
	{
		success = false;
	}

	return success;
}

bool SerializerXML::DeserializeVariable(const std::string& variableName, int& intValue)
{
	bool success = true;

	assert(m_CurrentlyParsedXMLNode != nullptr);
	if (m_CurrentlyParsedXMLNode != nullptr)
	{
		const TiXmlElement* variableXMLNode = m_CurrentlyParsedXMLNode->FirstChildElement(variableName);
		if (variableXMLNode != nullptr)
		{
			success = false;

			int tempValue = 0;
			int result = variableXMLNode->QueryIntAttribute("value", &tempValue);
			assert(result == TIXML_SUCCESS);
			if (result == TIXML_SUCCESS)
			{
				intValue = tempValue;
				success = true;
			}
		}
	}
	else
	{
		success = false;
	}

	return success;
}

bool SerializerXML::DeserializeVariable(const std::string& variableName, std::string& stringValue)
{
	bool success = true;

	assert(m_CurrentlyParsedXMLNode != nullptr);
	if (m_CurrentlyParsedXMLNode != nullptr)
	{
		const TiXmlElement* variableXMLNode = m_CurrentlyParsedXMLNode->FirstChildElement(variableName);
		if (variableXMLNode != nullptr)
		{
			success = false;

			std::string tempValue;
			int result = variableXMLNode->QueryStringAttribute("value", &tempValue);
			assert(result == TIXML_SUCCESS);
			if (result == TIXML_SUCCESS)
			{
				stringValue = tempValue;
				success = true;
			}
		}
	}
	else
	{
		success = false;
	}

	return success;
}
