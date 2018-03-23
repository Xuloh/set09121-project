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

	// shuts down the renderer
	void shutdown();

	// adds the given Drawable to the render queue
	void queue(const sf::Drawable* drawable, bool forGUI = false);

    // renders all the Drawable that were queued and empties the queue
	void render();
}