#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "PhysicsObject.h"
#include "PhysicsScene.h"
#include "GameObject.h"

//in application 3d
//PhysicsObject rocket(100.0f);
//crate.AddForce(glm::vec3(1000.0f, 0, 0));
//crate.Update(1.0f);
//// run above with break point. Q what is the velocity. 10m/p/sec/sec // tutorial qq

class PhysicsEngine_OneApp : public aie::Application {
public:

	PhysicsEngine_OneApp();
	virtual ~PhysicsEngine_OneApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	PhysicsScene* m_physicsScene;
	GameObject* m_demoGameObject;
	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;
};