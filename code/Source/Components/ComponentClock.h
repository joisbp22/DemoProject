#ifndef GAME_COMPONENT_CLOCK_H
#define GAME_COMPONENT_CLOCK_H

#include "GameObjectSystem/ComponentBase.h"

class GameObject;
class ComponentTransform2D;

class ComponentClock : public ComponentBase
{
	DECLARE_COMPONENT(ComponentClock, ComponentBase)
public:
	ComponentClock();
	virtual ~ComponentClock() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	virtual void Initialize() override;

	virtual void Update(unsigned long deltaTime);

private:

	void SetClock();

	std::weak_ptr<GameObject> m_HoursLinePrefab;
	std::weak_ptr<GameObject> m_MinutesLinePrefab;
	std::weak_ptr<GameObject> m_SecondsLinePrefab;

	std::weak_ptr<ComponentTransform2D> m_HoursLineTransformComponent;
	std::weak_ptr<ComponentTransform2D> m_MinutesLineTransformComponent;
	std::weak_ptr<ComponentTransform2D> m_SecondsLineTransformComponent;
};

#endif // GAME_COMPONENT_CLOCK_H