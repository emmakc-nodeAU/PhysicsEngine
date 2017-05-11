#include "GameObject.h"
#include "PhysicsObject.h"
#include "Gizmos.h"
#include <glm/glm.hpp>
#include "PhysicsSphereShape.h"

GameObject::GameObject()
	: m_physicsObject(nullptr)
{
}

void GameObject::DebugPhysicsRender()
{
	if (m_physicsObject != nullptr)
	{
		//Render Gameobject if any exist.
		glm::vec3 position = m_physicsObject->GetPosition();
		// 
		PhysicsShape* pShape = m_physicsObject->GetShape();
		if (pShape == nullptr) return;		// Defensive coding
		switch (pShape->GetType())
		{
			// Plane
			// AABB

		default:
			break;
		case PhysicsShape::ShapeType::Plane:
		{

		}
			break;
		case PhysicsShape::ShapeType::AABB:
		{

		}
		case PhysicsShape::ShapeType::Sphere:
		{
			PhysicsSphereShape* pSphere = (PhysicsSphereShape*)pShape;
			aie::Gizmos::addSphere(position, pSphere->GetRadius(), 15, 15, glm::vec4(1, 0, 0, 1));
			break;
		}

		}
		// OLD: PreSwitch: aie::Gizmos::addSphere(position, 1.0f, 15, 15, glm::vec4(1, 0, 0, 1));
	}
}

void GameObject::SetPhysicsObject(PhysicsObject * physicsObject)
{
	m_physicsObject = physicsObject;
}

PhysicsObject * GameObject::GetPhysicsObject() const
{
	return m_physicsObject;
}
