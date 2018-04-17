#pragma once

#include <ecm/ecm.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// A component that automatically triggers a game over when its timer runs out
// on top of that, throughtout the duration of the timer, it applies a fade effect on the screen
// and plays heart beat sounds depending on the remaining time
// it also has a deplete rate that changes the rate at which the timer decreases
class OxygenTimerComponent : public ecm::Component {
protected:
    float initialTime;
    float remainingTime;
    float depleteRate;
    sf::Sound slowHeartBeatSound;
    sf::Sound fastHeartBeatSound;
    sf::VertexArray fadeVertices;

public:
    OxygenTimerComponent() = delete;
    OxygenTimerComponent(ecm::Entity* parent, float initialTime, float depleteRate = 1.f);
    ~OxygenTimerComponent() = default;

    void update(double dt) override;
    void render() override;

    // get the remaining time
    float getRemainingTime() const;

    // get the deplete rate
    float getDepleteRate() const;

    // change the deplete rate
    // 1.0f will deplete the timer at normal speed
    // 0.5f at half the normal speed
    // and 2.0f at twice the normal speed
    void setDepleteRate(float depleteRate);
};
