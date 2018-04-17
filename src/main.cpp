#include <SFML/Graphics.hpp>
#include <engine.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "Scenes.h"
#include "main.h"
#include "popup-system.h"
#include "tilemap-system.h"

using namespace std;
using namespace sf;

Text framerateCounter;
bool displayFramerate = false;
float frametimes[256] = {};
uint8_t ftc = 0;

void closeWindow(const Event& event) {
    // only exit the game when in the main menu or when the close button was pressed
    if (event.type == Event::Closed || scene::getCurrentScene() == "main-menu")
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

    // setup framerate counter
    framerateCounter = Text();
    framerateCounter.setFont(*resources::get<Font>("FiraCode-Medium.ttf"));
    framerateCounter.setCharacterSize(24.f);
    framerateCounter.setFillColor(Color::White);
    framerateCounter.setOutlineThickness(2.f);
    framerateCounter.setOutlineColor(Color::Black);
    framerateCounter.setString("0 fps");
    framerateCounter.setOrigin(0.f, 0.f);
    framerateCounter.setPosition(0.f, 0.f);

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

    // setup tilemap
    auto tilemap = tilemap::getTilemap();
    tilemap->setTexture(resources::get<Texture>("testsheet.png"));
    tilemap->setSpriteSize({ 32, 32 });
    tilemap->setTileSize({ 64, 64 });
    tilemap->setDefaultSpriteIndex(1);
    tilemap->setTileSpriteIndex(tilemap::WALL, 2);

    //load saved options
    string line;
    ifstream myfile("save.txt");
    if (myfile.is_open())
    {
        cout << "reading options file" << endl;
        while (getline(myfile, line))
        {
            stringstream lineStream(line);
            string key, valueStr;
            getline(lineStream, key, ','); // get the key before the ,
            getline(lineStream, valueStr); // get the value after
            const auto value = stoi(valueStr); // parse the value to an int

            // apply the option
            if (key == "Fullscreen")
                renderer::setFullscreen(bool(value));
            else if (key == "DisplayFrames")
                displayFramerate = bool(value);
            else if (key == "Controller")
                input::setUseController(bool(value));
            else
                input::bindKey(key, Keyboard::Key(value));
            cout << key << ',' << value << '\n';
        }
        myfile.close();
    }
    else cout << "Unable to open options file" << endl;

	// setup scenes
	scene::add("main-menu", make_shared<MainMenuScene>());
	scene::add("options-menu", make_shared<OptionsScene>());
	scene::add("test-level", make_shared<TestLevelScene>());
    scene::add("game-over", make_shared<GameOverScene>());
	scene::load("main-menu");
}

void update() {
	static Clock clock;
	const auto dt = clock.restart().asSeconds();

    // update framerate counter
    if(displayFramerate) {
        frametimes[++ftc] = dt;
        if (ftc % 60 == 0) {
            double davg = 0;
            for (const auto t : frametimes)
                davg += t;
            davg = 1.0 / (davg / 255.0);
            framerateCounter.setString(to_string(static_cast<int>(davg)) + " fps");
        }
    }

	event::update();
	physics::update(dt);
    scene::update(dt);
    popup::update(dt);
}

void render() {
    scene::render();
    popup::render();

    // display framerate counter
    if (displayFramerate)
        renderer::queue(&framerateCounter, true);

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