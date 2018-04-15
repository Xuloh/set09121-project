#include <string>
#include <unordered_map>
#include <set>
#include <SFML/Window.hpp>
#include "renderer/renderer-system.h"
#include "input-system.h"

using namespace std;
using namespace sf;

static set<string> controls;
static unordered_map<string, Keyboard::Key> azertyKeymap;
static unordered_map<string, Keyboard::Key> qwertyKeymap;
static unordered_map<string, Keyboard::Key> customKeymap;
static unordered_map<string, unsigned> joystickButtonsMap;
static unordered_map<string, input::AxisControl> joystickAxisMap;

// default keymap is qwerty
static const unordered_map<string, Keyboard::Key>* activeKeymap = &qwertyKeymap;

static auto useController = false;

void input::addControl(const string& control, const Keyboard::Key defaultAzerty, const Keyboard::Key defaultQwerty) {
	controls.insert(control);
	azertyKeymap[control] = defaultAzerty;
	qwertyKeymap[control] = defaultQwerty;
}

bool input::bindKey(const string& control, const Keyboard::Key key) {
	if(controls.find(control) != controls.end()) {
		for(const auto& pair : customKeymap)
			if (pair.second == key)
				return false; // the key is already bound to another control
		customKeymap[control] = key;
		return true; // the key was bound
	}
	return false; // the control is not registered
}

void input::unbindKey(const string& control) {
	customKeymap.erase(control);
}

bool input::bindJoystickButton(const string& control, const unsigned joystickButton) {
    if (controls.find(control) != controls.end()) {
        for (const auto& pair : joystickButtonsMap)
            if (pair.second == joystickButton)
                return false;
        joystickButtonsMap[control] = joystickButton;
        return true;
    }
    return false;
}

void input::unbindJoystickButton(const string& control) {
    joystickButtonsMap.erase(control);
}

bool input::bindJoystickAxis(const std::string& control, const Joystick::Axis axis, const float min, const float max) {
    if(controls.find(control) != controls.end()) {
        const AxisControl axisControl = { axis, min, max };
        for (const auto& pair : joystickAxisMap)
            if (pair.second == axisControl)
                return false;
        joystickAxisMap[control] = axisControl;
        return true;
    }
    return false;
}

void input::unbindJoystickAxis(const std::string& control) {
    joystickAxisMap.erase(control);
}

void input::setAzertyActive() {
	activeKeymap = &azertyKeymap;
}

void input::setQwertyActive() {
	activeKeymap = &qwertyKeymap;
}

void input::setCustomActive() {
	activeKeymap = &customKeymap;
}

bool input::usingController() {
    return useController;
}

void input::setUseController(const bool useController) {
    if (Joystick::isConnected(0))
        ::useController = useController;
    else
        ::useController = false;
}

bool input::isControlPressed(const string& control) {
    if (controls.count(control) && renderer::getWindow().hasFocus()) {
        if (!useController)
            return Keyboard::isKeyPressed(activeKeymap->at(control));
        if (joystickButtonsMap.count(control))
            return Joystick::isButtonPressed(0, joystickButtonsMap[control]);
        if(joystickAxisMap.count(control)) {
            const auto& axisControl = joystickAxisMap[control];
            const auto axisPosition = Joystick::getAxisPosition(0, axisControl.axis);
            return axisPosition >= axisControl.min && axisPosition <= axisControl.max;
        }
    }
    return false;
}

Keyboard::Key input::getKey(const string& control) {
	return controls.count(control) ? activeKeymap->at(control) : Keyboard::Unknown;
}

unsigned input::getJoystickButton(const string& control) {
    return controls.count(control) ? joystickButtonsMap[control] : Joystick::ButtonCount + 1;
}

bool input::getJoystickAxis(const string& control, AxisControl* axisControl) {
    if(controls.count(control)) {
        *axisControl = joystickAxisMap[control];
        return true;
    }
    return false;
}
