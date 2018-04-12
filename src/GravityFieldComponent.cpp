#include <physics/physics-system.h>
#include <physics/PhysicsComponent.h>
#include "GravityFieldComponent.h"

using namespace std;
using namespace ecm;
using namespace physics;

GravityFieldComponent::GravityFieldComponent(Entity* parent, const float fieldRadius) : Component(parent) {
    const auto physics = this->parent->getComponent<PhysicsComponent>();
    force = .0f;
    b2CircleShape shape;
    shape.m_p = Vector2f_To_b2Vec2(invertHeight(this->parent->getPosition()));
    shape.m_radius = fieldRadius;
    b2FixtureDef fixtureDef;
    fixtureDef.isSensor = true;
    fixtureDef.shape = &shape;
    field = physics->getBody().CreateFixture(&fixtureDef);
}

void GravityFieldComponent::update(double dt) {
    auto i = 0;
    for(auto contactEdge = field->GetBody()->GetContactList(); contactEdge; contactEdge = contactEdge->next) {
        const auto contact = contactEdge->contact;
        if (contact->IsTouching() && isContactWithThis(contact)) {
            const auto other = getOtherFixture(contact);            
            auto direction = field->GetBody()->GetPosition() - other->GetBody()->GetPosition();
            // newton's equation of gravitational force without the gravitational constant
            const auto force = this->force * other->GetBody()->GetMass() / direction.LengthSquared();
            direction.Normalize();
            direction *= force;
            other->GetBody()->ApplyForceToCenter(direction, true);
        }
    }
}

void GravityFieldComponent::render() {}

void GravityFieldComponent::setForce(const float force) {
    this->force = force;
}

float GravityFieldComponent::getForce() const {
    return force;
}


bool GravityFieldComponent::isContactWithThis(b2Contact* contact) const{
    return contact->GetFixtureA() == field || contact->GetFixtureB() == field;
}

b2Fixture* GravityFieldComponent::getOtherFixture(b2Contact* contact) const {
    return contact->GetFixtureA() == field ? contact->GetFixtureB() : contact->GetFixtureA();
}
