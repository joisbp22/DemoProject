#ifndef CORE_PHYSICS_MODULE_H
#define CORE_PHYSICS_MODULE_H

#include "ModuleSystem/ModuleBase.h"

#include "AABB.h"

class PhysicsModule : public ModuleBase
{
	DECLARE_MODULE(PhysicsModule, ModuleBase)

public:
	PhysicsModule();
	virtual ~PhysicsModule() override;

	virtual void Update(unsigned long deltaTime) override;

	void RegisterCollideable(std::weak_ptr<ICollideable> collideable);
	void UnregisterCollideable(std::weak_ptr<ICollideable> collideable);

private:

	struct CollidingPair
	{
		std::weak_ptr<ICollideable> m_Collideable1;
		std::weak_ptr<ICollideable> m_Collideable2;

		Vector2D m_Normal;
		float m_Displacement = 0.0f;
	};

	std::vector<AABB> m_AABBVector;
};

#endif // CORE_PHYSICS_MODULE_H