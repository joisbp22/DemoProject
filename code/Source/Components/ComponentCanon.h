#ifndef GAME_COMPONENT_CANON_H
#define GAME_COMPONENT_CANON_H

#include "GameObjectSystem/ComponentBase.h"

#include "Serialization/Serializable.h"

#include "Math/Matrix2D.h"

class InputModule;
class ComponentTransform2D;
class ComponentProjectileManager;

class ComponentCanon : public ComponentBase
{
	DECLARE_COMPONENT(ComponentCanon, ComponentBase)

public:
	ComponentCanon();
	virtual ~ComponentCanon() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	virtual void Initialize() override;

	virtual void Update(unsigned long deltaTime) override;

private:

	void ApplyRotation();

	void CheckFireRate(unsigned long deltaTime);

	std::weak_ptr<InputModule> m_InputModule;
	std::weak_ptr<ComponentTransform2D> m_TransformComponent;

	std::weak_ptr<ComponentProjectileManager> m_ProjectileManager;
	float m_RotationSpeed;
	float m_MinRotationInDegrees;
	float m_MaxRotationInDegrees;
	float m_CurrentRotationInDegrees;

	Matrix2D m_InitialLocalRotation;

	float m_TimeBetweenShots;
	float m_TimeUntilNextShot;
};

#endif // GAME_COMPONENT_CANON_H
