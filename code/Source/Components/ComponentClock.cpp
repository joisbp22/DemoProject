#include "ComponentClock.h"

#include "GameObjectSystem/GameObjectModule.h"
#include "GameObjectSystem/GameObject.h"

#include "GameObjectSystem/Components/ComponentTransform2D.h"

#include "Serialization/SerializationModule.h"

#include "Utils/time.h"

IMPLEMENT_COMPONENT(ComponentClock)

ComponentClock::ComponentClock()
{

}

ComponentClock::~ComponentClock()
{

}

bool ComponentClock::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_HoursLinePrefab, ComponentClock);
	assert(success);
	success &= DESERIALIZE_VARIABLE(m_MinutesLinePrefab, ComponentClock);
	assert(success);
	success &= DESERIALIZE_VARIABLE(m_SecondsLinePrefab, ComponentClock);
	assert(success);

	return success;
}

void ComponentClock::Initialize()
{
	std::shared_ptr<GameObjectModule> gameObjectModule = GET_MODULE(GameObjectModule).lock();
	if (gameObjectModule != nullptr)
	{
		std::shared_ptr<GameObject> newHoursLine = gameObjectModule->CloneObject(m_HoursLinePrefab).lock();
		if (newHoursLine != nullptr)
		{
			newHoursLine->SetParent(GetOwner());
			m_HoursLineTransformComponent = newHoursLine->GetComponent<ComponentTransform2D>();
		}

		std::shared_ptr<GameObject> newMinutesLine = gameObjectModule->CloneObject(m_MinutesLinePrefab).lock();
		if (newMinutesLine != nullptr)
		{
			newMinutesLine->SetParent(GetOwner());
			m_MinutesLineTransformComponent = newMinutesLine->GetComponent<ComponentTransform2D>();
		}

		std::shared_ptr<GameObject> newSecondsLine = gameObjectModule->CloneObject(m_SecondsLinePrefab).lock();
		if (newSecondsLine != nullptr)
		{
			newSecondsLine->SetParent(GetOwner());
			m_SecondsLineTransformComponent = newSecondsLine->GetComponent<ComponentTransform2D>();
		}
	}

	SetClock();
}

void ComponentClock::Update(unsigned long deltaTime)
{
	SetClock();
}

void ComponentClock::SetClock()
{
	int hours = 0;
	int minutes = 0;
	int seconds = 0;
	GetTime(hours, minutes, seconds);

	float hoursAngleInDegrees = (float)hours / 24.0f * 360.0f;
	float minutesAngleInDegrees = (float)minutes / 60.0f * 360.0f;
	float secondsAngleInDegrees = (float)seconds / 60.0f * 360.0f;

	std::shared_ptr<ComponentTransform2D> hoursTransformComponent = m_HoursLineTransformComponent.lock();
	if (hoursTransformComponent != nullptr)
	{
		Transform2D newLocalTransform;
		newLocalTransform.m_Matrix.SetRotation(hoursAngleInDegrees);
		hoursTransformComponent->SetLocalTransform(newLocalTransform);
	}

	std::shared_ptr<ComponentTransform2D> minutesTransformComponent = m_MinutesLineTransformComponent.lock();
	if (minutesTransformComponent != nullptr)
	{
		Transform2D newLocalTransform;
		newLocalTransform.m_Matrix.SetRotation(minutesAngleInDegrees);
		minutesTransformComponent->SetLocalTransform(newLocalTransform);
	}

	std::shared_ptr<ComponentTransform2D> secondsTransformComponent = m_SecondsLineTransformComponent.lock();
	if (secondsTransformComponent != nullptr)
	{
		Transform2D newLocalTransform;
		newLocalTransform.m_Matrix.SetRotation(secondsAngleInDegrees);
		secondsTransformComponent->SetLocalTransform(newLocalTransform);
	}
}
