#include <exception>
#include <queue>
#include <memory>
#include "renderer-system.h"

using namespace std;
using namespace sf;

static queue<const Drawable*> drawables;
static queue<const Drawable*> gui;
static RenderWindow* renderWindow;
static shared_ptr<View> sceneView;
static shared_ptr<View> guiView;

RenderWindow& renderer::getWindow() {
	return *renderWindow;
}

View& renderer::getSceneView() {
    return *sceneView;
}

View& renderer::getGUIView() {
    return *guiView;
}

void renderer::initialise(RenderWindow& window) {
	renderWindow = &window;

    const auto windowSize = Vector2f(window.getSize());
    sceneView.reset(new View({ 0.f, -windowSize.y, windowSize.x, windowSize.y }));
    guiView.reset(new View({ 0.f, 0.f, windowSize.x, windowSize.y }));
}

void renderer::shutdown() {
	while (!drawables.empty())
		drawables.pop();
}

void renderer::render() {
	if (renderWindow == nullptr)
		throw exception("No render window set :(");

    renderWindow->setView(*sceneView);
	while (!drawables.empty()) {
		renderWindow->draw(*drawables.front());
		drawables.pop();
	}

    renderWindow->setView(*guiView);
    while(!gui.empty()) {
        renderWindow->draw(*gui.front());
        gui.pop();
    }
}

void renderer::queue(const Drawable* drawable, bool forGUI) {
    if (!forGUI)
        drawables.push(drawable);
    else
        gui.push(drawable);
}