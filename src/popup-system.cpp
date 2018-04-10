#include "popup-system.h"
#include "main.h"
#include <SFML/Graphics.hpp>
#include <ecm/ecm.h>
#include <gui/gui.h>
#include <memory>

using namespace std;
using namespace sf;
using namespace ecm;
using namespace gui;

static shared_ptr<Entity> popupText;
static shared_ptr<Entity> popupTitle;
static shared_ptr<Entity> layout;

static float timer = 0.f;
static bool activePopup = false;

void popup::initialise() {
    auto guiFactory = GUIFactory({ Color::White, Color::Cyan, &font, 24.f });

    popupText = guiFactory.makeLabel("Popup text");
    popupTitle = guiFactory.makeLabel("Popup title");

    layout = make_shared<Entity>();
    auto layoutComponent = layout->addComponent<LayoutComponent>(1.f, 1.f);
    layoutComponent->addItem(popupText, .1f, .8f);
    layoutComponent->addItem(popupTitle, .1f, .7f);
}

void popup::shutdown() {
    timer = 0.f;
    activePopup = false;
}

void popup::popup(const string& message, const float time, const string& title) {
    popupText->getComponent<TextComponent>()->getText().setString(message);
    popupTitle->getComponent<TextComponent>()->getText().setString(title);

    timer = time;
    activePopup = true;
}

void popup::update(const double dt) {
    if (activePopup) {
        timer -= dt;
        if (timer <= 0)
            activePopup = false;
        else {
            layout->update(dt);
            popupText->update(dt);
            popupTitle->update(dt);
        }
    }
}

void popup::render() {
    if (activePopup) {
        popupTitle->render();
        popupText->render();
    }
}
