#pragma once

#include <ecm/ecm.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <vector>

class SpritesheetAnimatorComponent : public ecm::Component {
protected:
	sf::Sprite& sprite;
	std::shared_ptr<sf::Texture> texture;

	float animationTime;
	float timer;

	//added floats for keyframe / endframe
	unsigned keyFrame;
	unsigned endFrame;

	sf::Vector2u spriteSize;

	unsigned currentTextureRect;

	std::vector<sf::IntRect> textureRects;

	void updateTextureRects();

public:
	SpritesheetAnimatorComponent() = delete;
	SpritesheetAnimatorComponent(ecm::Entity* parent, std::shared_ptr<sf::Texture> texture, float animationTime = 1.f);

	~SpritesheetAnimatorComponent() = default;

	void update(double dt) override;
	void render() override;

	//public key frame get/set methods
	unsigned getKeyFrame() const;
	void setKeyFrame(unsigned keyFrame);


	//public end frame get/set methods
	unsigned getEndFrame() const;
	void setEndFrame(unsigned endFrame);

	float getAnimationTime() const;
	void setAnimationTime(float animationTime);

	const sf::Vector2u& getSpriteSize() const;
	void setSpriteSize(sf::Vector2u spriteSize);
};