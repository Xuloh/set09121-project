#pragma once

#include <ecm/ecm.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <vector>

class SpritesheetAnimatorComponent : public ecm::Component {
protected:
    sf::Sprite& sprite;
    std::unique_ptr<sf::Texture> texture;

    float animationTime;
    float timer;

    sf::Vector2u spriteSize;

    unsigned currentTextureRect = 0;
    std::vector<sf::IntRect> textureRects;

    void updateTextureRects();

public:
    SpritesheetAnimatorComponent() = delete;
    SpritesheetAnimatorComponent(ecm::Entity* parent, const std::string& texturePath, float animationTime = 1.f);

    ~SpritesheetAnimatorComponent() = default;

    void update(double dt) override;
    void render() override;

    float getAnimationTime() const;
    void setAnimationTime(float animationTime);

    const sf::Vector2u& getSpriteSize() const;
    void setSpriteSize(sf::Vector2u spriteSize);
};