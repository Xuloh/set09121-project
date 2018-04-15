#include <exception>
#include <queue>
#include <memory>
#include "renderer-system.h"
#include "event/event-system.h"

using namespace std;
using namespace sf;

static queue<const Drawable*> drawables;
static queue<const Drawable*> gui;
static RenderWindow* renderWindow;
static shared_ptr<View> sceneView;
static shared_ptr<View> guiView;

static void resizeView(const Event& event) {
    const Vector2f newSize = { float(event.size.width), float(event.size.height) };
    sceneView->setSize(newSize);
    guiView->setCenter(newSize * .5f);
    guiView->setSize(newSize);
}

static auto resizeHandler = make_shared<event::eventFunctionType>(&resizeView);

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

    event::registerHandler(Event::Resized, resizeHandler);
}

void renderer::shutdown() {
	while (!drawables.empty())
		drawables.pop();

    while (!gui.empty())
        gui.pop();

    event::unregisterHandler(Event::Resized, resizeHandler);
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