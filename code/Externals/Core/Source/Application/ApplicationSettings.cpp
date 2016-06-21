#include "ApplicationSettings.h"

#include "tinyxml.h"

#include <assert.h>


const std::string ApplicationSettings::k_SettingsFilePath = "Data/ApplicationSettings.xml";

// default settings
const std::string ApplicationSettings::k_DefaultName = "";
const unsigned int ApplicationSettings::k_DefaultScreenWidth = 800;
const unsigned int ApplicationSettings::k_DefaultScreenHeight = 600;
const unsigned int ApplicationSettings::k_DefaultFramerateMax = 50;

ApplicationSettings::ApplicationSettings()
	: m_Name(k_DefaultName)
	, m_ScreenWidth(k_DefaultScreenWidth)
	, m_ScreenHeight(k_DefaultScreenHeight)
	, m_FramerateMax(k_DefaultFramerateMax)
{
}

ApplicationSettings::~ApplicationSettings()
{
}

bool ApplicationSettings::Initialize()
{
	bool success = true;

	// If there is no settings file or if the file doesn't contain the information we need
	// the game will still run but the default settings will be used
	TiXmlDocument doc(k_SettingsFilePath);
	bool isFileLoaded = doc.LoadFile();
	assert(isFileLoaded == true);
	if (isFileLoaded == true)
	{
		const TiXmlElement* applicationXMLNode = doc.FirstChildElement("Application");
		assert(applicationXMLNode != nullptr);
		if (applicationXMLNode != nullptr)
		{
			m_Name = applicationXMLNode->Attribute("name");

			const TiXmlElement* screenSizeXMLNode = applicationXMLNode->FirstChildElement("ScreenSize");
			assert(screenSizeXMLNode != nullptr);
			if (screenSizeXMLNode != nullptr)
			{
				int returnCode = 0;
				returnCode = screenSizeXMLNode->QueryUnsignedAttribute("width", &m_ScreenWidth);
				assert(returnCode == TIXML_SUCCESS);

				returnCode = screenSizeXMLNode->QueryUnsignedAttribute("height", &m_ScreenHeight);
				assert(returnCode == TIXML_SUCCESS);
			}

			const TiXmlElement* framerateMaxXMLNode = applicationXMLNode->FirstChildElement("FramerateMax");
			assert(framerateMaxXMLNode != nullptr);
			if (framerateMaxXMLNode != nullptr)
			{
				int returnCode = 0;
				returnCode = framerateMaxXMLNode->QueryUnsignedAttribute("fps", &m_FramerateMax);
				assert(returnCode == TIXML_SUCCESS);
			}
		}
	}

	return success;
}

const std::string& ApplicationSettings::GetName() const
{
	return m_Name;
}

unsigned int ApplicationSettings::GetScreenWidth() const
{
	return m_ScreenWidth;
}

unsigned int ApplicationSettings::GetScreenHeight() const
{
	return m_ScreenHeight;
}

unsigned int ApplicationSettings::GetFramerateMax() const
{
	return m_FramerateMax;
}
