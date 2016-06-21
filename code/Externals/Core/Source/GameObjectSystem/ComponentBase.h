#ifndef CORE_COMPONENT_BASE_H
#define CORE_COMPONENT_BASE_H

#include "Serialization/Serializable.h"

#include "Utils/Factory.h"

class GameObject;
class Vector2D;

class ComponentBase : public Serializable
{
public:
	virtual ~ComponentBase();

	virtual void Initialize();
	virtual void PostInitialize();

	virtual void Shutdown();

	virtual void Destroy();

	virtual void Update(unsigned long deltaTime);

	std::weak_ptr<GameObject> GetOwner() const;
	void SetOwner(std::weak_ptr<GameObject> owner);

	virtual void OnCollisionEvent(std::weak_ptr<const ComponentBase> other, const Vector2D& normal, float displacement);

	virtual void OnSetActive(bool isActive);

protected:
	ComponentBase();

private:
	std::weak_ptr<GameObject> m_Owner;
};

#define DECLARE_COMPONENT(ComponentName, BaseClass) \
	DECLARE_SERIALIZABLE(ComponentName, BaseClass) \
	private:

#define IMPLEMENT_COMPONENT(ComponentName) \
	IMPLEMENT_SERIALIZABLE(ComponentName)

#define GET_COMPONENT(ComponentName) (GetOwner().expired() ? std::weak_ptr<ComponentName>() : GetOwner().lock()->GetComponent<ComponentName>())

#endif // CORE_COMPONENT_BASE_H