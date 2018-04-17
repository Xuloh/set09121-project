#pragma once

#include <Box2D/Box2D.h>
#include "ecm/ecm.h"
#include "physics-system.h"

namespace physics {
    
    // a Component that handles a b2Body and updates the position and rotation of its parent entity
    // based on the position and rotation of this b2Body
    class PhysicsComponent : public ecm::Component {
    protected:
        b2Body* body;
        b2Fixture* fixture;
        const bool dynamic;
    public:
        PhysicsComponent() = delete;
        explicit PhysicsComponent(ecm::Entity* parent, bool dynamic, const sf::Vector2f& size);

        ~PhysicsComponent() override;

        // get a reference to the b2Body
        b2Body& getBody() const;

        // get a reference to the b2Fixture
        b2Fixture& getFixture() const;

        void update(double dt) override;
        void render() override;

        // compute whether the given PhysicsComponent is touching this PhysicsComponent
        bool isTouching(const PhysicsComponent& other) const;

        // compute whether the given PhysicsComponent is touching this PhysicsComponent
        // contact data will be written to the given b2Contact
        bool isTouching(const PhysicsComponent& other, b2Contact const* contact) const;
        std::vector<const b2Contact*> getTouching() const;

        // set/get the velocity of the b2Body
        sf::Vector2f getVelocity() const;
        void setVelocity(const sf::Vector2f& velocity) const;

        // set the resitution (bounciness) of the b2Fixture
        void setRestitution(float restitution) const;

        // set the friction of the b2Fixture
        void setFriction(float friction) const;

        // set the mass of the b2Body
        void setMass(float mass) const;

        // applies an impulse to the b2Body (impulses directly affect the velocity of the b2Body, unlike forces)
        void impulse(const sf::Vector2f& impulse) const;

        // dampens the velocity of the b2Body
        void dampen(const sf::Vector2f& factor) const;

        // moves the b2Body to the given position
        void teleport(const sf::Vector2f& position) const;

        // rotate the b2Body by the given angle relative to its current rotation
        void rotate(float angle) const;
    };
}
