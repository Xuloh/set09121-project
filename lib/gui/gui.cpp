#include <memory>
#include "gui.h"

using namespace gui;
using namespace sf;
using namespace std;
using namespace event;

// *** GUIFactory class *** //

GUIFactory::GUIFactory(const GUISettings settings) : settings(settings) {}

std::shared_ptr<ecm::Entity> GUIFactory::makeLabel(String text) const {
	auto label = make_shared<ecm::Entity>();

	auto labelText = label->addComponent<TextComponent>();
	labelText->setText();
	labelText->getText().setFont(*settings.font);
	labelText->getText().setCharacterSize(settings.characterSize);
	labelText->getText().setFillColor(settings.baseColor);
	labelText->getText().setString(text);

	return label;
}

std::shared_ptr<ecm::Entity> GUIFactory::makeButton(String text, const eventFunction onClickHandler) const {
	auto button = make_shared<ecm::Entity>();

	auto buttonText = button->addComponent<TextComponent>();
	buttonText->setText();
	buttonText->getText().setFont(*settings.font);
	buttonText->getText().setCharacterSize(settings.characterSize);
	buttonText->getText().setFillColor(settings.baseColor);
	buttonText->getText().setString(text);

	auto buttonClick = button->addComponent<ClickComponent>();
	buttonClick->onClick = onClickHandler;

	auto buttonHover = button->addComponent<MouseHoverComponent>();
	buttonHover->baseColor = settings.baseColor;
	buttonHover->hoverColor = settings.hoverColor;

	return button;
}

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

// *** LayoutComponent class *** //

LayoutComponent::LayoutComponent(ecm::Entity* parent) : LayoutComponent(parent, 0.f, 0.f) {}
LayoutComponent::LayoutComponent(ecm::Entity* parent, float x, float y) : Component(parent), x(x), y(y) {
	updateSize();
	updateLayout = true;
	registerHandler(Event::Resized, this);
}

LayoutComponent::~LayoutComponent() {
	unregisterHandler(Event::Resized, this);
}

void LayoutComponent::update(double dt) {
	if (updateLayout) {
		for (auto& item : items)
			item.entity->setPosition(parent->getPosition() + Vector2f(item.x * width, item.y * height));
		updateLayout = false;
	}
}

void LayoutComponent::render() {}


void LayoutComponent::handleEvent(const Event& event) {
	updateLayout = true;
	updateSize();
}

void LayoutComponent::addItem(std::shared_ptr<ecm::Entity> entity, float x, float y) {
	items.push_back({ entity, x, y });
}


void LayoutComponent::updateSize() {
	width = renderer::getWindow().getView().getSize().x * x;
	height = renderer::getWindow().getView().getSize().y * y;
}
