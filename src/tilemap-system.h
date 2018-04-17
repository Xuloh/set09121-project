#pragma once

#include "Tilemap.h"
#include <memory>

namespace tilemap {
    // return a pointer to the game's tilemap instance
    std::shared_ptr<Tilemap> getTilemap();

    // renders the tilemap
    void render();
}