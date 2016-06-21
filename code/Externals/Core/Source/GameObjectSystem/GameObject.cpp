#include "GameObject.h"

#include "ComponentBase.h"

#include "Components/ComponentTransform2D.h"

#include "Serialization/SerializationModule.h"


IMPLEMENT_SERIALIZABLE(GameObject)


GameObject::GameObject()
	: m_IsDestroyed(false)
	, m_IsActive(true)
	, m_IsInitialized(false)
	, m_ParentTransformBranchIndex(0)
{

}

GameObject::~GameObject()
{
}

bool GameObject::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_Components, GameObject);
	assert(success);
	success &= DESERIALIZE_VARIABLE(m_Children, GameObject);
	assert(success);

	for (auto it : m_Components)
	{
		it->SetOwner(std::static_pointer_cast<GameObject>(m_This.lock()));
	}

	for (auto it : m_Children)
	{
		it->SetParent(std::static_pointer_cast<GameObject>(m_This.lock()), false);
	}

	return success;
}

void GameObject::Initialize()
{
	if (m_IsInitialized)
		return;

	for (auto it : m_Components)
	{
		it->Initialize();
	}

	// in case components do re-parenting of game objects during initialization, we have to make a copy
	std::vector<std::shared_ptr<GameObject> > children (m_Children);
	for (auto it : children)
	{
		it->Initialize();
	}
}

void GameObject::PostInitialize()
{
	for (auto it : m_Components)
	{
		it->PostInitialize();
	}

	// in case components do re-parenting of game objects during initialization, we have to make a copy
	std::vector<std::shared_ptr<GameObject> > children(m_Children);
	for (auto it : children)
	{
		it->PostInitialize();
	}

	m_IsInitialized = true;
}

void GameObject::Shutdown()
{
	for (auto it : m_Components)
	{
		it->Shutdown();
	}

	// in case components do re-parenting of game objects during shutdown (which is weird to be honest), we have to make a copy
	std::vector<std::shared_ptr<GameObject> > children(m_Children);
	for (auto it : children)
	{
		it->Shutdown();
	}
}

void GameObject::Destroy()
{
	m_IsDestroyed = true;
}

void GameObject::Update(unsigned long deltaTime)
{
	if (m_IsActive)
	{
		for (auto it : m_Components)
		{
			it->Update(deltaTime);
		}

		// in case components do re-parenting of game objects during update, we have to make a copy
		std::vector<std::shared_ptr<GameObject> > children(m_Children);
		for (auto it : children)
		{
			it->Update(deltaTime);
		}
	}
}

void GameObject::GarbageCollect()
{
	std::vector<std::shared_ptr<GameObject> >::iterator it = m_Children.begin();
	for (; it != m_Children.end(); )
	{
		if ((*it)->IsDestroyed())
		{
			it = m_Children.erase(it);
		}
		else
		{
			(*it)->GarbageCollect();
			++it;
		}
	}
}

void GameObject::SetParent(std::weak_ptr<GameObject> parent, bool autoAddToParentList)
{
	std::shared_ptr<GameObject> oldParentShared = m_Parent.lock();
	std::shared_ptr<GameObject> newParentShared = parent.lock();

	// so we don't kill ourselves after being removed from our parent
	std::shared_ptr<GameObject> thisShared = std::static_pointer_cast<GameObject>(m_This.lock());

	if (oldParentShared != nullptr)
	{
		oldParentShared->RemoveChild(thisShared);
	}

	if (autoAddToParentList && newParentShared != nullptr)
	{
		newParentShared->AddChild(thisShared);
	}

	m_Parent = parent;
}

void GameObject::SetActive(bool isActive)
{
	m_IsActive = isActive;

	for (auto it : m_Components)
	{
		it->OnSetActive(isActive);
	}

	// in case components do re-parenting of game objects during set active, we have to make a copy
	std::vector<std::shared_ptr<GameObject> > children(m_Children);
	for (auto it : children)
	{
		it->OnSetActive(isActive);
	}
}

bool GameObject::IsDestroyed() const
{
	return m_IsDestroyed;
}

std::weak_ptr<ComponentTransform2D> GameObject::GetParentTransforms() const
{
	return m_ParentTransform;
}

const GameObject::DirectChildrenTransformMap& GameObject::GetDirectChildTransforms() const
{
	return m_DirectChildTransforms;
}

