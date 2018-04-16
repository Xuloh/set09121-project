#pragma once

#include <ecm/ecm.h>
#include <SFML/Graphics.hpp>

class OxygenTimerComponent : public ecm::Component {
protected:
    float initialTime;
    float remainingTime;
    float depleteRate;
    sf::VertexArray fadeVertices;

public:
    OxygenTimerComponent() = delete;
    OxygenTimerComponent(ecm::Entity* parent, float initialTime, float depleteRate = 1.f);
    ~OxygenTimerComponent() = default;

    void update(double dt) override;
    void render() override;

    float getRemainingTime() const;
    float getDepleteRate() const;
    void setDepleteRate(float depleteRate);
};
