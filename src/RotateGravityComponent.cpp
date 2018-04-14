#include "RotateGravityComponent.h"
#include <physics/physics-system.h>
#include <input/input-system.h>
#include <iostream>

using namespace std;
using namespace ecm;
using namespace sf;
using namespace physics;

// switch the coordinates of a vector
inline Vector2f switchVector(const Vector2f& vector) {
    return { vector.y, vector.x };
}

// rotate a vector 90 degrees clockwise
inline b2Vec2 rotateVectorLeft(const b2Vec2& vector) {
    return { vector.y, -vector.x };
}

// rotate a vector 90 degrees counterclockwise
inline b2Vec2 rotateVectorRight(const b2Vec2& vector) {
    return { -vector.y, vector.x };
}

RotateGravityComponent::RotateGravityComponent(ecm::Entity* parent) : Component(parent) {
    playerPhysics = this->parent->getComponent<PlayerPhysicsComponent>();
    rotateLeft = false;
    rotateRight = false;
    registerHandler(Event::KeyPressed, this);
}

RotateGravityComponent::~RotateGravityComponent() {
    unregisterHandler(Event::KeyPressed, this);
}

void RotateGravityComponent::update(double dt) {
    // if the 2 rotations happened at the same time they cancel each other
    if (rotateLeft && rotateRight) {
        rotateLeft = false;
        rotateRight = false;
        return;
    }

    auto gravity = getWorld()->GetGravity();
    auto gravityModified = false;
    
    if(rotateLeft) {
        rotateLeft = false;
        gravityModified = true;
        gravity = rotateVectorLeft(gravity);
        playerPhysics->rotate(90.f);
    }
    if(rotateRight) {
        rotateRight = false;
        gravityModified = true;
        gravity = rotateVectorRight(gravity);
        playerPhysics->rotate(-90.f);
    }

    if(gravityModified) {
        playerPhysics->setMaxVelocity(switchVector(playerPhysics->getMaxVelocity()));
        getWorld()->SetGravity(gravity);
        gravity.Normalize();
        playerPhysics->setDownwards(b2Vec2_To_Vector2f(gravity, false));
        gravity = rotateVectorLeft(gravity);
        playerPhysics->setLeftwards(b2Vec2_To_Vector2f(gravity, false));
    }
}

void RotateGravityComponent::render() {}

void RotateGravityComponent::handleEvent(const sf::Event& event) {
    if (!rotateLeft && event.key.code == input::getKey("GravityLeft"))
        rotateLeft = true;
    else if (!rotateRight && event.key.code == input::getKey("GravityRight"))
        rotateRight = true;
}
