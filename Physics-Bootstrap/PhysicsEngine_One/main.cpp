#include "PhysicsEngine_OneApp.h"

int main() {
	
	auto app = new PhysicsEngine_OneApp();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}