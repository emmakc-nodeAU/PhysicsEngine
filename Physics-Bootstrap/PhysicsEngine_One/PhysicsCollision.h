#pragma once
#include <glm/glm.hpp>
#include "PhysicsObject.h"

class PhysicsObject;

// Collision System need to store data
// Classes part of same system, but dont store a state - place in namespace in bundle.

namespace PhysicsCollision
{
	struct CollisionInfo
	{
		bool wasCollision;
		glm::vec3 normal;
		float interceptDistance;
	};

	// Sphere v Sphere - refer to miguelcasillas.com
	struct SphereToSphere
	{
		glm::vec3 m_vectorCenter;
		float m_fRadius;
	};

	bool CheckCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);
	// Check dir of collision (using Normal and intersect distance)
	// Specific Shape Collision Functions: Each Type

	// 1. Sphere v Plane
	bool CheckSpherePlaneCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);
	// 2. Plane v Sphere
	bool CheckPlaneSphereCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);

	// 3. Sphere v Sphere
	bool CheckSphereSphereCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);

	// 4. AABB v AABB
	bool CheckAABBAABBCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);

	// 5. AABB v Sphere
	bool CheckAABBSphereCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);
	// 6. Sphere v AABB
	bool CheckSphereAABBCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);

	// 7. AABB v Plane
	bool CheckAABBPlaneCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);

	void ResolveCollision(PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo& collisionInfo);
	void HandleVelocityChange(PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo& collisionInfo);
	void HandleSeparation(PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo& collisionInfo);
}
