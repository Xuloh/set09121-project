#pragma once

#include <ecm/ecm.h>
#include <event/event-system.h>
#include <memory>
#include "PlayerPhysicsComponent.h"

class RotateGravityComponent : public ecm::Component, public event::EventHandler {
protected:
    std::shared_ptr<PlayerPhysicsComponent> playerPhysics;
    bool rotateLeft;
    bool rotateRight;

public:
    RotateGravityComponent() = delete;
    explicit RotateGravityComponent(ecm::Entity* parent);
    ~RotateGravityComponent();

    void update(double dt) override;
    void render() override;
    void handleEvent(const sf::Event& event) override;
};
