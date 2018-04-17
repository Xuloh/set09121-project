#pragma once

#include <ecm/ecm.h>
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// A component that handles a spritesheet and changes its sprite at regular intervals
// it basically slices a spritesheet into different sprites based on its sprite size attribute and then regularly
// changes its sprite, it can be given a starting and ending sprite index to only animate part of its spritesheet
// the sliced sprites are indexed from left to right and then from top to bottom
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

	// get/set the key frame
    // this attribute corresponds to the index of the starting sprite
	unsigned getKeyFrame() const;
	void setKeyFrame(unsigned keyFrame);


    // get/set the end frame
    // this attribute corresponds to the index of the ending sprite
	unsigned getEndFrame() const;
	void setEndFrame(unsigned endFrame);

    // get/set the animation time
    // this attribute corresponds to the time between subsequent sprite changes
	float getAnimationTime() const;
	void setAnimationTime(float animationTime);

    // get/set the sprite size
    // this attribute corresponds to the size of a single sprite in the spritesheet and is used to slice it
	const sf::Vector2u& getSpriteSize() const;
	void setSpriteSize(sf::Vector2u spriteSize);
};