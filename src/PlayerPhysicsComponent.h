#pragma once

#include <physics/PhysicsComponent.h>
#include <ecm/ecm.h>
#include <SFML/Graphics.hpp>
#include <event/event-system.h>

class PlayerPhysicsComponent : public physics::PhysicsComponent, public event::EventHandler {
protected:
    b2Vec2 size;
    sf::Vector2f maxVelocity;
    bool grounded;
    float groundSpeed;
    bool jump;

    bool isGrounded() const;
public:
    PlayerPhysicsComponent() = delete;
    explicit PlayerPhysicsComponent(ecm::Entity* parent, const sf::Vector2f& size);
    ~PlayerPhysicsComponent() override;

    void update(double dt) override;
    void handleEvent(const sf::Event& event) override;

    float getGroundSpeed() const;
    void setGroundSpeed(float groudSpeed);

    const sf::Vector2f& getMaxVelocity() const;
    void setMaxVelocity(sf::Vector2f maxVelocity);
};