#pragma once
#include "PhysicsShape.h"
#include "Gizmos.h"
#include <glm/glm.hpp>

class PhysicsAABBShape : public PhysicsShape
{
public:
	PhysicsAABBShape() = delete;
	PhysicsAABBShape(float extents, float width, float height)
		: PhysicsShape(PhysicsShape::ShapeType::AABB)
		, m_extents(extents)
		, m_width (width)
		, m_height (height)
	{

	}

	void SetExtents(glm::vec3 extents) { m_extents = extents; }
	glm::vec3 GetExtents() const { return m_extents; }

	void SetWidth(glm::vec3 width) { m_width = width; }
	glm::vec3 GetWidth() const { return m_width; }

	void SetHeight(glm::vec3 height) { m_height = height; }
	glm::vec3 GetHeight() const { return m_height; }

private:
	glm::vec3 m_extents;
	glm::vec3 m_width;
	glm::vec3 m_height;
	//glm::vec3 pShape;
};