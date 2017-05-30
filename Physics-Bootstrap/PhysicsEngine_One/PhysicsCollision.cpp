#include "PhysicsCollision.h"
#include "PhysicsShape.h"
#include "PhysicsObject.h"

#include "PhysicsPlaneShape.h"
#include "PhysicsSphereShape.h"
#include "PhysicsAABBShape.h"

#include <glm\glm.hpp>


bool PhysicsCollision::CheckCollision(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	/*	Tutorial reference: Collision Detection
		Summary:		Array of functions, identify two shapes colliding, locate correct Collision Check.
		Create:			Function pointers, index thru pointers, return correct collision function based on primitive type
		Differences:	'typedef' instead of 'using'

		1. Create: Function Pointer types 
		2. Create: Array for Collision Functions: specify them in array, ordered by 'ShapeType' enum (note nullptr placeholders until all classes are written)
		3. Create: ShapeID index hash
		4. Check: Locate Collision Function combo, Call Collision combo
		5. Post Collision - See next function to resolve direction/velocity of GameObjects
	*/

	// 1. CREATE: Function Pointer types
	typedef bool(*collisionFnc) (const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);
	// 2. Create: Array of the Collision Functions:
	static collisionFnc collisionFunctionArray[] = {
	nullptr, nullptr, CheckPlaneSphereCollision,
	nullptr, nullptr, nullptr,
	CheckSpherePlaneCollision, nullptr, nullptr
	};

	// 3. Create: ShapeID index 'hash' two shapeIDs:
	auto index = ((int)(obj1->GetShape()->GetType()) * (int)PhysicsShape::ShapeType::NUM_SHAPES) +
		(int)(obj2->GetShape()->GetType());

	// 4. Check Collision Function combo, call it:
	if (collisionFunctionArray[index] != nullptr)
	{
		return collisionFunctionArray[index](obj1, obj2, collisionInfo);
	}
	return false;
}

//bool PhysicsCollision::CheckAABBSphereCollision(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
//{
//	/*
//	Formula:	AABB v Sphere
//
//	*/
//
//	PhysicsAABBShape* pAABB = (PhysicsAABBShape*)obj1->GetShape();
//	PhysicsSphereShape* pSphere = (PhysicsSphereShape*)obj2->GetShape();
//
//}

void PhysicsCollision::ResolveCollision(PhysicsObject * obj1, PhysicsObject * obj2, CollisionInfo& collisionInfo)
{
	if (collisionInfo.wasCollision)
	{
		// Resolve Position and Velocity of GameObject primitives.
		HandleSeparation(obj1, obj2, collisionInfo);
		HandleVelocityChange(obj1, obj2, collisionInfo);
	}
}

