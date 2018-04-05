#include "PhysicsComponent.h"
#include <iostream>

using namespace std;
using namespace sf;
using namespace physics;

PhysicsComponent::PhysicsComponent(ecm::Entity* parent, bool dynamic, const Vector2f& size) : Component(parent), dynamic(dynamic) {
    b2BodyDef bodyDef;
    bodyDef.type = this->dynamic ? b2_dynamicBody : b2_staticBody;
    bodyDef.position = Vector2f_To_b2Vec2(invertHeight(this->parent->getPosition()));

    body = getWorld()->CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(size.x * scaleInv * .5f, size.y * scaleInv * .5f);

    b2FixtureDef fixtureDef;
    fixtureDef.friction = this->dynamic ? .1f : .8f;
    fixtureDef.restitution = .2f;
    fixtureDef.shape = &shape;

    fixture = body->CreateFixture(&fixtureDef);
}

PhysicsComponent::~PhysicsComponent() {
    body->SetActive(false);
    getWorld()->DestroyBody(body);
    body = nullptr;
}

b2Body& PhysicsComponent::getBody() const {
    return *body;
}

b2Fixture& PhysicsComponent::getFixture() const {
    return *fixture;
}

void PhysicsComponent::update(double dt) {
    parent->setPosition(invertHeight(b2Vec2_To_Vector2f(body->GetPosition())));
    parent->setRotation(radiansToDegrees(body->GetAngle()));
}

void PhysicsComponent::render() {}

bool PhysicsComponent::isTouching(const PhysicsComponent& other) const {
    b2Contact* contact;
    return isTouching(other, contact);
}

bool PhysicsComponent::isTouching(const PhysicsComponent& other, b2Contact const* contact) const {
    const auto otherFixture = &other.getFixture();
    const auto& world = *getWorld();
    const auto contactList = world.GetContactList();
    for(int32 i = 0; i < world.GetContactCount(); i++) {
        const auto& currentContact = contactList[i];
        if(currentContact.IsTouching() &&
            ((currentContact.GetFixtureA() == fixture && currentContact.GetFixtureB() == otherFixture) ||
            (currentContact.GetFixtureA() == otherFixture && currentContact.GetFixtureB() == fixture))) {
            contact = &currentContact;
            return true;
        }
    }
    return false;
}

std::vector<const b2Contact*> PhysicsComponent::getTouching() const {
    vector<const b2Contact*> result;
    auto edge = body->GetContactList();
    while(edge != nullptr) {
        const auto contact = edge->contact;
        if (contact->IsTouching())
            result.push_back(contact);
        edge = edge->next;
    }

    return result;
}

const Vector2f PhysicsComponent::getVelocity() const {
    return b2Vec2_To_Vector2f(body->GetLinearVelocity());
}

void PhysicsComponent::setVelocity(const Vector2f& velocity) const {
    body->SetLinearVelocity(Vector2f_To_b2Vec2(velocity));
}

void PhysicsComponent::setRestitution(const float restitution) const {
    fixture->SetRestitution(restitution);
}

void PhysicsComponent::setFriction(const float friction) const {
    fixture->SetFriction(friction);
}

void PhysicsComponent::setMass(const float mass) const {
    fixture->SetDensity(mass);
}

void PhysicsComponent::impulse(const Vector2f& impulse) const {
    body->ApplyLinearImpulseToCenter({ impulse.x, -impulse.y }, true);
}

void PhysicsComponent::dampen(const Vector2f& factor) const {
    const auto velocity = body->GetLinearVelocity();
    body->SetLinearVelocity({ velocity.x * factor.x, velocity.y * factor.y });
}

void PhysicsComponent::teleport(const Vector2f& position) const {
    body->SetTransform(Vector2f_To_b2Vec2(invertHeight(position)), body->GetAngle());
}
