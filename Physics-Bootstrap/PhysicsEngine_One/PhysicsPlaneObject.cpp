#include "PhysicsPlaneObject.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Gizmos.h"

using glm::vec2;
using glm::vec4;
using aie::Gizmos;

PhysicsPlaneShape::PhysicsPlaneShape()
{
}

PhysicsPlaneShape::PhysicsPlaneShape(glm::vec2 normal, float distance)
{
}

PhysicsPlaneShape::~PhysicsPlaneShape()
{
}

void PhysicsPlaneShape::fixedUpdate(glm::vec2 gravity, float timeStep)
{
}

void PhysicsPlaneShape::makeGizmo()
{
	float lineSegmentLength = 300;
	vec2 centerPoint = m_normal * m_distanceToOrigin;
	vec2 parallel(m_normal.y, -m_normal.x);
	vec4 colour(1, 1, 1, 1);
	vec2 start = centerPoint + (parallel * lineSegmentLength);
	vec2 end = centerPoint - (parallel * lineSegmentLength);
	aie::Gizmos::add2DLine(start, end, colour);
}

void PhysicsPlaneShape::resetPosition()
{
}
