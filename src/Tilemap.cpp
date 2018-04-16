#include "Tilemap.h"
#include <fstream>
#include <iostream>
#include <physics/physics-system.h>

using namespace std;
using namespace sf;
using namespace tilemap;
using namespace physics;

void Tilemap::load(const string& filePath) {
    size_t width = 0;
    size_t height = 0;

    string buffer;
    ifstream file(filePath);

    // read the whole file into the buffer
    if(file.good()) {
        file.seekg(0, ios::end);
        buffer.resize(file.tellg());
        file.seekg(0);
        file.read(&buffer[0], buffer.size());
        file.close();
    }
    else
        throw string("Couldn't open level file : " + filePath);

    // parse the content of the file
    vector<Tile> tempTiles;
    size_t widthCheck = 0;
    for(unsigned i = 0; i < buffer.size(); i++) {
        const auto c = buffer[i];
        if (c == '\n') {
            if (width == 0)
                width = i;
            else if (width != widthCheck - 1)
                throw string("inconsistent line length at line " + to_string(height) + " in level file : " + filePath);
            widthCheck = 0;
            height++;
        }
        else
            tempTiles.push_back(Tile(c));
        widthCheck++;
    }

    // count the last line if there is no empty line at the end
    if (buffer[buffer.size() - 1] != '\n')
        height++;

    // check that the number of tiles and the width and height are correct
    if (tempTiles.size() != width * height) {
        throw string("An error occured while parsing level file : " + filePath);
    }
    // copy the tiles
    tiles = make_unique<Tile[]>(width * height);
    this->width = width;
    this->height = height;
    copy(tempTiles.begin(), tempTiles.end(), &tiles[0]);
    
    cout << "Level loaded from file " << filePath
         << " with " << width << "x" << height
         << "(" << width * height << ") tiles" << endl;
    
    buildVertices();
    buildBodies();
}

void Tilemap::setTexture(const shared_ptr<Texture> texture) {
    this->texture = texture;
}

void Tilemap::setTileSize(const Vector2f& tileSize) {
    this->tileSize = tileSize;
}

void Tilemap::setSpriteSize(const Vector2u& spriteSize) {
    this->spriteSize = spriteSize;
}

void Tilemap::setTileSpriteIndexMap(const std::unordered_map<Tile, unsigned>& map) {
    tileSpriteIndexMap = map;
}

void Tilemap::setTileSpriteIndex(const Tile tile, const unsigned index) {
    tileSpriteIndexMap[tile] = index;
}

void Tilemap::setDefaultSpriteIndex(const unsigned defaultIndex) {
    defaultSpriteIndex = defaultIndex;
}

Tile Tilemap::getTile(const Vector2u position) const {
    if (position.x > width || position.y > height)
        throw string("Tile out of range : " + to_string(position.x) + ", " + to_string(position.y));
    return tiles[position.y * width + position.x];
}

Vector2f Tilemap::getTilePosition(const Vector2u position) const {
    return { float(position.x) * tileSize.x, float(position.y) * tileSize.y };
}

size_t Tilemap::getWidth() const {
    return width;
}

size_t Tilemap::getHeight() const {
    return height;
}

void Tilemap::draw(RenderTarget& target, RenderStates states) const {
    states.transform *= getTransform();
    states.texture = texture.get();
    target.draw(vertices, states);
}

