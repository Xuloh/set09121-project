#include <renderer/renderer-system.h>
#include "ShapeComponent.h"

ShapeComponent::ShapeComponent(ecm::Entity* parent) : Component(parent) {}

void ShapeComponent::update(double dt) {
    const auto size = shape->getLocalBounds();
    const auto origin = parent->getOrigin();
    shape->setOrigin(size.width * origin.x, size.height * origin.y);
    shape->setPosition(parent->getPosition());
    shape->setRotation(parent->getRotation());
}

void ShapeComponent::render() {
    renderer::queue(shape.get());
}

sf::Shape& ShapeComponent::getShape() const {
    return *shape;
}
