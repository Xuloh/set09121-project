#pragma once
#include <string>
#include <set>
#include <SFML/Window.hpp>

namespace input {

    // An axis control associates an axis with a minimum and maximum position
    struct AxisControl {
        sf::Joystick::Axis axis;
        float min;
        float max;
    };

    // test the equality of 2 AxisControls
    inline bool operator ==(const AxisControl& a, const AxisControl& b) {
        return a.axis == b.axis && a.min == b.min && a.max == b.max;
    }

	// register a new control name
	void addControl(const std::string& control, sf::Keyboard::Key defaultAzerty, sf::Keyboard::Key defaultQwerty);
	
    // get the list of registered controls
    const std::set<std::string>& getControls();

	// binds the given key to the given control in the custom keymap, the control must be registered with addControl
	// returns true if the key was bound or false if the control was not registered or if the key is already bound to another control
	bool bindKey(const std::string& control, sf::Keyboard::Key key);
	
	// unbind whatever key was bound to control
	void unbindKey(const std::string& control);
	
    // binds the given control in the joystick buttons map, the control must be registered with addControl
    // returns true if the button was bound or false if the control was not registered or if the button is already bound to another control
    bool bindJoystickButton(const std::string& control, unsigned joystickButton);

    // unbind whatever joystick button was bound to control
    void unbindJoystickButton(const std::string& control);

    // binds the given control in the joystick axis map, the control must be registered with addControl
    // returns true if the axis was bound or false if the control was not registered or if the axis is already bound to another control with the same min and max values
    bool bindJoystickAxis(const std::string& control, sf::Joystick::Axis axis, float min, float max);

    // unbind whatever joystick axis was bound to control
    void unbindJoystickAxis(const std::string& control);

	// set active keymap to azerty
	void setAzertyActive();

	// set active keymap to qwerty
	void setQwertyActive();

	// set active keymap to custom
	void setCustomActive();

    // returns true if currently using the controller as input or false if not
    bool usingController();

    // set wether the controller should be used as input or not
    void setUseController(bool useController);

	// determines if the given control is pressed
	// returns false when the window does not have the focus
	// also returns false if the given control does not exist
    // if input is configured to use a controller, this function will check the state of the controller instead of the state of the keyboard
	bool isControlPressed(const std::string& control);

	// returns the key associated with the given control in the active keymap
	// returns the unknown key sf::Keyboard::Unknown if the given control does not exist
	sf::Keyboard::Key getKey(const std::string& control);

    // return the joystick button associated with the given control
    // return sf::Joystick::ButtonCount + 1 if the given control does not exist
    unsigned getJoystickButton(const std::string& control);

    // assigns the axis control associated with the given control to the given axis control pointer
    // returns true if a value was actually assigned or returns false if the given control does not exist
    bool getJoystickAxis(const std::string& control, AxisControl* axisControl);
}
