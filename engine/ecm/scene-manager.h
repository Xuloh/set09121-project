#pragma once

#include "ecm.h"
#include <string>
#include <memory>

namespace scene {
    // add the given scene with the given name, it can then be loaded using this name
    void add(const std::string& name, const std::shared_ptr<ecm::Scene>& scene);

    // loads the scene associated with the given name and unloads the current scene
    // the operation doesn't take place immediately but on the next call to scene::update
    // to avoid conflicts with event::update
    void load(const std::string& name);

    // get the name of the current scene
    const std::string& getCurrentScene();

    // update the current scene
    void update(double dt);

    // render the current scene
    void render();

    // shutdown the scene manager (all the registered scenes are destroyed)
    void shutdown();
}
