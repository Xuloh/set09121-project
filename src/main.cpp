#include <SFML/Graphics.hpp>
#include <engine.h>
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
	event::registerHandler(Event::Closed, make_shared<event::eventFunctionType>(&closeWindow));
	event::registerHandler(Event::KeyReleased, make_shared<event::eventFunctionType>(&closeWindowOnEscapePressed));

    // setup keyboard controls
    input::addControl("Left", Keyboard::Q, Keyboard::A);
    input::addControl("Right", Keyboard::D, Keyboard::D);
    input::addControl("Jump", Keyboard::Space, Keyboard::Space);
    input::addControl("GravityLeft", Keyboard::A, Keyboard::Q);
    input::addControl("GravityRight", Keyboard::E, Keyboard::E);
    input::setQwertyActive();

    // setup controller controls
    input::bindJoystickAxis("Left", Joystick::X, -100.f, 0.f);
    input::bindJoystickAxis("Right", Joystick::X, 0.f, 100.f);
    input::bindJoystickButton("Jump", 0);
    input::bindJoystickButton("GravityLeft", 1);
    input::bindJoystickButton("GravityRight", 2);

    // setup scenes
    scene::add("main-menu", make_shared<MainMenuScene>());
    scene::add("test-level", make_shared<TestLevelScene>());
    scene::load("main-menu");
}

void update() {
	static Clock clock;
	const auto dt = clock.restart().asSeconds();

	event::update();
	physics::update(dt);
    scene::update(dt);
    popup::update(dt);
}

void render() {
    scene::render();
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

    scene::shutdown();
	physics::shutdown();
	renderer::shutdown();
    popup::shutdown();

	return 0;
}