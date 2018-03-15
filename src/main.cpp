#include <ecm.h>
#include <event-system.h>
#include <renderer-system.h>
#include <input-system.h>
#include <SFML/Graphics.hpp>
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

void load() {
	// register window handlers
	event::registerHandler(Event::Closed, &closeWindow);
	event::registerHandler(Event::KeyReleased, &closeWindowOnEscapePressed);

	font.loadFromFile("res/fonts/FiraCode-Medium.ttf");

	mainMenu.reset(new MainMenuScene());
	mainMenu->load();

	activeScene = mainMenu;
}

void update() {
	static Clock clock;
	const auto dt = clock.restart().asSeconds();

	event::update();

	activeScene->update(dt);
}

void render() {
	activeScene->render();
	renderer::render();
}

int main() {
	RenderWindow window(VideoMode(800, 600), "MOIM", Style::Titlebar | Style::Close);
	renderer::initialise(window);

	load();

	while(window.isOpen()) {
		window.clear();
		update();
		render();
		window.display();
	}
	
	renderer::shutdown();

	return 0;
}