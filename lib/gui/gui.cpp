#include "gui.h"
#include <memory>

using namespace gui;
using namespace sf;
using namespace std;
using namespace event;

// *** GUIElement class *** //

GUIElement::GUIElement(const FloatRect& hitArea) : hitArea(make_shared<FloatRect>(hitArea)) {}

void GUIElement::handleEvent(const Event& event) {
	switch(event.type) {
		case Event::MouseMoved:
			if(!mouseInArea && hitArea->contains(event.mouseMove.x, event.mouseMove.y)) {
				mouseInArea = true;
				onMouseEnter(event);
			}
			else if(mouseInArea && !hitArea->contains(event.mouseMove.x, event.mouseMove.y)) {
				mouseInArea = false;
				onMouseExit(event);
			}
			break;
		case Event::MouseButtonReleased:
			if(mouseInArea && event.mouseButton.button == Mouse::Left)
				onClick(event);
			break;
		default:
			break;
	}
}

// *** Label class *** //

Label::Label(const FloatRect& hitArea) : GUIElement(hitArea) {}

void Label::render() {
	renderer::queue(text.get());
}

// a label doesn't react to any event
void Label::onMouseEnter(const Event& event) {}

void Label::onMouseExit(const Event& event) {}

void Label::onClick(const Event& event) {}

void Label::handleEvent(const Event& event) {}

// *** Button class *** //

Button::Button(const FloatRect& hitArea) : GUIElement(hitArea) {
	registerHandler(Event::MouseMoved, this);
	registerHandler(Event::MouseButtonReleased, this);
}

Button::~Button() {
	unregisterHandler(Event::MouseMoved, this);
	unregisterHandler(Event::MouseButtonReleased, this);

	delete &hitArea;
	delete &baseColor;
	delete &hoverColor;
	delete &text;
}


void Button::render() {
	renderer::queue(text.get());
}

void Button::onMouseEnter(const Event& event) {
	text->setFillColor(*hoverColor);
}

void Button::onMouseExit(const Event& event) {
	text->setFillColor(*baseColor);
}

void Button::onClick(const Event& event) {
	if(onClickHandler != nullptr)
		onClickHandler(event);
}

// *** GUIFactory class *** //

GUIFactory::GUIFactory(const float baseCharacterSize) : baseCharacterSize(baseCharacterSize) {}

std::shared_ptr<Label> GUIFactory::makeLabel(const string& text, const sf::Vector2f& position, float characterSize) const {
	auto label = make_shared<Label>();

	label->text = make_shared<Text>();
	label->text->setFont(*font);
	label->text->setCharacterSize(baseCharacterSize * characterSize);
	label->text->setFillColor(*baseColor);
	label->text->setString(text);
	label->text->setPosition(position);

	label->hitArea = make_shared<FloatRect>(label->text->getGlobalBounds());

	return label;
}

shared_ptr<Button> GUIFactory::makeButton(const string& text, const Vector2f& position, const eventFunction onClickHandler, const float characterSize) const {
	auto button = make_shared<Button>();
	
	button->baseColor = baseColor;
	button->hoverColor = hoverColor;
	
	button->onClickHandler = onClickHandler;

	button->text = make_shared<Text>();
	button->text->setFont(*font);
	button->text->setCharacterSize(baseCharacterSize * characterSize);
	button->text->setFillColor(*baseColor);
	button->text->setString(text);
	button->text->setPosition(position);

	button->hitArea = make_shared<FloatRect>(button->text->getGlobalBounds());

	return button;
}