void Tilemap::buildVertices() {
    // initialise the vertex array
    vertices.clear();
    vertices.setPrimitiveType(Quads);
    vertices.resize(width * height * 4); // there's 4 points for each quad

    for(size_t y = 0; y < height; y++) {
        for(size_t x = 0; x < width; x++) {
            auto tile = getTile({ static_cast<unsigned int>(x), static_cast<unsigned int>(y) });
            
            // get the tile's sprite index, or use the default index
            const auto it = tileSpriteIndexMap.find(tile);
            const auto spriteIndex = it != tileSpriteIndexMap.end() ? it->second : defaultSpriteIndex;
            
            // compute the coordinates of the top left corner of the tile's sprite in the texture
            const auto& textureSize = texture->getSize();
            const auto spriteX = spriteIndex % (textureSize.x / spriteSize.x);
            const auto spriteY = (spriteIndex - spriteX) / (textureSize.x / spriteSize.x);

            const auto quad = &vertices[(y * width + x) * 4]; // get the quad for the current tile
            
            // set the quad's 4 corners
            quad[0].position = { float(x) * tileSize.x, float(y) * tileSize.y };
            quad[1].position = { float(x + 1) * tileSize.x, float(y) * tileSize.y };
            quad[2].position = { float(x + 1) * tileSize.x, float(y + 1) * tileSize.y };
            quad[3].position = { float(x) * tileSize.x, float(y + 1) * tileSize.y };

            // set the quad's 4 texture coordinates
            quad[0].texCoords = { float(spriteX) * spriteSize.x, float(spriteY) * spriteSize.y };
            quad[1].texCoords = { float(spriteX + 1) * spriteSize.x, float(spriteY) * spriteSize.y };
            quad[2].texCoords = { float(spriteX + 1) * spriteSize.x, float(spriteY + 1) * spriteSize.y };
            quad[3].texCoords = { float(spriteX) * spriteSize.x, float(spriteY + 1) * spriteSize.y };
        }
    }
}

void Tilemap::buildBodies() {
    // destroy the old box2d bodies
    auto world = getWorld();
    for(auto& body : bodies)
        world->DestroyBody(body);
    bodies.clear();

    struct TileData {
        Vector2f position;
        Vector2f size;
        Tile tile;
    };

    // gather all the wall tiles
    vector<TileData> tilesData;
    for(unsigned y = 0; y < height; y++) {
        for(unsigned x = 0; x < width; x++) {
            const auto tile = getTile({ x, y });
            if(tile == WALL || tile == FLOOR)
                tilesData.push_back({ getTilePosition({ x, y }), tileSize, tile });
        }
    }

    // merge subsequent tiles horizontaly if they are the same type of tile
    if(!tilesData.empty()) {
        vector<TileData> optimisedTiles;
        auto last = tilesData[0];
        unsigned sameCount = 0;

        for(unsigned i = 0; i < tilesData.size(); i++) {
            const auto same = tilesData[i].position.y == last.position.y &&
                        tilesData[i].position.x == last.position.x + tileSize.x * (1 + sameCount) &&
                        tilesData[i].tile == last.tile;
            if(same)
                sameCount++;
            else {
                if(sameCount)
                    last.size.x = (1 + sameCount) * tileSize.x;
                optimisedTiles.push_back(last);
                sameCount = 0;
                last = tilesData[i];
            }
        }
        if (sameCount) {
            last.size.x = (1 + sameCount) * tileSize.x;
            optimisedTiles.push_back(last);
        }

        const auto xsave = optimisedTiles.size();
        sameCount = 0;
        vector<TileData> reOptimisedTiles;
        for(unsigned i = 0; i < optimisedTiles.size(); i++) {
            last = optimisedTiles[i];
            for(unsigned j = i + 1; j < optimisedTiles.size(); j++) {
                const auto same = optimisedTiles[j].position.x == last.position.x &&
                    optimisedTiles[j].position.y == last.position.y + tileSize.y * (1 + sameCount) &&
                    optimisedTiles[j].size.x == last.size.x &&
                    optimisedTiles[j].tile == last.tile;
                if(same) {
                    sameCount++;
                    optimisedTiles.erase(optimisedTiles.begin() + j);
                    j--;
                }
            }
            if(sameCount)
                last.size.y = (1 + sameCount) * tileSize.y;
            reOptimisedTiles.push_back(last);
            sameCount = 0;
        }

        tilesData.swap(reOptimisedTiles);
    }

    // build the box2d bodies
    for(const auto& tileData : tilesData) {
        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position = Vector2f_To_b2Vec2(invertHeight({ tileData.position.x + tileData.size.x * .5f, tileData.position.y + tileData.size.y * .5f }));

        auto body = world->CreateBody(&bodyDef);

        b2PolygonShape shape;
        shape.SetAsBox(tileData.size.x * scaleInv * .5f, tileData.size.y * scaleInv * .5f);

        b2FixtureDef fixtureDef;
        fixtureDef.friction = .8f;
        fixtureDef.restitution = 0.f;
        fixtureDef.shape = &shape;

        body->CreateFixture(&fixtureDef);
        bodies.push_back(body);
    }
}