void PhysicsCollision::HandleVelocityChange(PhysicsObject * obj1, PhysicsObject * obj2, CollisionInfo& collisionInfo)
{
	/*
	--- CALCULATE VELOCITY RESPONSE ---
	Theory: Conservation of Energy
	Total Energy in a System = The same before and after collision
	Elastic Kinetic Collisions = Kinetic energy before collision is equal to kinetic energy after collision
	Friction above = zero
	
	Sphere v Sphere Collision
	Mass center = center of sphere
	Collision type= Elastic .: no kinetic energy lost at collision
	Formula: V1 & V2 Sphere Velocities: See slide 7/29 Collision Response and Friction

	Sphere (S1) v Plane Collision
	Collision: Elastic: Plane Static/Rigid, Ball bounce off it, no kinetic energy lost
	S1 = Sphere, 
	V1 = S1's velocity before collision
	N = Vector, unit normal of plane
	Calculate Force: Newton's 3rd law: Force the Plane exerts on Sphere
	Sphere Response Collision V2 = V1 + 2*N+(N.V1)
	eg. 
	Sphere V1 (1,0)
	Plane N (-0.702, -0.702)
	Velocity2 = (1,0) + 2 * (-0.702, -0.702)+((-0.702, -0.702) . (1,0))
	V2 .: = (1,0) + 2 * (-0.702, -0.702)+(-0.702,0)
	V2 .: = -1.404
	*/
	

	// TODO: Actually do this - just make them stop for now
	//obj1->SetVelocity(glm::vec3(0, 0, 0));
	//obj2->SetVelocity(glm::vec3(0, 0, 0));

	// Elasticity, 0 = no energy, 0.7 will a little energy bounce
	// BOUNCE: Formula from Chris Hecker: ease of elasticity
	float e = 0.7f;
	glm::vec3 relativeVelocity = obj2->GetVelocity() - obj1->GetVelocity();
	float jTop = -(1 + e) * glm::dot(relativeVelocity, collisionInfo.normal);
	float jBottom = glm::dot(collisionInfo.normal, collisionInfo.normal) * (1.0f / obj1->GetMass() + 1.0f / obj2->GetMass());
	float j = jTop / jBottom;

	// Collision Response: Hit me
	// Formula: new V = old v - J value / our mass * collision normal
	// Static object doesnt have mass, or infinite mass or zero.
	glm::vec3 obj1NewVelocity = obj1->GetVelocity() - ((j / obj1->GetMass()) * collisionInfo.normal);
	glm::vec3 obj2NewVelocity = obj2->GetVelocity() + ((j / obj2->GetMass()) * collisionInfo.normal);
	// if object static, else 1/mass.
	//* objA / mass  modify the second part of equation above.

	// Assign new Velocity to objects:
	obj1->SetVelocity(obj1NewVelocity);
	obj2->SetVelocity(obj2NewVelocity);
}

void PhysicsCollision::HandleSeparation(PhysicsObject * obj1, PhysicsObject * obj2, CollisionInfo& collisionInfo)
{
	// MATH:
	// 2 Objects = find position after collision by:
	// Take original position, add/minus offset along normal
	// scale intersecpt distance by mass of other object
	// Pa = Pa - x * N
	// Pb = Pb +y * N

	// FIND Values:
	float totalSystemMass = obj1->GetMass() + obj2->GetMass();
	float obj1Offset = collisionInfo.interceptDistance * (obj2->GetMass() / totalSystemMass);
	float obj2Offset = collisionInfo.interceptDistance * (obj1->GetMass() / totalSystemMass);
	// Calculate Offset: Apply formula Pa: Obj1 && Pb: Obj2. if dynamic, set to 0 in static check.
	glm::vec3 obj1NewPosition = obj1->GetPosition() - (obj1Offset * collisionInfo.normal);
	glm::vec3 obj2NewPosition = obj2->GetPosition() - (obj2Offset * collisionInfo.normal);

	obj1->SetPosition(obj1NewPosition);
	obj2->SetPosition(obj2NewPosition);

	//check object static or ! = move amount to RB class
}

bool PhysicsCollision::CheckPlaneSphereCollision(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo& collisionInfo)
{
	bool wasCollision = CheckSpherePlaneCollision(obj2, obj1, collisionInfo);
	if (wasCollision)
	{
		// Flip normal, reverse of SpherePlaneCollision below
		collisionInfo.normal = -collisionInfo.normal;
	}
	return collisionInfo.wasCollision;
}

bool PhysicsCollision::CheckAABBAABBCollision(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	// Object1: AABB v Object2: AABB
	PhysicsAABBShape* pAABB1 = (PhysicsAABBShape*)obj1->GetShape();
	PhysicsAABBShape* pAABB2 = (PhysicsAABBShape*)obj2->GetShape();
	
	// Check X Min/Max position
	//obj1->GetPosition().length.x;
	//obj1->GetPosition().length.y;
	//obj1->GetPosition().length.z;
	//
	//obj2->GetPosition().length.x;
	//obj2->GetPosition().length.y;
	//obj2->GetPosition().length.z;
	// Check X, Y, Z Bounds
	glm::vec3 box1Min = obj1->GetPosition() - pAABB1->GetExtents();
	glm::vec3 box1Max = obj1->GetPosition() + pAABB1->GetExtents();

	glm::vec3 box2Min = obj2->GetPosition() - pAABB2->GetExtents();
	glm::vec3 box2Max = obj2->GetPosition() + pAABB2->GetExtents();

	// Part1: Check Collision
	bool isCollision = (
		box1Min.x < box2Max.x &&
		box1Max.x > box2Min.x &&
		box1Min.y < box2Max.y &&
		box1Max.y > box2Min.y &&
		box1Min.z < box2Max.z &&
		box1Max.z > box2Min.z
		);
	return isCollision;

	// Part2: Using the Bool result
	// If Collision true, find where is it located x,y,z
	if (isCollision = false)
	{
		collisionInfo.wasCollision = false;
		return false;
	}
	else
	{
		collisionInfo.interceptDistance =
				(
				// Check X Min/Max
				(box2Max.x - box1Max.x) < (box1Max.x - box2Min.x)
				);

		collisionInfo.wasCollision = true;
		return true;
	}


}