void GameObject::InitializeDirectChildTransforms()
{
	m_DirectChildTransforms.clear();

	std::weak_ptr<ComponentTransform2D> parentTransform;

	for (auto it : m_Components)
	{
		if (it->IsTypeOf(ComponentTransform2D::GetType()))
		{
			parentTransform = std::static_pointer_cast<ComponentTransform2D>(it);
			break;
		}
	}

	unsigned int childIndex = 0;
	for (auto it : m_Children)
	{
		DirectChildrenTransformVector& directChildTransforms = m_DirectChildTransforms[childIndex];
		it->InitializeDirectChildTransforms(directChildTransforms, parentTransform, childIndex);

		++childIndex;
	}
}

void GameObject::OnCollisionEvent(std::weak_ptr<const ComponentBase> other, const Vector2D& normal, float displacement) const
{
	for (auto it : m_Components)
	{
		it->OnCollisionEvent(other, normal, displacement);
	}
}

void GameObject::AddChild(std::weak_ptr<GameObject> child)
{
	std::shared_ptr<GameObject> childShared = child.lock();
	if (childShared == nullptr)
		return;

	DirectChildrenTransformMap& directChildTransforms = m_DirectChildTransforms;
	std::weak_ptr<ComponentTransform2D> newParentTransform = GetComponent<ComponentTransform2D>();
	unsigned int newBranchIndex = m_Children.size();
	if (newParentTransform.expired())
	{
		newParentTransform = m_ParentTransform;
		std::shared_ptr<ComponentTransform2D> parentTransform = m_ParentTransform.lock();
		if (parentTransform != nullptr)
		{
			std::shared_ptr<GameObject> parentGameObject = parentTransform->GetOwner().lock();
			if (parentGameObject != nullptr)
			{
				directChildTransforms = parentGameObject->m_DirectChildTransforms;
				newBranchIndex = parentGameObject->m_ParentTransformBranchIndex;
			}
		}
	}

	std::weak_ptr<ComponentTransform2D> childTransformComponent = childShared->GetComponent<ComponentTransform2D>();
	if (childTransformComponent.expired())
	{
		std::shared_ptr<ComponentTransform2D> childParentTransform = childShared->m_ParentTransform.lock();
		if (childParentTransform != nullptr)
		{
			std::shared_ptr<GameObject> childParentGameObject = childParentTransform->GetOwner().lock();
			if (childParentGameObject != nullptr)
			{
				DirectChildrenTransformMap::const_iterator oldParentTransforms = childParentGameObject->m_DirectChildTransforms.find(childShared->m_ParentTransformBranchIndex);
				assert(oldParentTransforms != childParentGameObject->m_DirectChildTransforms.end());
				if (oldParentTransforms != childParentGameObject->m_DirectChildTransforms.end())
				{
					DirectChildrenTransformVector& newDirectChildrenVector = directChildTransforms[m_Children.size()];
					newDirectChildrenVector.insert(newDirectChildrenVector.end(), oldParentTransforms->second.begin(), oldParentTransforms->second.end());
				}
			}
		}

		for (auto mapIt : childShared->m_DirectChildTransforms)
		{
			for (auto vectorIt : mapIt.second)
			{
				std::shared_ptr<ComponentTransform2D> childTransform = vectorIt.lock();
				if (childTransform != nullptr)
				{
					std::shared_ptr<GameObject> childOwner = childTransform->GetOwner().lock();
					if (childOwner != nullptr)
					{
						childOwner->m_ParentTransformBranchIndex = newBranchIndex;
						childOwner->m_ParentTransform = newParentTransform;
					}

					std::shared_ptr<ComponentTransform2D> newParentTransformShared = newParentTransform.lock();
					if (newParentTransformShared != nullptr)
					{
						const Transform2D& parentWorldTransform = newParentTransformShared->GetWorldTransform();
						childTransform->SetLocalTransform(parentWorldTransform.WorldToLocal(childTransform->GetWorldTransform()));
					}
				}
			}
		}
	}
	else
	{
		directChildTransforms[m_Children.size()].push_back(childTransformComponent);
		childShared->m_ParentTransformBranchIndex = newBranchIndex;
		childShared->m_ParentTransform = newParentTransform;

		std::shared_ptr<ComponentTransform2D> newParentTransformShared = newParentTransform.lock();
		std::shared_ptr<ComponentTransform2D> childTransformComponentShared = childTransformComponent.lock();
		if (newParentTransformShared != nullptr && childTransformComponentShared != nullptr)
		{
			const Transform2D& parentWorldTransform = newParentTransformShared->GetWorldTransform();
			const Transform2D& childWorldTransform = childTransformComponentShared->GetWorldTransform();
			Transform2D childLocalTransform = parentWorldTransform.WorldToLocal(childWorldTransform);
			childTransformComponentShared->SetLocalTransform(childLocalTransform);
		}
	}

	m_Children.push_back(childShared);
}

