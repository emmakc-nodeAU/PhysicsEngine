#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "PhysicsCollision.h"

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

//PhysicsCollision::CollisionInfo collisionInfo;

void PhysicsScene::Update(float deltaTime)
{
	for (PhysicsObject* object : m_physicsObjects)
	{
		// Static check
		if (!object->GetIsStatic())
		{
		object->AddAcceleration(m_gravity);
		object->Update(deltaTime);
		}
	}
	PhysicsCollision::CollisionInfo collisionInfo;

	for (auto objInteratorOne = m_physicsObjects.begin();
		objInteratorOne != m_physicsObjects.end();
		objInteratorOne++)
	{
		for (auto objInteratorTwo = std::next(objInteratorOne);
			objInteratorTwo != m_physicsObjects.end();
			objInteratorTwo++)
		{
			bool wasCollision = PhysicsCollision::CheckCollision(*objInteratorOne, *objInteratorTwo, collisionInfo);
			if (wasCollision)
			{
				PhysicsCollision::ResolveCollision(*objInteratorOne, *objInteratorTwo, collisionInfo);
			}
		}
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
