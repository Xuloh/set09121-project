#pragma once

#include <physics/PhysicsComponent.h>
#include <ecm/ecm.h>
#include <SFML/Graphics.hpp>
#include <event/event-system.h>

// A subclass of PhysicsComponent that implements physics specific to the player
// for example : moving and jumping in response to key presses
class PlayerPhysicsComponent : public physics::PhysicsComponent, public event::EventHandler {
protected:
    b2Vec2 size;
    sf::Vector2f maxVelocity;
    bool grounded;
    float groundSpeed;
    bool jump;
    sf::Vector2f downwards;
    sf::Vector2f leftwards;

    bool isGrounded() const;
    bool isBelow(b2Vec2 point) const;
public:
    PlayerPhysicsComponent() = delete;
    explicit PlayerPhysicsComponent(ecm::Entity* parent, const sf::Vector2f& size);
    ~PlayerPhysicsComponent() override;

    void update(double dt) override;
    void handleEvent(const sf::Event& event) override;

    // set and get the maximum ground speed of the player
    float getGroundSpeed() const;
    void setGroundSpeed(float groudSpeed);

    // set and get the maximum velocity of the player
    const sf::Vector2f& getMaxVelocity() const;
    void setMaxVelocity(sf::Vector2f maxVelocity);

    // set and get the downwards vector of the player
    // this vector represents the downards direction of the player
    // it usually is the normalised gravity vector
    const sf::Vector2f& getDownwards() const;
    void setDownwards(sf::Vector2f downwards);

    // set and get the leftwards vector of the player
    // this vector represents the leftwards direction of the player
    // it should always be relative to the player's downwards direction
    const sf::Vector2f& getLeftwards() const;
    void setLeftwards(sf::Vector2f leftwards);
};
