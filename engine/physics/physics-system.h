#pragma once

#include <Box2D/Box2D.h>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include "renderer/renderer-system.h"

namespace physics {

	// initialise the physics system using the given gravity
	void initialise(b2Vec2 gravity = { 0.f, -9.81f });
	
	// shutdowns the physics system
	void shutdown();

	// update the physics system
	void update(const double& dt);

	// returns a pointer to the Box2D world managed by the physics system
	std::shared_ptr<b2World> getWorld();

    // reset the gravity to its default value (the one that was passed to the initialise function)
    void resetGravity();

	// the scale used to convert meters to pixels
	const float scale = 50.f;

	// the scale used to convert pixels to meters
	const float scaleInv = 1.f / scale;

	// Converts Box2D coordinates to SFML coordinates
	// it applies the physics scale and inverts the y coordinate
	//
	// Note : this function's name breaks the naming convention for readability purposes
	// ReSharper disable once CppInconsistentNaming
	inline sf::Vector2f b2Vec2_To_Vector2f(const b2Vec2& vector, bool applyScale = true) {
        if (applyScale)
            return { vector.x * scale, vector.y * scale };
        return { vector.x, vector.y };
	}

	// Converts SFML coordinates to Box2D coordinates
	// it applies the inverse physics scale and inverts the y coordinate
	//
	// Note : this function's name breaks the naming convention for readability purposes
	// ReSharper disable once CppInconsistentNaming
	inline b2Vec2 Vector2f_To_b2Vec2(const sf::Vector2f vector, bool applyScale = true) {
        if(applyScale)
		    return { vector.x * scaleInv, vector.y * scaleInv };
        return { vector.x, vector.y };
	}

    // Inverts the y property of the given vector (mostly used when converting from Box2D coordinates to SFML coordinates)
    inline sf::Vector2f invertHeight(sf::Vector2f vector) {
        return { vector.x, -vector.y };
	}

	// Converts radians (used within Box2D) to degrees (used within SFML)
	inline float radiansToDegrees(const float radians) {
		return -(radians * 180.f / b2_pi);
	}

	// Converts degrees (used within SFML) to radians (used within Box2D)
	inline float degreesToRadians(const float degrees) {
		return -(degrees * b2_pi / 180.f);
	}
}
