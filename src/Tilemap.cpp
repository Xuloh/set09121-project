#include "Tilemap.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace sf;

using namespace tilemap;

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
        cout << tempTiles.size() << " | " << width << " | " << height << endl;
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
}

void Tilemap::setTexture(const shared_ptr<Texture> texture) {
    this->texture = texture;
}

void Tilemap::setTileSize(const Vector2u& tileSize) {
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

template<typename T> ostream& operator<<(ostream& out, Vector2<T>& vector) {
    return out << "[" << vector.x << "," << vector.y << "]";
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
