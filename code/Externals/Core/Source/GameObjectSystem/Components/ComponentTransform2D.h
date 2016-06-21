#ifndef CORE_COMPONENT_TRANSFORM_2D_H
#define CORE_COMPONENT_TRANSFORM_2D_H

#include "GameObjectSystem/ComponentBase.h"

#include "Math/Transform2D.h"

class GameObjectModule;

class ComponentTransform2D : public ComponentBase
{
	DECLARE_COMPONENT(ComponentTransform2D, ComponentBase)

public:
						ComponentTransform2D();
	virtual				~ComponentTransform2D();

	virtual bool		Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	virtual void		Initialize() override;
	virtual void		PostInitialize() override;

	virtual void		Shutdown() override;

	virtual void		Destroy() override;

	virtual void		Update(unsigned long deltaTime) override;

	void				UpdateWorldTransform();

	bool				IsDirty() const;

	const Transform2D&	GetLocalTransform() const;
	const Transform2D&	GetWorldTransform() const;

	void				SetLocalTransform(const Transform2D& newLocalTransform);

	void				SetWorldPosition(const Point2D& newWorldPosition);
	void				Translate(const Vector2D& translation);
	void				SetLocalScale(float uniformScale);
	void				Scale(float uniformScale);
	void				SetWorldRotation(const Matrix2D& newRotation);
	void				SetLocalRotation(const Matrix2D& newRotation);
	void				Rotate(float angleInDegrees);

private:

	void				UpdateWorldTransform(const Transform2D& parentTransform);
	void				SetDirty();

	Transform2D m_LocalTransform2D;
	Transform2D m_WorldTransform2D;

	bool m_IsDirty;

	std::weak_ptr<GameObjectModule> m_GameObjectModule;
};

#endif // CORE_COMPONENT_TRANSFORM_2D_H
