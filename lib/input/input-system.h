#pragma once
#include <string>
#include <SFML/Window.hpp>

namespace input {
	// register a new control name
	void addControl(const std::string& control, sf::Keyboard::Key defaultAzerty, sf::Keyboard::Key defaultQwerty);
	
	// binds the given key to the given control in the custom keymap, the control must be registered with addControl
	// returns true if the key was bound or false if the control was not registered or if the key is already bound to another control
	bool bindKey(const std::string& control, sf::Keyboard::Key key);
	
	// unbind whatever key was bound to control
	void unbindKey(const std::string& control);
	
	// set active keymap to azerty
	void setAzertyActive();

	// set active keymap to qwerty
	void setQwertyActive();

	// set active keymap to custom
	void setCustomActive();

	// determines if the given control is pressed, always returns false when the window does not have the focus
	bool isControlPressed(const std::string& control);

	// returns the key associated with the given control in the active keymap
	sf::Keyboard::Key getKey(const std::string& control);
}
