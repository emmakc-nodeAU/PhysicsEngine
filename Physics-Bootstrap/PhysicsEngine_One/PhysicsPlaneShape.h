#pragma once
#include "PhysicsShape.h"
#include <glm\glm.hpp>

class PhysicsPlaneShape : public PhysicsShape
{
public:
	PhysicsPlaneShape() = delete;
	PhysicsPlaneShape(glm::vec3 normal, float distanceFromOrigin)
		: PhysicsShape(PhysicsShape::ShapeType::Plane)
		, m_normal(normal)
		, m_distanceFromOrigin(distanceFromOrigin)
	{
	}

	void SetNormal(glm::vec3 normal) { m_normal = normal; }
	glm::vec3 GetNormal() const { return m_normal; }

	void SetDistanceFromOrigin(float distanceFromOrigin){m_distanceFromOrigin = distanceFromOrigin;}
	float GetDistanceFromOrigin() const { return m_distanceFromOrigin; }

private:
	glm::vec3 m_normal;
	float m_distanceFromOrigin;
};