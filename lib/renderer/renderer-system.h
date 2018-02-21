#pragma once

#include <SFML/Graphics.hpp>

namespace renderer {
	// return a reference to the renderer's Window
	sf::RenderWindow& getWindow();

	// initialise the renderer with the given Window
	void initialise(sf::RenderWindow& window);
	// shuts down the renderer
	void shutdown();

	// adds the given Drawable to the render queue
	void queue(const sf::Drawable* drawable);
	// renders all the Drawable that were queued and empties the queue
	void render();
}