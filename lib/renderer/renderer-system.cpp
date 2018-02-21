#include <exception>
#include <queue>
#include "renderer-system.h"

using namespace std;
using namespace sf;

static queue<const Drawable*> drawables;
static RenderWindow* renderWindow;

RenderWindow& renderer::getWindow() {
	return *renderWindow;
}

void renderer::initialise(sf::RenderWindow& window) {
	renderWindow = &window;
}

void renderer::shutdown() {
	while (!drawables.empty())
		drawables.pop();
}

void renderer::render() {
	if (renderWindow == nullptr)
		throw exception("No render window set :(");

	while (!drawables.empty()) {
		renderWindow->draw(*drawables.front());
		drawables.pop();
	}
}

void renderer::queue(const Drawable* drawable) {
	drawables.push(drawable);
}