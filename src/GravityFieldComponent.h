#pragma once

#include <ecm/ecm.h>
#include <Box2D/Box2D.h>
#include <forward_list>

class GravityFieldComponent : public ecm::Component {
protected:
    b2Fixture* field;
    std::forward_list<b2Body*> affectedBodies;
    float force;
    inline bool isContactWithThis(b2Contact* contact) const;
    inline b2Fixture* getOtherFixture(b2Contact* contact) const;
public:
    GravityFieldComponent() = delete;
    explicit GravityFieldComponent(ecm::Entity* parent, float fieldRadius);
    ~GravityFieldComponent() = default;

    void update(double dt) override;
    void render() override;

    void setForce(float force);
    float getForce() const;
};
