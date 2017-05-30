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

//	void SetWidth(glm::vec3 width) { m_width = width; }
//	float GetWidth() const { return m_width; }
//
//	void SetHeight(glm::vec3 height) { m_height = height; }
//	float GetHeight() const { return m_height; }

private:
	glm::vec3 m_extents;
	//glm::vec3 m_width;
	//glm::vec3 m_height;
	//glm::vec3 pShape;
};