bool PhysicsCollision::CheckSpherePlaneCollision(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo& collisionInfo)
{
	// Object 1: Sphere
	PhysicsSphereShape* pSphere = (PhysicsSphereShape*)obj1->GetShape();
	// Object 2: Plane
	PhysicsPlaneShape* pPlane = (PhysicsPlaneShape*)obj2->GetShape();

	// Position: Object 1
	// Dot product b/n Sphere and Planes Normal
	float distanceFromPlane = (
		glm::dot(obj1->GetPosition(), pPlane->GetNormal())) - pPlane->GetDistanceFromOrigin();
	if (distanceFromPlane < pSphere->GetRadius())
	{
		// Collision true
		collisionInfo.normal = -pPlane->GetNormal();
		// Radius of sphere minus distance between objects provides intercept distance in units.
		collisionInfo.interceptDistance = pSphere->GetRadius() - distanceFromPlane;
		collisionInfo.wasCollision = true;
		return true;
	}
	else
	{
		// Collision false
		collisionInfo.wasCollision = false;
		return false;
	}
	// Substract planes distance from origin

	return collisionInfo.wasCollision;
}

bool PhysicsCollision::CheckSphereSphereCollision(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	/*
	Check Elastic Collision between two Sphere surfaces:
	Check distance between surface of spheres.
	1. Compare length from centers of the spheres
		a. Distance vector:			vecd = s1.pos - s2.pos
		b. Find length/DISTANCE:	vecd.length = sqrtvecd.x^2 + vecd.y^2 + vecd.z^2
	2. Sum of the radiuses:			SUMRADIUS = s1.radius + s2.radius

	RETURN:
	3. If DISTANCE is < SUM RADIUS, collision true.

	*/
	// Object 1: Sphere
	PhysicsSphereShape* pSphere1 = (PhysicsSphereShape*)obj1->GetShape();
	// Object 2: Sphere
	PhysicsSphereShape* pSphere2 = (PhysicsSphereShape*)obj2->GetShape();

	//DISTANCE
	// 1. Distance Difference vector: vecd = s1.pos - s2.pos
	glm::vec3 vDistance = (obj1->GetPosition() - obj2->GetPosition());
	// 2. Find Length: DISTANCE = vecd.length = sqrtvecd.x^2 + vecd.y^2 + vecd.z^2
	vDistance.x = glm::sqrt(vDistance.x);
	vDistance.y = glm::sqrt(vDistance.y);
	vDistance.z = glm::sqrt(vDistance.z);

	float fLength = vDistance.length();

	// 3. SUM RADIUS: = s1.radius + s2.radius
	float fSumRadius = (pSphere1->GetRadius() - pSphere2->GetRadius());

	// Check Collision: Distance < SumRadius
	if (fLength < fSumRadius)
	{
		// Collision true
		collisionInfo.interceptDistance = fSumRadius;
		collisionInfo.wasCollision = true;
		return true;
	}
	else
	{
		// !Collision
		collisionInfo.wasCollision = false;
		return false;
	}
	return collisionInfo.wasCollision;
}

//bool PhysicsCollision::CheckSphereAABBCollision(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
//{
//	return false;
//}
