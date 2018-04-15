#pragma once

#include <SFML/Graphics.hpp>

namespace renderer {
	// return a reference to the renderer's Window
	sf::RenderWindow& getWindow();

    // return a reference to the view used to render the scene
    sf::View& getSceneView();

    // return a reference to the view used to render the GUI
    sf::View& getGUIView();

	// initialise the renderer with the given Window
	void initialise(sf::RenderWindow& window);

    // returns the current resolution of the window
    const sf::VideoMode& getResolution();

    // applies the given resolution to the window
    void setResolution(sf::VideoMode resolution);

    // returns true if the window is in fullscreen or false if not
    bool isFullscreen();

    // sets the fullscreen state of the window
    void setFullscreen(bool isFullscreen);

	// shuts down the renderer
	void shutdown();

	// adds the given Drawable to the render queue
	void queue(const sf::Drawable* drawable, bool forGUI = false);

    // renders all the Drawable that were queued and empties the queue
	void render();
}