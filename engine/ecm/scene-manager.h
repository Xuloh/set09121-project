#pragma once

#include "ecm.h"
#include <string>
#include <memory>

namespace scene {
    void add(const std::string& name, const std::shared_ptr<ecm::Scene>& scene);
    void load(const std::string& name);
    void update(double dt);
    void render();
    void shutdown();
}
