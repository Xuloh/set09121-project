#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <Box2D/Box2D.h>

namespace tilemap {
    // the different kinds of tiles
    enum Tile {
        EMPTY = ' ',
        START = 's',
        END = 'e',
        WALL = 'w',
        ENEMY = 'x'
    };

    // A subclass of sf::Drawable and sf::Transformable that represents a tile map
    // it can load a map from a text file and render it to a sf::RenderTarget
    class Tilemap : public sf::Drawable, public sf::Transformable {
    public:
        // load a map from the given file path
        void load(const std::string& filePath);

        // set the texture to use as a tile set
        void setTexture(std::shared_ptr<sf::Texture> texture);

        // set the size of the tiles on screen
        void setTileSize(const sf::Vector2f& tileSize);

        // set the size of a single sprite in the tile set
        void setSpriteSize(const sf::Vector2u& spriteSize);

        // set the map that will be used to map a Tile to the index of its sprite in the tile set
        void setTileSpriteIndexMap(const std::unordered_map<Tile, unsigned>& map);

        // set the index of the sprite in the tile set for the given Tile to the given value
        void setTileSpriteIndex(Tile tile, unsigned index);

        // set the default sprite index in the tile set
        // it will be used for Tiles that don't have a specific sprite index
        void setDefaultSpriteIndex(unsigned defaultIndex);

        // get the tile at the given position in the map
        Tile getTile(sf::Vector2u position) const;

        // return the position of the given tile in the world
        sf::Vector2f getTilePosition(sf::Vector2u position) const;

        size_t getWidth() const;
        size_t getHeight() const;

    private:
        // properties used for rendering
        sf::VertexArray vertices;
        std::shared_ptr<sf::Texture> texture;
        sf::Vector2f tileSize;
        sf::Vector2u spriteSize;

        // properties of the actual tile map
        std::unique_ptr<Tile[]> tiles;
        size_t width = 0;
        size_t height = 0;

        // map to associate a tile and a sprite index
        std::unordered_map<Tile, unsigned> tileSpriteIndexMap;
        unsigned defaultSpriteIndex = 0;

        std::vector<b2Body*> bodies;

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void buildVertices();
        void buildBodies();
    };
}