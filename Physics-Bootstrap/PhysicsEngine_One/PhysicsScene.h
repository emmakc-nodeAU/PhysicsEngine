#pragma once
#include <set>
#include <glm/glm.hpp>

class PhysicsObject;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void Add(PhysicsObject* newObject);
	void Remove(PhysicsObject* objectToRemove);

	void Update(float deltaTime);	

	void SetGravity(const glm::vec3& gravity);
	glm::vec3 GetGravity() const;

private:
	std::set<PhysicsObject*> m_physicsObjects;
	glm::vec3 m_gravity;
};