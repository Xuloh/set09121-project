#pragma once

#include <Box2D/Box2D.h>
#include "ecm/ecm.h"
#include "physics-system.h"

namespace physics {
    
    class PhysicsComponent : public ecm::Component {
    protected:
        b2Body* body;
        b2Fixture* fixture;
        const bool dynamic;
    public:
        PhysicsComponent() = delete;
        explicit PhysicsComponent(ecm::Entity* parent, bool dynamic, const sf::Vector2f& size);

        ~PhysicsComponent() override;

        b2Body& getBody() const;
        b2Fixture& getFixture() const;

        void update(double dt) override;
        void render() override;

        bool isTouching(const PhysicsComponent& other) const;
        bool isTouching(const PhysicsComponent& other, b2Contact const* contact) const;
        std::vector<const b2Contact*> getTouching() const;

        const sf::Vector2f getVelocity() const;
        void setVelocity(const sf::Vector2f& velocity) const;

        void setRestitution(float restitution) const;
        void setFriction(float friction) const;
        void setMass(float mass) const;
        void impulse(const sf::Vector2f& impulse) const;
        void dampen(const sf::Vector2f& factor) const;
        void teleport(const sf::Vector2f& position) const;
    };
}
