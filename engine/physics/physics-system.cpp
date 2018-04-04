#include "physics-system.h"

using namespace std;
using namespace sf;
using namespace physics;

static shared_ptr<b2World> world;
const int32 velocityIterations = 8;
const int32 positionIterations = 3;

void physics::initialise(const b2Vec2 gravity) {
	world.reset(new b2World(gravity));
}

void physics::shutdown() {
	world.reset();
}

void physics::update(const double& dt) {
	world->Step(float32(dt), velocityIterations, positionIterations);
}

shared_ptr<b2World> physics::getWorld() {
	return world;
}
