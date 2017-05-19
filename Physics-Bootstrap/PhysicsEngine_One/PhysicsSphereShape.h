#pragma once
#include "PhysicsShape.h"

class PhysicsSphereShape : public PhysicsShape
{
public:
	PhysicsSphereShape() = delete;

	PhysicsSphereShape(float radius)
		: PhysicsShape(PhysicsShape::ShapeType::Sphere)
		, m_radius(radius)
	{
	}

	void SetRadius(float radius) { m_radius = radius; }

	float GetRadius() const { return m_radius; }

private:
	float m_radius;
};