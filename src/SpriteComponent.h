#pragma once

#include <ecm/ecm.h>
#include <SFML/Graphics.hpp>
#include <memory>

// a Component that handles a single sf::Shape
class SpriteComponent : public ecm::Component {
protected:
    std::shared_ptr<sf::Sprite> sprite;
public:

    SpriteComponent() = delete;
    explicit SpriteComponent(ecm::Entity* parent);

    ~SpriteComponent() = default;

    void update(double dt) override;
    void render() override;

    sf::Sprite& getSprite() const;

    template<typename... Targs> void setSprite(Targs... params) {
        sprite.reset(new sf::Sprite(params...));
    }
};