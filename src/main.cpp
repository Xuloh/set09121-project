#include <SFML/Graphics.hpp>
#include <engine.h>
#include "Scenes.h"
#include "main.h"
#include "popup-system.h"

using namespace std;
using namespace sf;

void closeWindow(const Event& event) {
    // only exit the game when in the main menu
    if (scene::getCurrentScene() == "main-menu")
        renderer::getWindow().close();
    // otherwise return to the main menu
    else
        scene::load("main-menu");
}

void closeWindowOnEscapePressed(const Event& event) {
    // when not using a controller, exit when escape is pressed
    if (!input::usingController())
        if (event.key.code == Keyboard::Escape)
            closeWindow(event);
}

void closeWindowWithController(const Event& event) {
    // when using a controller, escape is mapped to select and start
    if (input::usingController())
        if (event.joystickButton.button == 6 || event.joystickButton.button == 7)
            closeWindow(event);
}

void load() {
	// register window handlers
	event::registerHandler(Event::Closed, make_shared<event::eventFunctionType>(&closeWindow));
	event::registerHandler(Event::KeyPressed, make_shared<event::eventFunctionType>(&closeWindowOnEscapePressed));
    event::registerHandler(Event::JoystickButtonPressed, make_shared<event::eventFunctionType>(&closeWindowWithController));

    // setup keyboard controls
    input::addControl("Left", Keyboard::Q, Keyboard::A);
    input::addControl("Right", Keyboard::D, Keyboard::D);
    input::addControl("Jump", Keyboard::Space, Keyboard::Space);
    input::addControl("GravityLeft", Keyboard::A, Keyboard::Q);
    input::addControl("GravityRight", Keyboard::E, Keyboard::E);
    input::setQwertyActive();

    // setup controller controls
	input::bindJoystickAxis("Left", Joystick::X, -100.f, -30.f);
	input::bindJoystickAxis("Right", Joystick::X, 30.f, 100.f);
	input::bindJoystickButton("Jump", 0);
	input::bindJoystickButton("GravityLeft", 4);
	input::bindJoystickButton("GravityRight", 5);

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