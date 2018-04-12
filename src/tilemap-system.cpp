#include "tilemap-system.h"
#include <renderer/renderer-system.h>

using namespace std;
using namespace tilemap;

static auto tilemap_ = make_shared<Tilemap>();

shared_ptr<Tilemap> tilemap::getTilemap() {
    return tilemap_;
}

void tilemap::render() {
    renderer::queue(tilemap_.get());
}