#include "resources-manager.h"

using namespace std;
using namespace sf;

template<> shared_ptr<Font> resources::load(const string& name) {
    auto font = make_shared<Font>();
	//uncomment for debug
	//if (!font->loadFromFile("debug/res/fonts/" + name))
    if (!font->loadFromFile("res/fonts/" + name))
        throw string("Could not load font : " + name);
    return font;
}

template<> shared_ptr<Texture> resources::load(const string& name) {
    auto texture = make_shared<Texture>();
	//Uncomment for debug
	//if (!texture->loadFromFile("debug/res/sprites" + name))
    if (!texture->loadFromFile("res/sprites" + name))
        throw string("Could not load sprite : " + name);
    return texture;
}
