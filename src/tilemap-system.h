#pragma once

#include "Tilemap.h"
#include <memory>

namespace tilemap {
    std::shared_ptr<Tilemap> getTilemap();
    void render();
}