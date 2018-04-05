#pragma once

#include <ecm/ecm.h>
#include <SFML/Graphics.hpp>
#include <memory>

// a Component that handles a single sf::Shape
class ShapeComponent : public ecm::Component {
protected:
    std::shared_ptr<sf::Shape> shape;
public:

    ShapeComponent() = delete;
    explicit ShapeComponent(ecm::Entity* parent);

    ~ShapeComponent() = default;

    void update(double dt) override;
    void render() override;

    sf::Shape& getShape() const;

    template<typename T, typename... Targs> void setShape(Targs... params) {
        static_assert(std::is_base_of<sf::Shape, T>::value, "must be a shape");
        shape.reset(new T(params...));
    }
};