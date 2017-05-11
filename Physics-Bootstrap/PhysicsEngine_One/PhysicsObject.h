#pragma once
#include <glm/glm.hpp>
#include "PhysicsShape.h"

class PhysicsObject
{
	// What do physics objects have in common:
	// Vec3 Position | Mass | vec3 Velocity
	//
public:
	PhysicsObject();
	// Pass in default mass
	PhysicsObject(float mass);
	~PhysicsObject();

	void SetPosition(const glm::vec3& newPosition);
	glm::vec3 GetPosition() const;

	void SetMass(float mass);
	float GetMass() const;
	
	void SetVelocity(const glm::vec3& newVelocity);
	glm::vec3 GetVelocity() const;


	void AddForce(const glm::vec3 force);
	void AddAcceleration(const glm::vec3 acceleration);
	void AddVelocity(const glm::vec3 velocity);
	void Update(float deltaTime);

	void AddShape(PhysicsShape* shape);
	void RemoveShape();	// once more than one shape, pass in shape
	
	PhysicsShape* GetShape() const;


private:
	glm::vec3 m_position;
	float	  m_mass;
	glm::vec3 m_velocity;
	glm::vec3 m_force;	// apply every frame, to continuely affect object.
	glm::vec3 m_acceleration;

	PhysicsShape* m_shape;
};
