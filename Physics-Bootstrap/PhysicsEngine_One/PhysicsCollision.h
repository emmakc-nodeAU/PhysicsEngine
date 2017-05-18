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

	bool CheckCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);
	// Check dir of collision (using Normal and intersect distance)
	// Specific Shape Collision Functions: Each Type
	// 1. Sphere v Plane
	bool CheckSpherePlaneCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);

	// 2. Sphere v Sphere
	// 3. Sphere v AABB

	// 4. Plane v Plane
	// 5. Plane v Sphere
	bool CheckPlaneSphereCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);
	// 6. Plane v AABB

	// 7. AABB v AABB
	// 8. AABB v Sphere
	// 9. AABB v Plane

	void ResolveCollision(PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo& collisionInfo);
	void HandleVelocityChange(PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo& collisionInfo);
	void HandleSeparation(PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo& collisionInfo);
}
