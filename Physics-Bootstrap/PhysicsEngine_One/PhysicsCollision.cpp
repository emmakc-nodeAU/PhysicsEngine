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
	nullptr, CheckPlaneAABBCollision, CheckPlaneSphereCollision,
	CheckAABBPlaneCollision, CheckAABBAABBCollision, CheckAABBSphereCollision,
	CheckSpherePlaneCollision, CheckSphereAABBCollision, CheckSphereSphereCollision
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

bool PhysicsCollision::CheckAABBSphereCollision(
	const PhysicsObject * obj1, 
	const PhysicsObject * obj2, 
	CollisionInfo & collisionInfo)
{
	// CREATE OBJECTS
	PhysicsAABBShape* pAABB = (PhysicsAABBShape*)obj1->GetShape();
	PhysicsSphereShape* pSphere = (PhysicsSphereShape*)obj2->GetShape();
	
	auto offset = obj2->GetPosition() - obj1->GetPosition();

	auto extents = pAABB->GetExtents();
	glm::vec3 edge = glm::vec3(0);

	edge.x = glm::clamp(glm::dot(offset, glm::vec3(1, 0, 0)), -extents.x, extents.x);
	edge.y = glm::clamp(glm::dot(offset, glm::vec3(0, 1, 0)), -extents.y, extents.y);
	edge.z = glm::clamp(glm::dot(offset, glm::vec3(0, 0, 1)), -extents.z, extents.z);

	auto aabbClosestPoint = obj1->GetPosition() + edge;

	offset = obj2->GetPosition() - aabbClosestPoint;
	float length = glm::length(offset);

	if (length < pSphere->GetRadius() && length > 0)
	{
		collisionInfo.normal = glm::normalize(offset);
		collisionInfo.interceptDistance = pSphere->GetRadius() - length;
		collisionInfo.wasCollision = true;
	}
	else
	{
		collisionInfo.wasCollision = false;
	}

	return collisionInfo.wasCollision;

	//// CHECK POSITION: AABB
	//glm::vec3 box1Min = obj2->GetPosition() - pAABB->GetExtents();
	//glm::vec3 box1Max = obj2->GetPosition() + pAABB->GetExtents();
	//// CHECK POSITION: Sphere
	//obj1->GetPosition();
	//
	//bool wasCollision = CheckAABBSphereCollision(obj2, obj1, collisionInfo);
	//if (wasCollision)
	//{
	//	collisionInfo.normal = -collisionInfo.normal;
	//}
	//return collisionInfo.wasCollision;
}

bool PhysicsCollision::CheckSphereAABBCollision(
	const PhysicsObject * obj1, 
	const PhysicsObject * obj2, 
	CollisionInfo & collisionInfo)
{
	bool wasCollision = CheckAABBSphereCollision(obj2, obj1, collisionInfo);
	if (wasCollision)
	{
		collisionInfo.normal = -collisionInfo.normal;
	}
	return collisionInfo.wasCollision;

	//// CREATE OBJECTS
	//PhysicsSphereShape* pSphere = (PhysicsSphereShape*)obj1->GetShape();
	//PhysicsAABBShape* pAABB = (PhysicsAABBShape*)obj2->GetShape();
	//
	//// CHECK POSITION: Sphere
	//obj1->GetPosition();
	//pSphere->GetRadius();
	//
	//// CHECK POSITION: AABB
	//glm::vec3 box1Min = obj2->GetPosition() - pAABB->GetExtents();
	//glm::vec3 box1Max = obj2->GetPosition() + pAABB->GetExtents();
	//
	//// Formula: Dot product b/n Sphere and Extents
	//float distanceFromSphereToAABB = (
	//	(glm::dot(obj1->GetPosition().x, box1Min.x)) <= obj1->GetPosition().x &&
	//	(glm::dot(obj1->GetPosition().x, box1Max.x)) <= obj1->GetPosition().x &&
	//	(glm::dot(obj1->GetPosition().y, box1Min.y)) <= obj1->GetPosition().y &&
	//	(glm::dot(obj1->GetPosition().y, box1Max.y)) <= obj1->GetPosition().y &&
	//	(glm::dot(obj1->GetPosition().z, box1Min.z)) <= obj1->GetPosition().z &&
	//	(glm::dot(obj1->GetPosition().z, box1Max.z)) <= obj1->GetPosition().z
	//	);
	//
	//bool wasCollision = CheckSphereAABBCollision(obj2, obj1, collisionInfo);
	//if (wasCollision)
	//{
	//	//
	//}
	//return collisionInfo.wasCollision;
}

