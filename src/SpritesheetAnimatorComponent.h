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

	//added float for keyframe
	float keyFrame;

    sf::Vector2u spriteSize;

	//set to keyframe
    unsigned currentTextureRect = keyFrame;
    std::vector<sf::IntRect> textureRects;

    void updateTextureRects();

public:
    SpritesheetAnimatorComponent() = delete;
    SpritesheetAnimatorComponent(ecm::Entity* parent, const std::string& texturePath, float animationTime = 1.f);

    ~SpritesheetAnimatorComponent() = default;

    void update(double dt) override;
    void render() override;


	//public get/set methods
	float getKeyFrame() const;
	void setKeyFrame(float keyFrame);

    float getAnimationTime() const;
    void setAnimationTime(float animationTime);

    const sf::Vector2u& getSpriteSize() const;
    void setSpriteSize(sf::Vector2u spriteSize);
};