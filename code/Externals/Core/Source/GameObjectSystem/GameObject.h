#ifndef CORE_GAME_OBJECT_H
#define CORE_GAME_OBJECT_H

#include "Serialization/Serializable.h"

#include "Utils/Factory.h"

#include <vector>

class ComponentBase;
class ComponentTransform2D;
class Vector2D;

class GameObject : public Serializable
{
	DECLARE_SERIALIZABLE(GameObject, Serializable)

public:

	typedef std::vector<std::weak_ptr<ComponentTransform2D> > DirectChildrenTransformVector;
	typedef std::unordered_map<unsigned int, DirectChildrenTransformVector > DirectChildrenTransformMap;

	GameObject();
	~GameObject();

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	void Initialize();
	void PostInitialize();
	void Shutdown();

	void Destroy();

	void Update(unsigned long deltaTime);

	void GarbageCollect();

	void SetParent(std::weak_ptr<GameObject> parent, bool autoAddToParentList = true);
	void SetActive(bool isActive);

	bool IsDestroyed() const;

	template<class T>
	std::weak_ptr<T> GetComponent() const;

	std::weak_ptr<ComponentTransform2D> GetParentTransforms() const;
	const DirectChildrenTransformMap& GetDirectChildTransforms() const;
	void InitializeDirectChildTransforms();

	void OnCollisionEvent(std::weak_ptr<const ComponentBase> other, const Vector2D& normal, float displacement) const;

private:

	void AddChild(std::weak_ptr<GameObject> child);
	void RemoveChild(std::weak_ptr<GameObject> child);

	void InitializeDirectChildTransforms(DirectChildrenTransformVector& parentDirectChildrenTransform, std::weak_ptr<ComponentTransform2D> parentTransform, unsigned int branchIndex);

	void OnSetActive(bool isActive);

	std::vector<std::shared_ptr<ComponentBase> > m_Components;
	std::vector<std::shared_ptr<GameObject> > m_Children;

	DirectChildrenTransformMap m_DirectChildTransforms;
	std::weak_ptr<ComponentTransform2D> m_ParentTransform;
	unsigned int m_ParentTransformBranchIndex;

	std::weak_ptr<GameObject> m_Parent;

	bool m_IsDestroyed;
	bool m_IsActive;
	bool m_IsInitialized;
};

#include "GameObject.hpp"

#endif // CORE_GAME_OBJECT_H
