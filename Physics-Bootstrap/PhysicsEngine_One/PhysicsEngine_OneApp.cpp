#include "PhysicsEngine_OneApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "PhysicsObject.h"
#include "PhysicsSphereShape.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

PhysicsEngine_OneApp::PhysicsEngine_OneApp() {
	PhysicsObject crate(100.0f);
	crate.AddForce(glm::vec3(1000, 0, 0));
	crate.Update(1.0f);
	crate.AddForce(glm::vec3(0, 1000, 0));
}

PhysicsEngine_OneApp::~PhysicsEngine_OneApp() {

}

bool PhysicsEngine_OneApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(0,0,100), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	m_physicsScene = new PhysicsScene();
	// Apply Gravity
	m_physicsScene->SetGravity(glm::vec3(0.0f, -9.8f, 0.0f));	// horizontal force
	// Create Sphere
	m_demoGameObject = new GameObject();
	// Passes in Object and mass
	PhysicsObject* demoPhysicsObject = new PhysicsObject(5.0f);

	// New shape: now include physicsSphereShape: size
	demoPhysicsObject->AddShape(new PhysicsSphereShape(0.5f));	// Sets size, not creating an actual object.
	// Sphere
	m_physicsScene->Add(demoPhysicsObject);
	m_demoGameObject->SetPhysicsObject(demoPhysicsObject);
	m_demoGameObject->GetPhysicsObject()->SetVelocity(glm::vec3(10.0f, 25.0f, 0.0f));	// Parobolic Arc: Fire ball with projection arc added.

	return true;
}

void PhysicsEngine_OneApp::shutdown() {

	delete m_demoGameObject;
	delete m_physicsScene;
	Gizmos::destroy();
}

void PhysicsEngine_OneApp::update(float deltaTime) {



	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// ADD FORCE TO RED SPHERE
	//m_demoGameObject->GetPhysicsObject()->AddForce(glm::vec3(1, 0, 0));

	m_physicsScene->Update(deltaTime);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsEngine_OneApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);


	//REDSPHERE
	m_demoGameObject->DebugPhysicsRender();

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}