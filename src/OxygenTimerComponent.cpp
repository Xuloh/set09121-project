#include "OxygenTimerComponent.h"
#include <iostream>
#include <ecm/scene-manager.h>
#include <renderer/renderer-system.h>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace ecm;
using namespace sf;

OxygenTimerComponent::OxygenTimerComponent(Entity* parent, const float initialTime, const float depleteRate) : Component(parent) {
    this->initialTime = initialTime;
    this->remainingTime = initialTime;
    this->depleteRate = depleteRate;
    fadeVertices = VertexArray(Quads, 4);
}

void OxygenTimerComponent::update(const double dt) {
    remainingTime -= dt * depleteRate;
    if (remainingTime <= 0.f) {
        cout << "oxygen timer ran out :(" << endl;
        scene::load("game-over");
    }
}

void OxygenTimerComponent::render() {
    const auto& windowSize = renderer::getWindow().getSize();

    fadeVertices[0].position = { 0, 0 };
    fadeVertices[1].position = { float(windowSize.x), 0 };
    fadeVertices[2].position = { float(windowSize.x), float(windowSize.y) };
    fadeVertices[3].position = { 0, float(windowSize.y) };

    for(auto i = 0; i < 4; i++)
        fadeVertices[i].color = { 0, 0, 0, Uint8((initialTime - remainingTime) / initialTime * 255) };

    renderer::queue(&fadeVertices, true);
}

float OxygenTimerComponent::getRemainingTime() const {
    return remainingTime;
}

float OxygenTimerComponent::getDepleteRate() const {
    return depleteRate;
}

void OxygenTimerComponent::setDepleteRate(const float depleteRate) {
    this->depleteRate = depleteRate;
}
