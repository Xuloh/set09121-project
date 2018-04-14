#include <SFML/Graphics.hpp>
#include <engine.h>
#include <iostream>
#include "Scenes.h"
#include "main.h"
#include "popup-system.h"

using namespace std;
using namespace sf;

void closeWindow(const Event& event) {
	renderer::getWindow().close();
}

void closeWindowOnEscapePressed(const Event& event) {
	if (event.key.code == Keyboard::Escape)
		closeWindow(event);
}

void load() {
	// register window handlers
	event::registerHandler(Event::Closed, &closeWindow);
	event::registerHandler(Event::KeyReleased, &closeWindowOnEscapePressed);

    // setup controls
    input::addControl("Left", Keyboard::Q, Keyboard::A);
    input::addControl("Right", Keyboard::D, Keyboard::D);
    input::addControl("Jump", Keyboard::Space, Keyboard::Space);
    input::addControl("GravityLeft", Keyboard::A, Keyboard::Q);
    input::addControl("GravityRight", Keyboard::E, Keyboard::E);
    input::setAzertyActive();

	mainMenu.reset(new MainMenuScene());
	mainMenu->load();

    testLevel.reset(new TestLevelScene());
    //testLevel->load();

	activeScene = mainMenu;
}

void update() {
	static Clock clock;
	const auto dt = clock.restart().asSeconds();

	event::update();
	physics::update(dt);

	activeScene->update(dt);

    popup::update(dt);
}

void render() {
	activeScene->render();
    popup::render();
	renderer::render();
}

int main() {
	RenderWindow window(VideoMode(800, 600), "MOIM");

    renderer::initialise(window);
	physics::initialise();
    popup::initialise();

	load();

	while(window.isOpen()) {
		window.clear();
		update();
		render();
		window.display();
	}

    mainMenu.reset();
    testLevel.reset();
    activeScene.reset();

	physics::shutdown();
	renderer::shutdown();
    popup::shutdown();

	return 0;
}