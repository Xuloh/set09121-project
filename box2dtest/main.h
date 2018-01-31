#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Common/b2Math.h>

constexpr int gameWidth = 600;
constexpr int gameHeight = 600;

constexpr float squareSize = 50.f;
constexpr float boxBorderSize = 5.f;

constexpr float boxRotateSpeed = 90.f;

const sf::Vector2f windowCenter(gameWidth * .5f, gameHeight * .5f);

const sf::Keyboard::Key gravityUpKey = sf::Keyboard::Up;
const sf::Keyboard::Key gravityLeftKey = sf::Keyboard::Left;
const sf::Keyboard::Key gravityDownKey = sf::Keyboard::Down;
const sf::Keyboard::Key gravityRightKey = sf::Keyboard::Right;
const sf::Keyboard::Key resetCubes = sf::Keyboard::Space;
const sf::Keyboard::Key rotateBoxLeft = sf::Keyboard::Left;
const sf::Keyboard::Key rotateBoxRight = sf::Keyboard::Right;

const b2Vec2 gravityUp(0.f, 9.81f);
const b2Vec2 gravityLeft(-9.81f, 0.f);
const b2Vec2 gravityDown(0.f, -9.81f);
const b2Vec2 gravityRight(9.81f, 0.f);

constexpr int velocityIterations = 8;
constexpr int positionIterations = 3;
