#include <string>
#include <unordered_map>
#include <set>
#include <SFML/Window.hpp>
#include <renderer-system.h>
#include "input-system.h"

using namespace std;
using namespace sf;

static set<string> controls;

static unordered_map<string, Keyboard::Key> azertyKeymap;
static unordered_map<string, Keyboard::Key> qwertyKeymap;
static unordered_map<string, Keyboard::Key> customKeymap;

// default keymap is qwerty
static const unordered_map<string, Keyboard::Key>* activeKeymap = &qwertyKeymap;

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

void input::setAzertyActive() {
	activeKeymap = &azertyKeymap;
}

void input::setQwertyActive() {
	activeKeymap = &qwertyKeymap;
}

void input::setCustomActive() {
	activeKeymap = &customKeymap;
}

bool input::isControlPressed(const string& control) {
	return renderer::getWindow().hasFocus() && Keyboard::isKeyPressed(activeKeymap->at(control));
}

Keyboard::Key input::getKey(const string& control) {
	return controls.count(control) ? activeKeymap->at(control) : Keyboard::Unknown;
}
