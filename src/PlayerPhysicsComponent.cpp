#include "PlayerPhysicsComponent.h"
#include <input/input-system.h>
#include <iostream>

using namespace std;
using namespace sf;
using namespace physics;
using namespace ecm;

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* parent, const Vector2f& size) : PhysicsComponent(parent, true, size) {
    this->size = Vector2f_To_b2Vec2(size);
    maxVelocity = { 200.f, 400.f };
    groundSpeed = 30.f;
    grounded = false;
    jump = false;
    body->SetSleepingAllowed(false);
    body->SetFixedRotation(true);
    body->SetBullet(true);
    setRestitution(0.f);
    event::registerHandler(Event::KeyPressed, this);
}

PlayerPhysicsComponent::~PlayerPhysicsComponent() {
    event::unregisterHandler(Event::KeyPressed, this);
}

bool PlayerPhysicsComponent::isGrounded() const {
    const auto& positionY = body->GetPosition().y;
    const auto halfHeight = size.y * .5f;
    b2WorldManifold manifold;
    // iterate over all the contacts
    for(const auto& contact : getTouching()) {
        contact->GetWorldManifold(&manifold);
        const auto numPoints = contact->GetManifold()->pointCount;
        auto contactIsBelow = numPoints > 0;
        // determine if all the points are below the player
        for (auto i = 0; i < numPoints; i++)
            contactIsBelow &= manifold.points[i].y < positionY - halfHeight;
        if (contactIsBelow)
            return true;
    }
    return false;
}

void PlayerPhysicsComponent::update(double dt) {
    auto velocity = getVelocity();
    cout << (grounded ? "grounded" : "not grounded") << endl;

    // move left or right
    if (input::isControlPressed("Left")) {
        if (velocity.x > -maxVelocity.x)
            impulse({ float(dt) * -groundSpeed, 0.f });
    }
    else if (input::isControlPressed("Right")) {
        if (velocity.x < maxVelocity.x)
            impulse({ float(dt) * groundSpeed, 0.f });
    }
    else {
        dampen({ .9f, 1.f });
    }

    // jump
    if(jump) {
        jump = false;
        grounded = isGrounded();
        if(grounded) {
            setVelocity({ velocity.x, 0.f });
            impulse({ 0.f, -6.f });
        }
    }
    
    // update grounded state
    if (!grounded) {
        grounded = isGrounded();
        setFriction(0.f);
    }
    else
        setFriction(.1f);

    // restrain velocity to max velocity or below
    velocity = getVelocity();
    velocity.x = copysign(min(abs(velocity.x), maxVelocity.x), velocity.x);
    velocity.y = copysign(min(abs(velocity.y), maxVelocity.y), velocity.y);
    setVelocity(velocity);

    PhysicsComponent::update(dt);
}

void PlayerPhysicsComponent::handleEvent(const Event& event) {
    if (event.key.code == input::getKey("Jump"))
        jump = true;
}

float PlayerPhysicsComponent::getGroundSpeed() const {
    return groundSpeed;
}

void PlayerPhysicsComponent::setGroundSpeed(const float groudSpeed) {
    this->groundSpeed = groudSpeed;
}

const Vector2f& PlayerPhysicsComponent::getMaxVelocity() const {
    return maxVelocity;
}

void PlayerPhysicsComponent::setMaxVelocity(const Vector2f maxVelocity) {
    this->maxVelocity = maxVelocity;
}