bool PhysicsCollision::CheckAABBPlaneCollision(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	PhysicsAABBShape* pAABB = (PhysicsAABBShape*)obj1->GetShape();
	PhysicsPlaneShape* pPlane = (PhysicsPlaneShape*)obj2->GetShape();

	//calculate all AABB cube points
	glm::vec3 point1 = obj1->GetPosition() + glm::vec3(-pAABB->GetExtents().x, -pAABB->GetExtents().y, -pAABB->GetExtents().z);
	glm::vec3 point2 = obj1->GetPosition() + glm::vec3(-pAABB->GetExtents().x, -pAABB->GetExtents().y, pAABB->GetExtents().z);
	glm::vec3 point3 = obj1->GetPosition() + glm::vec3(-pAABB->GetExtents().x, pAABB->GetExtents().y, -pAABB->GetExtents().z);
	glm::vec3 point4 = obj1->GetPosition() + glm::vec3(-pAABB->GetExtents().x, pAABB->GetExtents().y, pAABB->GetExtents().z);

	glm::vec3 point5 = obj1->GetPosition() - glm::vec3(pAABB->GetExtents().x, pAABB->GetExtents().y, pAABB->GetExtents().z);
	glm::vec3 point6 = obj1->GetPosition() - glm::vec3(pAABB->GetExtents().x, pAABB->GetExtents().y, -pAABB->GetExtents().z);
	glm::vec3 point7 = obj1->GetPosition() - glm::vec3(pAABB->GetExtents().x, -pAABB->GetExtents().y, pAABB->GetExtents().z);
	glm::vec3 point8 = obj1->GetPosition() - glm::vec3(pAABB->GetExtents().x, -pAABB->GetExtents().y, -pAABB->GetExtents().z);

	//float distanceFromPlane = (glm::dot(obj1->GetPosition(), pPlane->getNormal())) - pPlane->getDistance();

	//Find the distance from each point to the plane
	float distanceFromPlane1 = (glm::dot(point1, pPlane->GetNormal())) - pPlane->GetDistanceFromOrigin();//GetDistance();
	float distanceFromPlane2 = (glm::dot(point2, pPlane->GetNormal())) - pPlane->GetDistanceFromOrigin();
	float distanceFromPlane3 = (glm::dot(point3, pPlane->GetNormal())) - pPlane->GetDistanceFromOrigin();
	float distanceFromPlane4 = (glm::dot(point4, pPlane->GetNormal())) - pPlane->GetDistanceFromOrigin();

	float distanceFromPlane5 = (glm::dot(point5, pPlane->GetNormal())) - pPlane->GetDistanceFromOrigin();
	float distanceFromPlane6 = (glm::dot(point6, pPlane->GetNormal())) - pPlane->GetDistanceFromOrigin();
	float distanceFromPlane7 = (glm::dot(point7, pPlane->GetNormal())) - pPlane->GetDistanceFromOrigin();
	float distanceFromPlane8 = (glm::dot(point8, pPlane->GetNormal())) - pPlane->GetDistanceFromOrigin();

	//check to see which point is the closest to the plane
	float fSmallest = 10000000000;
	if (distanceFromPlane1 < fSmallest) fSmallest = distanceFromPlane1;
	if (distanceFromPlane2 < fSmallest) fSmallest = distanceFromPlane2;
	if (distanceFromPlane3 < fSmallest) fSmallest = distanceFromPlane3;
	if (distanceFromPlane4 < fSmallest) fSmallest = distanceFromPlane4;
	if (distanceFromPlane5 < fSmallest) fSmallest = distanceFromPlane5;
	if (distanceFromPlane6 < fSmallest) fSmallest = distanceFromPlane6;
	if (distanceFromPlane7 < fSmallest) fSmallest = distanceFromPlane7;
	if (distanceFromPlane8 < fSmallest) fSmallest = distanceFromPlane8;

	//get the smallest intersecting distance and point it in the direction to reflect collision 
	if (fSmallest < 0)
	{
		collisionInfo.normal = -pPlane->GetNormal();
		collisionInfo.interceptDistance = -fSmallest;
		collisionInfo.wasCollision = true;
		/*collisionInfo.interceptDistance = pAABB->GetDims().x - distanceFromPlane;
		collisionInfo.interceptDistance = pAABB->GetDims().y - distanceFromPlane;
		collisionInfo.interceptDistance = pAABB->GetDims().z - distanceFromPlane; */
	}
	else
	{
		collisionInfo.wasCollision = false;
	}

	return collisionInfo.wasCollision;
	
	//// CREATE OBJECTS
	//PhysicsAABBShape* pAABB = (PhysicsAABBShape*)obj1->GetShape();
	//PhysicsPlaneShape* pPlane = (PhysicsPlaneShape*)obj2->GetShape();
	//
	//// CHECK POSITION: AABB X, Y, Z Position/Dimensions
	//glm::vec3 box1Min = obj1->GetPosition() - pAABB->GetExtents();
	//glm::vec3 box1Max = obj1->GetPosition() + pAABB->GetExtents();
	//
	//// CHECK POSITION: Plane normal, distance
	//glm::vec3 pDistance = obj2->GetPosition();
	//
	//if (obj2->GetPosition().x > box1Min.x && obj1->GetPosition().x < box1Max.x &&
	//	obj2->GetPosition().y > box1Min.y && obj1->GetPosition().y < box1Max.y &&
	//	obj2->GetPosition().z > box1Min.z && obj1->GetPosition().z < box1Max.z)
	//{
	//	collisionInfo.normal = pPlane->GetNormal();
	//	collisionInfo.interceptDistance = pAABB->GetExtents().x - pPlane->GetNormal().x;
	//	collisionInfo.interceptDistance = pAABB->GetExtents().y - pPlane->GetNormal().y;
	//	collisionInfo.interceptDistance = pAABB->GetExtents().z - pPlane->GetNormal().z;
	//	collisionInfo.wasCollision = true;
	//	return true;
	//}
	//else
	//{
	//	collisionInfo.wasCollision = false;
	//	return false;
	//}
}


