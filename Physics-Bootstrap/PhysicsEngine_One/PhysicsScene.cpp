#include "PhysicsScene.h"
#include "PhysicsObject.h"

// Physics scene:
// Container to hold a type of object

PhysicsScene::PhysicsScene()
	: m_gravity(glm::vec3(0,0,0))
{
}

PhysicsScene::~PhysicsScene()
{
	// Delete any physics objects
	for (PhysicsObject* object : m_physicsObjects)
	{
		delete object;
	}
}

void PhysicsScene::Add(PhysicsObject * newObject)
{
	m_physicsObjects.insert(newObject);
}

void PhysicsScene::Remove(PhysicsObject * objectToRemove)
{
	//Search for the object in the set
	auto it = m_physicsObjects.find(objectToRemove);
	if (it != m_physicsObjects.end())
	{
		// Delete object iterater pointing to
		delete *it;
		// call arrays function on set, then remove from list
		m_physicsObjects.erase(it);
	}
}

void PhysicsScene::Update(float deltaTime)
{
	for (PhysicsObject* object : m_physicsObjects)
	{
		//object->AddForce();	//AddForce.ForceType
		object->AddAcceleration(m_gravity);
		object->Update(deltaTime); // not yet written
	}
}

void PhysicsScene::SetGravity(const glm::vec3 & gravity)
{
	m_gravity = gravity;
}

glm::vec3 PhysicsScene::GetGravity() const
{
	return m_gravity;
}
