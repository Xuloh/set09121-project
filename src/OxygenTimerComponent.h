#pragma once

#include <ecm/ecm.h>

class OxygenTimerComponent : public ecm::Component {
protected:
    float remainingTime;
    float depleteRate;

public:
    OxygenTimerComponent() = delete;
    OxygenTimerComponent(ecm::Entity* parent, float remainingTime, float depleteRate = 1.f);
    ~OxygenTimerComponent() = default;

    void update(double dt) override;
    void render() override;

    float getRemainingTime() const;
    float getDepleteRate() const;
    void setDepleteRate(float depleteRate);
};