bool PhysicsCollision::CheckPlaneAABBCollision(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	bool wasCollision = CheckAABBPlaneCollision(obj2, obj1, collisionInfo);
	if (wasCollision)
	{
		collisionInfo.normal = -collisionInfo.normal;
	}
	return collisionInfo.wasCollision;
}

void PhysicsCollision::ResolveCollision(PhysicsObject * obj1, PhysicsObject * obj2, CollisionInfo& collisionInfo)
{
	if (collisionInfo.wasCollision)
	{
		// Resolve Position and Velocity of GameObject primitives.
		HandleSeparation(obj1, obj2, collisionInfo);
		HandleVelocityChange(obj1, obj2, collisionInfo);
	}
}

void PhysicsCollision::HandleVelocityChange(
	PhysicsObject * obj1, 
	PhysicsObject * obj2, 
	CollisionInfo& collisionInfo)
{
	/*
	--- CALCULATE VELOCITY RESPONSE ---
	Theory: Conservation of Energy
	Total Energy in a System = The same before and after collision
	Elastic Kinetic Collisions = Kinetic energy before collision is equal to kinetic energy after collision
	Friction above = zero
	
	Formula elements:	Elastic Response
	1. Normalise velocities vectors
	2. Dot Product of Normalized vectors: (||v1||.||v2|| = d)
	3. Multiply v1,v2 by dot product (v1*d = v3, v2*d = v4)
	4. Subtract v3, v4 from v1,v2 (v1-v3 = v5, v2 - v4 = v6)
	5. S1 velocity = v5 + v4
	6. S1 velocity = v6 + v3

	Sphere (S1) v Sphere (S2) Collision
	Mass center = center of sphere
	Collision type= Elastic .: no kinetic energy lost at collision
	Formula: V1 & V2 Sphere Velocities: See slide 7/29 Collision Response and Friction

	Sphere (S1) v Plane Collision (N)
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

void PhysicsCollision::HandleSeparation(
	PhysicsObject * obj1, 
	PhysicsObject * obj2, 
	CollisionInfo& collisionInfo)
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

bool PhysicsCollision::CheckAABBAABBCollision(
	const PhysicsObject * obj1, 
	const PhysicsObject * obj2, 
	CollisionInfo & collisionInfo)
{
	// CREATE OBJECTS:
	PhysicsAABBShape* pAABB1 = (PhysicsAABBShape*)obj1->GetShape();
	PhysicsAABBShape* pAABB2 = (PhysicsAABBShape*)obj2->GetShape();
	
	glm::vec3 max1 = obj1->GetPosition() + pAABB1->GetExtents();
	glm::vec3 min1 = obj1->GetPosition() - pAABB1->GetExtents();

	glm::vec3 max2 = obj2->GetPosition() + pAABB2->GetExtents();
	glm::vec3 min2 = obj2->GetPosition() - pAABB2->GetExtents();


	if (max1.x > min2.x && min1.x < max2.x && max1.y > min2.y && min1.y < max2.y &&
		max1.z > min2.z && min1.z < max2.z)
	{
		float x1 = max2.x - min1.x;
		float x2 = min2.x - max1.x;
		float y1 = max2.y - min1.y;
		float y2 = min2.y - max1.y;
		float z1 = max2.z - min1.z;
		float z2 = min2.z - max1.z;


		float fSmallestX = (abs(x1) < abs(x2)) ? x1 : x2;
		float fSmallestY = (abs(y1) < abs(y2)) ? y1 : y2;
		float fSmallestZ = (abs(z1) < abs(z2)) ? z1 : z2;


		float realSmallest = fSmallestX;
		glm::vec3 normal = glm::vec3(1, 0, 0);
		if (abs(fSmallestY) < abs(realSmallest))
		{
			realSmallest = fSmallestY;
			normal = glm::vec3(0, 1, 0);
		}
		if (abs(fSmallestZ) < abs(realSmallest))
		{
			realSmallest = fSmallestZ;
			normal = glm::vec3(0, 0, 1);
		}

		//do collision 
		collisionInfo.normal = normal;
		collisionInfo.interceptDistance = -realSmallest;
		collisionInfo.wasCollision = true;
	}
	else
	{
		collisionInfo.wasCollision = false;
	}
	return collisionInfo.wasCollision;
	

	//
	//// CHECK POSITION X, Y, Z Bounds
	//glm::vec3 box1Min = obj1->GetPosition() - pAABB1->GetExtents();
	//glm::vec3 box1Max = obj1->GetPosition() + pAABB1->GetExtents();
	//
	//glm::vec3 box2Min = obj2->GetPosition() - pAABB2->GetExtents();
	//glm::vec3 box2Max = obj2->GetPosition() + pAABB2->GetExtents();
	//
	//// Part1: Check Collision
	//// Is Box1 Max > Box2 Min and Box1 min < Box2 Max
	//bool isCollision = (
	//	box1Min.x < box2Max.x &&
	//	box1Max.x > box2Min.x &&
	//	box1Min.y < box2Max.y &&
	//	box1Max.y > box2Min.y &&
	//	box1Min.z < box2Max.z &&
	//	box1Max.z > box2Min.z
	//	);
	//return isCollision;
	//
	//// Part2: Using the Bool result
	//// If Collision true, find collision location on x,y,z & Store value?
	//if (isCollision = false)
	//{
	//	collisionInfo.wasCollision = false;
	//	return false;
	//}
	//else
	//{
	//	collisionInfo.interceptDistance =
	//			(
	//			// Check X/Y/Z Min/Max
	//				((box2Max.x - box2Min.x) < (box1Max.x - box1Min.x)),
	//				((box2Max.y - box2Min.y) < (box1Max.y - box1Min.y)),
	//				((box2Max.z - box2Min.z) < (box1Max.z - box1Min.z))
	//			);
	//
	//	collisionInfo.wasCollision = true;
	//	return true;
	//}
}

bool PhysicsCollision::CheckSpherePlaneCollision(
	const PhysicsObject * obj1, 
	const PhysicsObject * obj2, 
	CollisionInfo& collisionInfo)
{
	// CREATE: OBJECTS
	PhysicsSphereShape* pSphere = (PhysicsSphereShape*)obj1->GetShape();
	PhysicsPlaneShape* pPlane = (PhysicsPlaneShape*)obj2->GetShape();

	// CHECK POSITION: Formula: Dot product b/n Sphere and Plane Normal
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

bool PhysicsCollision::CheckSphereSphereCollision(
	const PhysicsObject * obj1, 
	const PhysicsObject * obj2, 
	CollisionInfo & collisionInfo)
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

	Formula elements: WasCollision: SPHERE v SPHERE
	Distance (D) is magnitude/length/size of the difference in center points (c)
	1. D = | c1 - c2 |

	Collision if distance < sum of radii (r)
	2. bCollision = D < SumOf r

	*/
	// CREATE OBJECTS
	PhysicsSphereShape* pSphere1 = (PhysicsSphereShape*)obj1->GetShape();
	PhysicsSphereShape* pSphere2 = (PhysicsSphereShape*)obj2->GetShape();

	glm::vec3 offset = obj2->GetPosition() - obj1->GetPosition();
	float d = glm::length(offset);

	float min = pSphere1->GetRadius() + pSphere2->GetRadius();

	if (d < min)
	{
		//then collision 
		collisionInfo.normal = glm::normalize(offset);
		collisionInfo.interceptDistance = min - d;

		collisionInfo.wasCollision = true;
	}

	else
	{
		collisionInfo.wasCollision = false;
	}

	return collisionInfo.wasCollision;

	//// FIND DISTANCE
	//// 1. Distance Difference vector: vecd = s1.pos - s2.pos
	//glm::vec3 vDistance = (obj1->GetPosition() - obj2->GetPosition());
	//// 2. Find Length: DISTANCE = vecd.length = sqrtvecd.x^2 + vecd.y^2 + vecd.z^2
	//vDistance.x = glm::sqrt(vDistance.x);
	//vDistance.y = glm::sqrt(vDistance.y);
	//vDistance.z = glm::sqrt(vDistance.z);
	//
	//float fLength = vDistance.length();
	//
	//// 3. SUM RADIUS: = s1.radius + s2.radius
	//float fSumRadius = (pSphere1->GetRadius() - pSphere2->GetRadius());
	//
	//// Check Collision: Distance < SumRadius
	//if (fLength < fSumRadius)
	//{
	//	// Collision true
	//	collisionInfo.interceptDistance = fSumRadius;
	//	collisionInfo.wasCollision = true;
	//	return true;
	//}
	//else
	//{
	//	// !Collision
	//	collisionInfo.wasCollision = false;
	//	return false;
	//}
	//
	//// Set C1, C2 values:
	//glm::vec3 s1CenterPoint;
	//glm::vec3 s2CenterPoint;
	//// Get C1, C2 values:
	//s1CenterPoint = (obj1->GetPosition());
	//s2CenterPoint = (obj2->GetPosition());
	//// 1. Find objects Distance: C1 - C2
	//glm::vec3 displacement = ((s1CenterPoint) - (s2CenterPoint));
	//// 2. bCollision = D < SumOf r
	//	if (displacement.length < pSphere1->GetRadius() + pSphere2->GetRadius())
	//	{
	//		collisionInfo.wasCollision = true;
	//	}
	//	else
	//	{
	//		collisionInfo.wasCollision = false;
	//	}
	//return collisionInfo.wasCollision;
}

