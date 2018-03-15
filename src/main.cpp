#include <ecm.h>
#include <gui.h>
#include <event-system.h>
#include <renderer-system.h>
#include <input-system.h>
#include <SFML/Graphics.hpp>

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
}

void update() {
	static Clock clock;
	auto dt = clock.restart().asSeconds();

	event::update();
}

void render() {
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