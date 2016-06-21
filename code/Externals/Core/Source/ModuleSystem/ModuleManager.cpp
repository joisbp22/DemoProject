#include "ModuleManager.h"

#include "tinyxml.h"
#include <assert.h>

#include "ModuleBase.h"
#include "GameObjectSystem/GameObjectModule.h"
#include "PhysicsSystem/PhysicsModule.h"
#include "Graphics/GraphicsModule.h"
#include "EventSystem/EventModule.h"
#include "InputSystem/InputModule.h"
#include "Serialization/SerializationModule.h"


const std::string ModuleManager::k_SettingsFilePath = "Data/ModuleSettings.xml";


ModuleManager::ModuleManager()
	: Singleton<ModuleManager>()
{
	REGISTER_FACTORY_CONSTRUCTABLE(GameObjectModule, m_ModuleFactory);
	REGISTER_FACTORY_CONSTRUCTABLE(PhysicsModule, m_ModuleFactory);
	REGISTER_FACTORY_CONSTRUCTABLE(GraphicsModule, m_ModuleFactory);
	REGISTER_FACTORY_CONSTRUCTABLE(EventModule, m_ModuleFactory);
	REGISTER_FACTORY_CONSTRUCTABLE(InputModule, m_ModuleFactory);
	REGISTER_FACTORY_CONSTRUCTABLE(SerializationModule, m_ModuleFactory);
}

ModuleManager::~ModuleManager()
{
}

bool ModuleManager::Initialize()
{
	bool success = true;

	success &= LoadModulesFromXML();

	std::shared_ptr<SerializationModule> serializationModule = GET_MODULE(SerializationModule).lock();
	if (serializationModule != nullptr)
	{
		Factory<Serializable>& factory = serializationModule->GetSerializableFactory();

		for (auto it : m_AllModules)
		{
			it.second->RegisterSerializables(factory);
		}
	}

	for (auto it : m_AllModules)
	{
		it.second->Initialize();
	}

	return success;
}

void ModuleManager::Shutdown()
{
	m_UpdateableModules.clear();

	for (auto it : m_AllModules)
	{
		it.second->Shutdown();
	}

	m_AllModules.clear();
}

void ModuleManager::Update(unsigned long deltaTime)
{
	for (auto it : m_UpdateableModules)
	{
		std::shared_ptr<ModuleBase> module = it.lock();
		if (module != nullptr)
			module->Update(deltaTime);
	}
}

bool ModuleManager::LoadModulesFromXML()
{
	bool success = true;

	// If there is no settings file, that means there are no modules, so no game to run -> application will exit
	// If one module fails to load, we assert but we still run the game
	TiXmlDocument doc(k_SettingsFilePath);
	success &= doc.LoadFile();
	assert(success == true);
	if (success == true)
	{
		const TiXmlElement* modulesXMLNode = doc.FirstChildElement("Modules");
		assert(modulesXMLNode != nullptr);
		if (modulesXMLNode != nullptr)
		{
			const TiXmlElement* updateableModulesXMLNode = modulesXMLNode->FirstChildElement("Updateable");
			assert(updateableModulesXMLNode != nullptr);
			if (updateableModulesXMLNode != nullptr)
			{
				const TiXmlElement* moduleXMLNode = updateableModulesXMLNode->FirstChildElement("Module");
				while (moduleXMLNode != nullptr)
				{
					std::string moduleClassName = moduleXMLNode->Attribute("type");

					FactoryConstructedObject<ModuleBase> constructedModule = m_ModuleFactory.Construct(moduleClassName);
					assert(constructedModule.IsValid());
					if (constructedModule.IsValid())
					{
						// every module should be unique
						ModuleMap::const_iterator it = m_AllModules.find(constructedModule.GetObjectType());
						assert(it == m_AllModules.end());
						if (it == m_AllModules.end())
						{
							m_UpdateableModules.push_back(constructedModule.GetConstructedObject());
							m_AllModules[constructedModule.GetObjectType()] = constructedModule.GetConstructedObject();
						}
					}

					moduleXMLNode = moduleXMLNode->NextSiblingElement("Module");
				}
			}

			const TiXmlElement* otherModulesXMLNode = modulesXMLNode->FirstChildElement("Others");
			assert(otherModulesXMLNode != nullptr);
			if (otherModulesXMLNode != nullptr)
			{
				const TiXmlElement* moduleXMLNode = otherModulesXMLNode->FirstChildElement("Module");
				while (moduleXMLNode != nullptr)
				{
					std::string moduleClassName = moduleXMLNode->Attribute("type");

					FactoryConstructedObject<ModuleBase> constructedModule = m_ModuleFactory.Construct(moduleClassName);
					assert(constructedModule.IsValid());
					if (constructedModule.IsValid())
					{
						// every module should be unique
						ModuleMap::const_iterator it = m_AllModules.find(constructedModule.GetObjectType());
						assert(it == m_AllModules.end());
						if (it == m_AllModules.end())
						{
							m_UpdateableModules.push_back(constructedModule.GetConstructedObject());
							m_AllModules[constructedModule.GetObjectType()] = constructedModule.GetConstructedObject();
						}
					}

					moduleXMLNode = moduleXMLNode->NextSiblingElement("Module");
				}
			}
		}
	}

	success &= m_AllModules.empty() == false;

	return success;
}
