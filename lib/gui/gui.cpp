#include "gui.h"
#include <memory>

using namespace gui;
using namespace sf;
using namespace std;
using namespace event;

// *** TextComponent class *** //

TextComponent::TextComponent(ecm::Entity* parent) : Component(parent) {}

void TextComponent::update(double dt) {
	text->setPosition(parent->getPosition());
}

void TextComponent::render() {
	renderer::queue(text.get());
}

Text& TextComponent::getText() const {
	return *text;
}

// *** ClickComponent class *** //

ClickComponent::ClickComponent(ecm::Entity* parent) : Component(parent), targetText(parent->getComponent<TextComponent>()->getText()), onClick(nullptr) {
	registerHandler(Event::MouseButtonReleased, this);
}

ClickComponent::~ClickComponent() {
	unregisterHandler(Event::MouseButtonReleased, this);
}

void ClickComponent::handleEvent(const sf::Event& event) {
	if (onClick != nullptr && event.mouseButton.button == Mouse::Left && targetText.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
		onClick(event);
}

void ClickComponent::update(double dt) {}

void ClickComponent::render() {}

// *** MouseHoverComponent class *** //

MouseHoverComponent::MouseHoverComponent(ecm::Entity* parent) : Component(parent), targetText(parent->getComponent<TextComponent>()->getText()), isMouseInArea(false) {
	registerHandler(Event::MouseMoved, this);
}

MouseHoverComponent::~MouseHoverComponent() {
	unregisterHandler(Event::MouseMoved, this);
}

void MouseHoverComponent::handleEvent(const sf::Event& event) {
	if (!isMouseInArea && targetText.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
		isMouseInArea = true;
		targetText.setFillColor(hoverColor);
	}
	else if (isMouseInArea && !targetText.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
		isMouseInArea = false;
		targetText.setFillColor(baseColor);
	}
}

void MouseHoverComponent::update(double dt) {}

void MouseHoverComponent::render() {}
