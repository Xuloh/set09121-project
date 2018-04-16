#include "OxygenTimerComponent.h"
#include <iostream>
#include <ecm/scene-manager.h>

using namespace std;
using namespace ecm;

OxygenTimerComponent::OxygenTimerComponent(Entity* parent, const float remainingTime, const float depleteRate) : Component(parent) {
    this->remainingTime = remainingTime;
    this->depleteRate = depleteRate;
}

void OxygenTimerComponent::update(const double dt) {
    remainingTime -= dt * depleteRate;
    if (remainingTime <= 0.f) {
        cout << "oxygen timer ran out :(" << endl;
        scene::load("game-over");
    }
}

void OxygenTimerComponent::render() {}

float OxygenTimerComponent::getRemainingTime() const {
    return remainingTime;
}

float OxygenTimerComponent::getDepleteRate() const {
    return depleteRate;
}

void OxygenTimerComponent::setDepleteRate(const float depleteRate) {
    this->depleteRate = depleteRate;
}
