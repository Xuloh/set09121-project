#include "PlayerPhysicsComponent.h"
#include "SpritesheetAnimatorComponent.h"
#include <input/input-system.h>
#include <iostream>

using namespace std;
using namespace sf;
using namespace physics;
using namespace ecm;

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* parent, const Vector2f& size) : PhysicsComponent(parent, true, size) {
	this->size = Vector2f_To_b2Vec2(size);
	maxVelocity = { 200.f, 400.f };
	downwards = { 0.f, -1.f };
	leftwards = { -1.f, 0.f };
	groundSpeed = 30.f;
	grounded = false;
	jump = false;
	body->SetSleepingAllowed(false);
	body->SetFixedRotation(true);
	body->SetBullet(true);
	setRestitution(0.f);
	event::registerHandler(Event::KeyPressed, this);
	event::registerHandler(Event::JoystickButtonPressed, this);
}

PlayerPhysicsComponent::~PlayerPhysicsComponent() {
	event::unregisterHandler(Event::KeyPressed, this);
	event::unregisterHandler(Event::JoystickButtonPressed, this);
}

bool PlayerPhysicsComponent::isGrounded() const {
	const auto& positionY = body->GetPosition().y;
	const auto halfHeight = size.y * .5f;
	b2WorldManifold manifold;
	// iterate over all the contacts
	for (const auto& contact : getTouching()) {
		contact->GetWorldManifold(&manifold);
		const auto numPoints = contact->GetManifold()->pointCount;
		auto contactIsBelow = numPoints > 0;
		// determine if all the points are below the player
		for (auto i = 0; i < numPoints; i++)
			contactIsBelow &= isBelow(manifold.points[i]);
		if (contactIsBelow)
			return true;
	}
	return false;
}

bool PlayerPhysicsComponent::isBelow(const b2Vec2 point) const {
	const auto& position = body->GetPosition();
	return
		(downwards.x == 0 && // vertical gravity
			downwards.y * point.y > downwards.y * (position.y + downwards.y * size.y * .5f))
		||
		(downwards.y == 0 && // horizontal gravity
			downwards.x * point.x > downwards.x * (position.x + downwards.y * size.x * .5f));
}


void PlayerPhysicsComponent::update(double dt) {
	auto velocity = getVelocity();

    // update grounded state
    grounded = isGrounded();
    if (!grounded)
        setFriction(0.f);
    else
        setFriction(.1f);

	// move left
	if (input::isControlPressed("Left")) {
		if (velocity.x > -maxVelocity.x)
			impulse(float(dt) * groundSpeed * leftwards);
	}
    // move right
	else if (input::isControlPressed("Right")) {
		if (velocity.x < maxVelocity.x)
			impulse(float(dt) * -groundSpeed * leftwards);
	}
    // idle
	else {
		if (downwards.x == 0)
			dampen({ .9f, 1.f });
		if (downwards.y == 0)
			dampen({ 1.f, .9f });
	}
	// jump
	if (jump) {
		jump = false;
		if (grounded) {
			// keep velocity depending on the downwards direction
			setVelocity({ !downwards.x * velocity.x, !downwards.y * velocity.y });
			impulse(-6.f * downwards);
		}
	}

    // update the player animation based on the physics state
    auto animator = parent->getComponent<SpritesheetAnimatorComponent>();
    // jump animation
    if (!grounded) {
        animator->setKeyFrame(15);
        animator->setEndFrame(22);
    }
    // move left animation
    else if (input::isControlPressed("Left")) {
        animator->setKeyFrame(23);
        animator->setEndFrame(28);
    }
    // move right animation
    else if (input::isControlPressed("Right")) {
        animator->setKeyFrame(29);
        animator->setEndFrame(34);
    }
    // idle animation
    else {
        animator->setKeyFrame(0);
        animator->setEndFrame(14);
    }

	// restrain velocity to max velocity or below
	velocity = getVelocity();
	velocity.x = copysign(min(abs(velocity.x), maxVelocity.x), velocity.x);
	velocity.y = copysign(min(abs(velocity.y), maxVelocity.y), velocity.y);
	setVelocity(velocity);

	PhysicsComponent::update(dt);
}

void PlayerPhysicsComponent::handleEvent(const Event& event) {
	if (!input::usingController() && event.type == Event::KeyPressed) {
		if (event.key.code == input::getKey("Jump"))
			jump = true;
	}
	else if (input::usingController() && event.type == Event::JoystickButtonPressed) {
		if (event.joystickButton.joystickId == 0 && event.joystickButton.button == input::getJoystickButton("Jump"))
			jump = true;
	}
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

const Vector2f& PlayerPhysicsComponent::getDownwards() const {
	return downwards;
}

void PlayerPhysicsComponent::setDownwards(const Vector2f downwards) {
	this->downwards = downwards;
}

const Vector2f& PlayerPhysicsComponent::getLeftwards() const {
	return leftwards;
}

void PlayerPhysicsComponent::setLeftwards(const Vector2f leftwards) {
	this->leftwards = leftwards;
}