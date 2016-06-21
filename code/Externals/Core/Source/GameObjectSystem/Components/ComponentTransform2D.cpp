#include "ComponentTransform2D.h"

#include "Math/Transform2D.h"

#include "Serialization/SerializationModule.h"

#include "GameObjectSystem/GameObjectModule.h"
#include "GameObjectSystem/GameObject.h"


IMPLEMENT_COMPONENT(ComponentTransform2D)

ComponentTransform2D::ComponentTransform2D()
	: m_IsDirty(false)
{

}

ComponentTransform2D::~ComponentTransform2D()
{

}

bool ComponentTransform2D::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_LocalTransform2D, ComponentTransform2D);
	assert(success);

	return success;
}

void ComponentTransform2D::Initialize()
{
	m_GameObjectModule = GET_MODULE(GameObjectModule);

	m_WorldTransform2D = m_LocalTransform2D;
	std::shared_ptr<GameObject> owner = GetOwner().lock();
	if (owner != nullptr)
	{
		std::shared_ptr<ComponentTransform2D> parentTransform = owner->GetParentTransforms().lock();
		if (parentTransform != nullptr)
		{
			m_WorldTransform2D = parentTransform->GetWorldTransform().LocalToWorld(m_LocalTransform2D);
		}
	}
}

void ComponentTransform2D::PostInitialize()
{

}

void ComponentTransform2D::Shutdown()
{

}

void ComponentTransform2D::Destroy()
{

}

void ComponentTransform2D::Update(unsigned long deltaTime)
{

}

void ComponentTransform2D::UpdateWorldTransform()
{
	m_WorldTransform2D = m_LocalTransform2D;

	std::shared_ptr<GameObject> owner = GetOwner().lock();
	if (owner != nullptr)
	{
		std::shared_ptr<ComponentTransform2D> parentTransform = owner->GetParentTransforms().lock();
		if (parentTransform != nullptr)
		{
			m_WorldTransform2D = parentTransform->GetWorldTransform().LocalToWorld(m_LocalTransform2D);
		}

		const GameObject::DirectChildrenTransformMap& directChildTransforms = owner->GetDirectChildTransforms();
		for (auto mapIt : directChildTransforms)
		{
			for (auto vectorIt : mapIt.second)
			{
				std::shared_ptr<ComponentTransform2D> childTransform = vectorIt.lock();
				if (childTransform != nullptr)
				{
					childTransform->UpdateWorldTransform(m_WorldTransform2D);
				}
			}
		}
	}

	m_IsDirty = false;
}

bool ComponentTransform2D::IsDirty() const
{
	return m_IsDirty;
}

const Transform2D& ComponentTransform2D::GetLocalTransform() const
{
	return m_LocalTransform2D;
}

const Transform2D& ComponentTransform2D::GetWorldTransform() const
{
	return m_WorldTransform2D;
}

void ComponentTransform2D::SetLocalTransform(const Transform2D& newLocalTransform)
{
	m_LocalTransform2D = newLocalTransform;

	SetDirty();
}

void ComponentTransform2D::SetWorldPosition(const Point2D& newWorldPosition)
{
	m_WorldTransform2D.m_Position = newWorldPosition;

	std::shared_ptr<GameObject> owner = GetOwner().lock();
	if (owner != nullptr)
	{
		std::shared_ptr<ComponentTransform2D> parentTransform = owner->GetParentTransforms().lock();
		if (parentTransform != nullptr)
		{
			m_LocalTransform2D = parentTransform->GetWorldTransform().WorldToLocal(m_WorldTransform2D);
		}
	}

	SetDirty();
}

void ComponentTransform2D::Translate(const Vector2D& translation)
{
	m_LocalTransform2D.m_Position += translation;

	SetDirty();
}

void ComponentTransform2D::SetLocalScale(float uniformScale)
{
	m_LocalTransform2D.m_Scale = uniformScale;

	SetDirty();
}

void ComponentTransform2D::Scale(float uniformScale)
{
	m_LocalTransform2D.m_Scale *= uniformScale;

	SetDirty();
}

void ComponentTransform2D::SetWorldRotation(const Matrix2D& newRotation)
{
	m_WorldTransform2D.m_Matrix = newRotation;

	std::shared_ptr<GameObject> owner = GetOwner().lock();
	if (owner != nullptr)
	{
		std::shared_ptr<ComponentTransform2D> parentTransform = owner->GetParentTransforms().lock();
		if (parentTransform != nullptr)
		{
			m_LocalTransform2D = parentTransform->GetWorldTransform().WorldToLocal(m_WorldTransform2D);
		}
	}

	SetDirty();
}

void ComponentTransform2D::SetLocalRotation(const Matrix2D& newRotation)
{
	m_LocalTransform2D.m_Matrix = newRotation;

	SetDirty();
}

void ComponentTransform2D::Rotate(float angleInDegrees)
{
	m_LocalTransform2D.m_Matrix.Rotate(angleInDegrees);

	SetDirty();
}

void ComponentTransform2D::UpdateWorldTransform(const Transform2D& parentTransform)
{
	std::shared_ptr<GameObject> owner = GetOwner().lock();
	if (owner != nullptr)
	{
		m_WorldTransform2D = parentTransform.LocalToWorld(m_LocalTransform2D);

		const GameObject::DirectChildrenTransformMap& directChildTransforms = owner->GetDirectChildTransforms();
		for (auto mapIt : directChildTransforms)
		{
			for (auto vectorIt : mapIt.second)
			{
				std::shared_ptr<ComponentTransform2D> childTransform = vectorIt.lock();
				if (childTransform != nullptr)
				{
					childTransform->UpdateWorldTransform(m_WorldTransform2D);
				}
			}
		}
	}

	m_IsDirty = false;
}

void ComponentTransform2D::SetDirty()
{
	m_IsDirty = true;

	std::shared_ptr<GameObjectModule> gameObjectModule = m_GameObjectModule.lock();
	if (gameObjectModule != nullptr)
	{
		gameObjectModule->RegisterModifiedTransformComponent(std::static_pointer_cast<ComponentTransform2D>(m_This.lock()));
	}
}
