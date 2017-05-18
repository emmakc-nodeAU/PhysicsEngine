#include "PhysicsEngine_OneApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "PhysicsScene.h"

#include "GameObject.h"

#include "PhysicsObject.h"
#include "PhysicsSphereShape.h"
#include "PhysicsPlaneShape.h"
#include "PhysicsAABBShape.h"

#include "PhysicsCollision.h"

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

	// CAMERA: Simple transforms
	m_viewMatrix = glm::lookAt(vec3(0,0,20), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	/*
	--- PHYSICS ENGINE ---
	---  Correct Order ---
	1. CREATE: Scene
		A. APPLY: Forces to Scene: Gravity
	2. CREATE: New GameObject: Sphere, Plane, demoGameObject
	3. CREATE: New PhysicsObject
		A. SET PhysicsObject to GameObject: Size, Mass, Position, Velocity
			i.	 Set SetPosition:
			ii.  Set Object bounds: AddShape:(Sphere, Plane, AABB)
			iii. Set Velocity: eg. arcs
		B. SET Static: Moving True/False
	4. ADD: GameObjects to Scene: m_ball, m_floor, m_demoGameObject

	*/

	// 1. SCENE:
	m_physicsScene = new PhysicsScene();

	// 1.A: GLOBAL SCENE FORCES: Gravity
	m_physicsScene->SetGravity(glm::vec3(0.0f, -9.8f, 0.0f));	// horizontal force

	// 2. PLANE: FLOOR
	m_floor = new GameObject();
	m_floor->SetPhysicsObject(new PhysicsObject(std::numeric_limits<float>().max())); // Mass set to max
	m_floor->GetPhysicsObject()->AddShape(new PhysicsPlaneShape(glm::vec3(0, 1, 0), 0));
	m_floor->GetPhysicsObject()->SetIsStatic(true);	// Floor will not move.

	// 2. SPHERE: BALL
	m_ball = new GameObject();
	m_ball->SetPhysicsObject(new PhysicsObject(2.0f));
	m_ball->GetPhysicsObject()->SetPosition(glm::vec3(0, 5, 0));
	m_ball->GetPhysicsObject()->AddShape(new PhysicsSphereShape(1.0f));
	//m_ball->GetPhysicsObject()->SetVelocity(glm::vec3(10.0f, 25.0f, 0.0f)); // Parobolic Arc: Fire ball with projection arc.
	m_ball->GetPhysicsObject()->SetIsStatic(false);

	// 2. demoGameObject: Plane
	// CREATE: Shape: Create Sphere	
	//m_demoGameObject = new GameObject();
	//PhysicsObject* demoPhysicsObject = new PhysicsObject(5.0f);
	//demoPhysicsObject->AddShape(new PhysicsPlaneShape(glm::vec3(1,0,0) , -30.0f));
	//m_demoGameObject->SetPhysicsObject(demoPhysicsObject);

	// 4. ADD GameObjects to the Scene
	//m_physicsScene->Add(demoPhysicsObject);
	m_physicsScene->Add(m_floor->GetPhysicsObject());
	m_physicsScene->Add(m_ball->GetPhysicsObject());

	return true;
}

void PhysicsEngine_OneApp::shutdown() {

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

	// ADD Physics Collision Check:
	PhysicsCollision::CollisionInfo collisionInfo;
	bool wasCollision = PhysicsCollision::CheckCollision(m_floor->GetPhysicsObject(), m_ball->GetPhysicsObject(), collisionInfo);
	if (wasCollision)
	{
		int i = 3;
	}
	
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

	// DRAW: GameObjects
	//m_demoGameObject->DebugPhysicsRender();
	m_ball->DebugPhysicsRender();
	m_floor->DebugPhysicsRender();

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}