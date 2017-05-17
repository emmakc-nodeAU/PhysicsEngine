#include "GameObject.h"
#include "PhysicsObject.h"
#include "Gizmos.h"
#include <glm/glm.hpp>
#include "PhysicsSphereShape.h"
#include "PhysicsAABBShape.h"
#include "PhysicsPlaneShape.h"

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
			// Sphere
			// Plane
			// AABB

		default:
			break;
		case PhysicsShape::ShapeType::Plane:
		{
			//Take Physics Shape and pass into type: Plane
			PhysicsPlaneShape* pPlane = (PhysicsPlaneShape*)pShape;
			// World up
			glm::vec3 worldUp = glm::vec3(0, 1, 0);
			// World forward
			glm::vec3 worldForward = glm::vec3(0, 0, 1);
			glm::vec3 right;
			glm::vec3 forward;
			glm::vec3 up;

			// Dot product bn worlds up and normal, return 1 if nearly same direction, if ! same direction
			float fDot = glm::dot(worldUp, pPlane->GetNormal());

			// Wrap others into if statement
			if (glm::abs(fDot) < 0.99f)
			{
				// Create: Rotation Matrix
				right = pPlane->GetNormal();
				// Gets perpendicular vector between right and world up
				// Column 3: Forward vector (per
				forward = glm::cross(right, worldUp);
				// Column 2: Up vector: Cross product b/n FWD and R vec.
				up = glm::cross(forward, right);
			}
			else
			{
				right = pPlane->GetNormal();		
				up = glm::cross(worldForward, right);
				forward = glm::cross(right,up);
			}	

			glm::mat4 transform(1);
			// Column1: Right 
			transform[0] = glm::vec4(right, 0);
			transform[1] = glm::vec4(up, 0);
			transform[2] = glm::vec4(forward, 0);
			transform[3] = glm::vec4(pPlane->GetDistanceFromOrigin() * pPlane->GetNormal(), 1);
			// Position: direction relative to rotation, local direction.
			// eg. Move along normal to get right position, move along x axis.

			// Draw object with X of 1, y and z of 1000x1000, but rotated so x dir matches transform matrix.
			aie::Gizmos::addAABBFilled(glm::vec3(0, 0, 0), 
				glm::vec3(1.0f, 1000.0f, 1000.0f), 
				glm::vec4(1, 0, 1, 1),
				&transform);

		}
			break;
		case PhysicsShape::ShapeType::AABB:
		{
			PhysicsAABBShape* pAABB = (PhysicsAABBShape*)pShape;
			//Do the same with height/length
			aie::Gizmos::addAABB(position, pAABB->GetExtents(), glm::vec4(0, 1, 0, 1));

			// EMMA WAS HERE AT 2:09PM ////////////////////////////////////////

			break;
		}
		case PhysicsShape::ShapeType::Sphere:
		{
			PhysicsSphereShape* pSphere = (PhysicsSphereShape*)pShape;
			aie::Gizmos::addSphere(position, pSphere->GetRadius(), 15, 15, glm::vec4(1, 0, 0, 1));
			break;
		}

		}
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
