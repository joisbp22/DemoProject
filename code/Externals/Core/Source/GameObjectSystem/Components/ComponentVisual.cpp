#include "ComponentVisual.h"

#include "Serialization/SerializationModule.h"

#include "Graphics/Renderables/IRenderable.h"
#include "ComponentTransform2D.h"
#include "GameObjectSystem/GameObject.h"

IMPLEMENT_COMPONENT(ComponentVisual)

ComponentVisual::ComponentVisual()
	: ComponentBase()
{

}

ComponentVisual::~ComponentVisual()
{

}

bool ComponentVisual::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_Renderable, ComponentVisual);
	assert(success);

	return success;
}

void ComponentVisual::Initialize()
{
	m_Renderable->RegisterRenderable();
	m_TransformComponent = GET_COMPONENT(ComponentTransform2D);
}

void ComponentVisual::Shutdown()
{
	m_Renderable->UnregisterRenderable();
}

void ComponentVisual::Update(unsigned long deltaTime)
{
	std::shared_ptr<ComponentTransform2D> transformComponent = m_TransformComponent.lock();
	if (transformComponent != nullptr)
	{
		const Transform2D& worldTransform = transformComponent->GetWorldTransform();

		m_Renderable->ApplyTransform(worldTransform);
	}
}

void ComponentVisual::OnSetActive(bool isActive)
{
	if (isActive)
	{
		m_Renderable->RegisterRenderable();
	}
	else
	{
		m_Renderable->UnregisterRenderable();
	}
}
