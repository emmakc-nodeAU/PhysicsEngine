#pragma once

class PhysicsObject;
class GameObject
{
public:
	GameObject();
	~GameObject() {};

	virtual void Update(float deltaTime) {};

	virtual void Render() {};

	void DebugPhysicsRender();
	void SetPhysicsObject(PhysicsObject* physicsObject);
	PhysicsObject* GetPhysicsObject() const;
private:
	PhysicsObject* m_physicsObject;
};