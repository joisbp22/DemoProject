#include "GameObjectModule.h"

#include "GameObject.h"

#include "GameObjectSystem/GameObject.h"
#include "GameObjectSystem/Components/ComponentVisual.h"
#include "GameObjectSystem/Components/ComponentTransform2D.h"
#include "GameObjectSystem/Components/Physics/ComponentRigidBody.h"
#include "GameObjectSystem/Components/Physics/ComponentColliderBox.h"
#include "GameObjectSystem/Components/Physics/ComponentColliderLine.h"

#include "Serialization/SerializationModule.h"

#include "tinyxml.h"


IMPLEMENT_MODULE(GameObjectModule)

GameObjectModule::GameObjectModule()
	: ModuleBase()
{
}

GameObjectModule::~GameObjectModule()
{

}

void GameObjectModule::RegisterSerializables(Factory<Serializable>& factory)
{
	REGISTER_FACTORY_CONSTRUCTABLE(GameObject, factory);
	REGISTER_FACTORY_CONSTRUCTABLE(ComponentVisual, factory);
	REGISTER_FACTORY_CONSTRUCTABLE(ComponentTransform2D, factory);
	REGISTER_FACTORY_CONSTRUCTABLE(ComponentRigidBody, factory);
	REGISTER_FACTORY_CONSTRUCTABLE(ComponentColliderBox, factory);
	REGISTER_FACTORY_CONSTRUCTABLE(ComponentColliderLine, factory);
}

bool GameObjectModule::Initialize()
{
	bool success = true;

	// for debug
	LoadPrefabs("Data/Prefabs.xml");

	return success;
}

void GameObjectModule::Shutdown()
{
	if (m_Root != nullptr)
	{
		m_Root->Shutdown();
		m_Root.reset();
	}
}

void GameObjectModule::Update(unsigned long deltaTime)
{
	if (m_Root != nullptr)
	{
		m_Root->Update(deltaTime);

		UpdateTransforms();

		// root should never get destroyed by anything else than this module
		assert(m_Root->IsDestroyed() == false);

		m_Root->GarbageCollect();
	}
}

bool GameObjectModule::LoadScene(const std::string& sceneFilePath)
{
	bool success = false;

	TiXmlDocument doc(sceneFilePath);
	bool fileLoaded = doc.LoadFile();
	assert(fileLoaded == true);
	if (fileLoaded == true)
	{
		const TiXmlElement* rootXMLNode = doc.FirstChildElement();
		assert(rootXMLNode != nullptr);
		if (rootXMLNode != nullptr)
		{
			BEGIN_DESERIALIZE_XML(rootXMLNode);
			bool sceneLoaded = DESERIALIZE_VARIABLE_XML(m_Root);
			if (!sceneLoaded)
			{
				m_Root.reset();
			}
			END_DESERIALIZE();
		}
	}

	if (m_Root != nullptr)
	{
		m_Root->InitializeDirectChildTransforms();

		m_Root->Initialize();
		m_Root->PostInitialize();

		success = true;
	}

	return success;
}

std::weak_ptr<GameObject> GameObjectModule::CloneObject(std::weak_ptr<GameObject> object)
{
	BEGIN_DESERIALIZE_CLONE(object);

	std::shared_ptr<GameObject> newObject;

	std::shared_ptr<SerializationModule> serializationModule = GET_MODULE(SerializationModule).lock();
	if (serializationModule != nullptr)
	{
		newObject = std::static_pointer_cast<GameObject>(serializer->ConstructSerializableObject(serializationModule->GetSerializableFactory(), GameObject::GetType()));
		if (newObject != nullptr)
		{
			newObject->Deserialize(serializer);
		}
	}

	END_DESERIALIZE();

	if (newObject != nullptr)
	{
		newObject->InitializeDirectChildTransforms();

		newObject->Initialize();
		newObject->PostInitialize();

		newObject->SetParent(m_Root);
	}

	return newObject;
}

void GameObjectModule::RegisterModifiedTransformComponent(std::weak_ptr<ComponentTransform2D> modifiedTransform)
{
	for (auto it : m_ModifiedTransforms)
	{
		if (it.lock() == modifiedTransform.lock())
		{
			return;
		}
	}

	m_ModifiedTransforms.push_back(modifiedTransform);
}

void GameObjectModule::UpdateTransforms()
{
	for (auto it : m_ModifiedTransforms)
	{
		std::shared_ptr<ComponentTransform2D> transformComponent = it.lock();
		if (transformComponent != nullptr && transformComponent->IsDirty())
		{
			std::shared_ptr<const GameObject> transformOwner = transformComponent->GetOwner().lock();
			if (transformOwner != nullptr && transformOwner->IsDestroyed() == false)
			{
				transformComponent->UpdateWorldTransform();
			}
		}
	}

	m_ModifiedTransforms.clear();
}

bool GameObjectModule::LoadPrefabs(const std::string& sceneFilePath)
{
	bool success = false;

	TiXmlDocument doc(sceneFilePath);
	bool fileLoaded = doc.LoadFile();
	assert(fileLoaded == true);
	if (fileLoaded == true)
	{
		const TiXmlElement* rootXMLNode = doc.FirstChildElement();
		assert(rootXMLNode != nullptr);
		if (rootXMLNode != nullptr)
		{
			BEGIN_DESERIALIZE_XML(rootXMLNode);
			bool sceneLoaded = DESERIALIZE_VARIABLE_XML(m_Prefabs);
			if (!sceneLoaded)
			{
				m_Prefabs.reset();
			}
			END_DESERIALIZE();
		}
	}

	if (m_Prefabs != nullptr)
	{
		m_Prefabs->InitializeDirectChildTransforms();

		success = true;
	}

	return success;
}
