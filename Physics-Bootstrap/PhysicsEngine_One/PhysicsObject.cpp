#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
	: PhysicsObject(1.0f)
{
}

PhysicsObject::PhysicsObject(float mass)
	: m_mass(mass)
	, m_position(0.0f, 0.0f, 0.0f)
	, m_velocity(0.0f, 0.0f, 0.0f)
	, m_force(0.0f,0.0f,0.0f)
	, m_acceleration (0.0f, 0.0f, 0.0f)
	, m_shape(nullptr)
{
}
PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::SetPosition(const glm::vec3& newPosition)
{
	m_position = newPosition;
}

glm::vec3 PhysicsObject::GetPosition() const
{
	return m_position;
}

void PhysicsObject::SetMass(float mass)
{
	m_mass = mass;
}

float PhysicsObject::GetMass() const
{
	return m_mass;
}

void PhysicsObject::SetVelocity(const glm::vec3 & newVelocity)
{
	m_velocity = newVelocity;
}

glm::vec3 PhysicsObject::GetVelocity() const
{
	return m_velocity;
}

void PhysicsObject::AddForce(const glm::vec3 force)
{
	m_force += force;	// accumulate all forces in a frame, in update each force will be used to calucaute acceleration.
	// once acceralteion found, then cal velocity and fro mvelocity we calc position.
}

void PhysicsObject::AddAcceleration(const glm::vec3 acceleration)
{
	m_acceleration += acceleration;
}

void PhysicsObject::AddVelocity(const glm::vec3 velocity)
{
	m_velocity += velocity;
}

void PhysicsObject::Update(float deltaTime)
{
	// Calucate getting things to move.
	// first cal the acceleration
	//glm::vec3 acceleration = m_force / m_mass; / /replaced by below:
	m_acceleration += m_force / m_mass;
		// cal velocity
	m_velocity = m_velocity + m_acceleration * deltaTime;
	m_position = m_position + m_velocity * deltaTime;
	
	//end of each frame, reset force to zero
	m_force = glm::vec3(0.0f, 0.0f, 0.0f);
	m_acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
}

void PhysicsObject::AddShape(PhysicsShape * shape)
{
	m_shape = shape;
}

void PhysicsObject::RemoveShape()
{
	delete m_shape;
	m_shape = nullptr;
}

PhysicsShape * PhysicsObject::GetShape() const
{
	return m_shape;
}
