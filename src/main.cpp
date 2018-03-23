#include <SFML/Graphics.hpp>
#include <iostream>
#include <engine.h>
#include "Scenes.h"
#include "main.h"

using namespace std;
using namespace sf;

Font font;

void closeWindow(const Event& event) {
	renderer::getWindow().close();
}

void closeWindowOnEscapePressed(const Event& event) {
	if (event.key.code == Keyboard::Escape)
		closeWindow(event);
}

void resizeView(const Event& event) {
    const Vector2f newSize = { float(event.size.width), float(event.size.height) };
    renderer::getSceneView().setSize(newSize);
    renderer::getGUIView().setCenter(newSize * .5f);
    renderer::getGUIView().setSize(newSize);
}

void load() {
	// register window handlers
	event::registerHandler(Event::Closed, &closeWindow);
	event::registerHandler(Event::KeyReleased, &closeWindowOnEscapePressed);
	event::registerHandler(Event::Resized, &resizeView);

	font.loadFromFile("res/fonts/FiraCode-Medium.ttf");

	mainMenu.reset(new MainMenuScene());
	mainMenu->load();

	activeScene = mainMenu;
}

void update() {
	static Clock clock;
	const auto dt = clock.restart().asSeconds();

	event::update();

	physics::update(dt);

	activeScene->update(dt);
}

void render() {
	activeScene->render();
	renderer::render();
}

int main() {
	RenderWindow window(VideoMode(800, 600), "MOIM");

	renderer::initialise(window);
	physics::initialise();

	load();

	while(window.isOpen()) {
		window.clear();
		update();
		render();
		window.display();
	}
	
	physics::shutdown();
	renderer::shutdown();

	return 0;
}