void GameObject::RemoveChild(std::weak_ptr<GameObject> child)
{
	std::shared_ptr<GameObject> childShared = child.lock();
	if (childShared == nullptr)
		return;

	std::shared_ptr<ComponentTransform2D> childParentTransform = childShared->m_ParentTransform.lock();
	if (childParentTransform != nullptr)
	{
		std::shared_ptr<GameObject> childParentGameObject = childParentTransform->GetOwner().lock();
		if (childParentGameObject != nullptr)
		{
			childParentGameObject->m_DirectChildTransforms.erase(childShared->m_ParentTransformBranchIndex);
		}
	}

	childShared->m_ParentTransformBranchIndex = 0;
	childShared->m_ParentTransform.reset();

	std::weak_ptr<ComponentTransform2D> childTransformComponent = childShared->GetComponent<ComponentTransform2D>();
	if (childTransformComponent.expired())
	{
		for (auto mapIt : childShared->m_DirectChildTransforms)
		{
			for (auto vectorIt : mapIt.second)
			{
				std::shared_ptr<ComponentTransform2D> childTransform = vectorIt.lock();
				if (childTransform != nullptr)
				{
					std::shared_ptr<GameObject> childOwner = childTransform->GetOwner().lock();
					if (childOwner != nullptr)
					{
						childOwner->m_ParentTransformBranchIndex = 0;
						childOwner->m_ParentTransform.reset();
					}

					childTransform->SetLocalTransform(Transform2D());
				}
			}
		}
	}
	else
	{
		std::shared_ptr<ComponentTransform2D> childTransform = childTransformComponent.lock();
		if (childTransform != nullptr)
		{
			childTransform->SetLocalTransform(Transform2D());
		}
	}

	std::vector<std::shared_ptr<GameObject> >::iterator it = m_Children.begin();
	for (; it != m_Children.end(); )
	{
		if (*it == childShared)
		{
			it = m_Children.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void GameObject::InitializeDirectChildTransforms(DirectChildrenTransformVector& parentDirectChildrenTransform, std::weak_ptr<ComponentTransform2D> parentTransform, unsigned int branchIndex)
{
	m_DirectChildTransforms.clear();

	bool foundTransformComponent = false;

	for (auto it : m_Components)
	{
		if (it->IsTypeOf(ComponentTransform2D::GetType()))
		{
			std::weak_ptr<ComponentTransform2D> newTransformFound = std::static_pointer_cast<ComponentTransform2D>(it);
			parentDirectChildrenTransform.push_back(newTransformFound);

			m_ParentTransform = parentTransform;
			m_ParentTransformBranchIndex = branchIndex;

			parentTransform = newTransformFound;
			foundTransformComponent = true;
			break;
		}
	}

	if (foundTransformComponent)
	{
		unsigned int childIndex = 0;
		for (auto it : m_Children)
		{
			DirectChildrenTransformVector& directChildTransforms = m_DirectChildTransforms[childIndex];
			directChildTransforms.clear();
			it->InitializeDirectChildTransforms(directChildTransforms, parentTransform, childIndex);

			++childIndex;
		}
	}
	else
	{
		for (auto it : m_Children)
		{
			it->InitializeDirectChildTransforms(parentDirectChildrenTransform, parentTransform, branchIndex);
			m_DirectChildTransforms[branchIndex] = parentDirectChildrenTransform;
		}
	}
}

void GameObject::OnSetActive(bool isActive)
{
	for (auto it : m_Components)
	{
		it->OnSetActive(isActive);
	}

	for (auto it : m_Children)
	{
		it->OnSetActive(isActive);
	}
}
