#pragma once
#include "PhysicsShape.h"
#include "Gizmos.h"
#include <glm/glm.hpp>

class PhysicsAABBShape : public PhysicsShape
{
public:
	PhysicsAABBShape() = delete;
	PhysicsAABBShape(glm::vec3 extents)
		: PhysicsShape(PhysicsShape::ShapeType::AABB)
		, m_extents(extents)
	{

	}
	void SetExtents(glm::vec3 extents) { m_extents = extents; }
	glm::vec3 GetExtents() const { return m_extents; }

private:
	glm::vec3 m_extents;
};