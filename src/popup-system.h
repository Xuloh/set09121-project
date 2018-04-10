#pragma once

#include <gui/gui.h>
#include <string>

namespace popup {
    // initialise the popup system
    void initialise();
    
    // shutdown the popup system
    void shutdown();

    // display the given message in the popup, optionaly specifying the duration of the popup and its title
    void popup(const std::string& message, float time = 2.f, const std::string& title = "");
    
    // update the popup system
    void update(double dt);

    // render the popup (if the popup is not active it isn't displayed)
    void render();
}
