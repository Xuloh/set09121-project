#include "SpriteComponent.h"
#include <renderer/renderer-system.h>

using namespace std;
using namespace sf;

SpriteComponent::SpriteComponent(ecm::Entity* parent) : Component(parent) {}

void SpriteComponent::update(double dt) {
    const auto size = sprite->getLocalBounds();
    const auto origin = parent->getOrigin();
    sprite->setOrigin(size.width * origin.x, size.height * origin.y);
    sprite->setPosition(parent->getPosition());
    sprite->setRotation(parent->getRotation());
}

void SpriteComponent::render() {
    renderer::queue(sprite.get());
}

Sprite& SpriteComponent::getSprite() const {
    return *sprite;
}
