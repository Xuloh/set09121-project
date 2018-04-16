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
static unsigned controller = 0;

std::string input::to_string(const Keyboard::Key key) {
    switch(key) {
        case Keyboard::Unknown: return "Unknown";
        case Keyboard::A: return "A";
        case Keyboard::B: return "B";
        case Keyboard::C: return "C";
        case Keyboard::D: return "D";
        case Keyboard::E: return "E";
        case Keyboard::F: return "F";
        case Keyboard::G: return "G";
        case Keyboard::H: return "H";
        case Keyboard::I: return "I";
        case Keyboard::J: return "J";
        case Keyboard::K: return "K";
        case Keyboard::L: return "L";
        case Keyboard::M: return "M";
        case Keyboard::N: return "N";
        case Keyboard::O: return "O";
        case Keyboard::P: return "P";
        case Keyboard::Q: return "Q";
        case Keyboard::R: return "R";
        case Keyboard::S: return "S";
        case Keyboard::T: return "T";
        case Keyboard::U: return "U";
        case Keyboard::V: return "V";
        case Keyboard::W: return "W";
        case Keyboard::X: return "X";
        case Keyboard::Y: return "Y";
        case Keyboard::Z: return "Z";
        case Keyboard::Num0: return "0";
        case Keyboard::Num1: return "1";
        case Keyboard::Num2: return "2";
        case Keyboard::Num3: return "3";
        case Keyboard::Num4: return "Num4";
        case Keyboard::Num5: return "Num5";
        case Keyboard::Num6: return "Num6";
        case Keyboard::Num7: return "Num7";
        case Keyboard::Num8: return "Num8";
        case Keyboard::Num9: return "Num9";
        case Keyboard::Escape: return "Esc";
        case Keyboard::LControl: return "LControl";
        case Keyboard::LShift: return "LShift";
        case Keyboard::LAlt: return "LAlt";
        case Keyboard::LSystem: return "LSystem";
        case Keyboard::RControl: return "RControl";
        case Keyboard::RShift: return "RShift";
        case Keyboard::RAlt: return "RAlt";
        case Keyboard::RSystem: return "RSystem";
        case Keyboard::Menu: return "Menu";
        case Keyboard::LBracket: return "[";
        case Keyboard::RBracket: return "]";
        case Keyboard::SemiColon: return ";";
        case Keyboard::Comma: return ",";
        case Keyboard::Period: return ".";
        case Keyboard::Quote: return "\"";
        case Keyboard::Slash: return "/";
        case Keyboard::BackSlash: return "\\";
        case Keyboard::Tilde: return "~";
        case Keyboard::Equal: return "=";
        case Keyboard::Dash: return "-";
        case Keyboard::Space: return "Space";
        case Keyboard::Return: return "Return";
        case Keyboard::BackSpace: return "Backspace";
        case Keyboard::Tab: return "Tab";
        case Keyboard::PageUp: return "PageUp";
        case Keyboard::PageDown: return "PageDown";
        case Keyboard::End: return "End";
        case Keyboard::Home: return "Home";
        case Keyboard::Insert: return "Insert";
        case Keyboard::Delete: return "Delete";
        case Keyboard::Add: return "+";
        case Keyboard::Subtract: return "-";
        case Keyboard::Multiply: return "*";
        case Keyboard::Divide: return "/";
        case Keyboard::Left: return "Left";
        case Keyboard::Right: return "Right";
        case Keyboard::Up: return "Up";
        case Keyboard::Down: return "Down";
        case Keyboard::Numpad0: return "Num0";
        case Keyboard::Numpad1: return "Num1";
        case Keyboard::Numpad2: return "Num2";
        case Keyboard::Numpad3: return "Num3";
        case Keyboard::Numpad4: return "Num4";
        case Keyboard::Numpad5: return "Num5";
        case Keyboard::Numpad6: return "Num6";
        case Keyboard::Numpad7: return "Num7";
        case Keyboard::Numpad8: return "Num8";
        case Keyboard::Numpad9: return "Num9";
        case Keyboard::F1: return "F1";
        case Keyboard::F2: return "F2";
        case Keyboard::F3: return "F3";
        case Keyboard::F4: return "F4";
        case Keyboard::F5: return "F5";
        case Keyboard::F6: return "F6";
        case Keyboard::F7: return "F7";
        case Keyboard::F8: return "F8";
        case Keyboard::F9: return "F9";
        case Keyboard::F10: return "F10";
        case Keyboard::F11: return "F11";
        case Keyboard::F12: return "F12";
        case Keyboard::F13: return "F13";
        case Keyboard::F14: return "F14";
        case Keyboard::F15: return "F15";
        case Keyboard::Pause: return "Pause";
        case Keyboard::KeyCount: return "";
        default: return "";
    }
}

void input::addControl(const string& control, const Keyboard::Key defaultAzerty, const Keyboard::Key defaultQwerty) {
	controls.insert(control);
	azertyKeymap[control] = defaultAzerty;
	qwertyKeymap[control] = defaultQwerty;
}

const set<string>& input::getControls() {
    return controls;
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
    if (useController) {
        for (unsigned i = 0; i < Joystick::Count; i++)
            if (Joystick::isConnected(i)) {
                ::useController = true;
                controller = i;
                break;
            }
    }
    else
        ::useController = false;
}

bool input::isControlPressed(const string& control) {
    if (controls.count(control) && renderer::getWindow().hasFocus()) {
        if (!useController)
            return Keyboard::isKeyPressed(activeKeymap->at(control));
        if (joystickButtonsMap.count(control))
            return Joystick::isButtonPressed(controller, joystickButtonsMap[control]);
        if(joystickAxisMap.count(control)) {
            const auto& axisControl = joystickAxisMap[control];
            const auto axisPosition = Joystick::getAxisPosition(controller, axisControl.axis);
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
