#include "SpritesheetAnimatorComponent.h"
#include "SpriteComponent.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace std;
using namespace sf;
using namespace ecm;

SpritesheetAnimatorComponent::SpritesheetAnimatorComponent(Entity* parent, const std::string& texturePath, const float animationTime) : Component(parent), sprite(this->parent->getComponent<SpriteComponent>()->getSprite()) {
    texture = make_unique<Texture>();

    if(!texture->loadFromFile(texturePath))
        cout << "could not load texture : " << texturePath << endl;

    sprite.setTexture(*texture);
    spriteSize = texture->getSize();
    currentTextureRect = 0;
	textureRects.emplace_back(0, 0, spriteSize.x, spriteSize.y);

    this->animationTime = animationTime;
    timer = 0.f;
}

void SpritesheetAnimatorComponent::updateTextureRects() {
    textureRects.clear();
    const auto& textureSize = texture->getSize();
	for (unsigned j = 0; j <= textureSize.y - spriteSize.y; j += spriteSize.y) {
		for (unsigned i = 0; i <= textureSize.x - spriteSize.x; i += spriteSize.x) {
            textureRects.emplace_back(i, j, spriteSize.x, spriteSize.y);
        }
    }
}

void SpritesheetAnimatorComponent::update(const double dt) {
    timer -= float(dt);
    if (timer <= 0.f) {
        sprite.setTextureRect(textureRects[currentTextureRect]);
        currentTextureRect = (currentTextureRect + 1) % textureRects.size();
        timer = animationTime;
    }
}

void SpritesheetAnimatorComponent::render() {}


//keyframe getter 
float SpritesheetAnimatorComponent::getKeyFrame() const
{
	return keyFrame;
}

//keyframe setter
void SpritesheetAnimatorComponent::setKeyFrame(float keyFrame)
{
	this->keyFrame = keyFrame;
}

//endframe getter 
float SpritesheetAnimatorComponent::getEndFrame() const
{
	return endFrame;
}

//endframe setter
void SpritesheetAnimatorComponent::setEndFrame(float endFrame)
{
	this->endFrame = endFrame;
}


float SpritesheetAnimatorComponent::getAnimationTime() const {
    return animationTime;
}

void SpritesheetAnimatorComponent::setAnimationTime(float animationTime) {
    this->animationTime = animationTime;
}

const Vector2u& SpritesheetAnimatorComponent::getSpriteSize() const {
    return spriteSize;
}

void SpritesheetAnimatorComponent::setSpriteSize(const Vector2u spriteSize) {
    this->spriteSize = spriteSize;
    updateTextureRects();
}

