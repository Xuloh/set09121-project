#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <SFML/System.hpp>

#define M_PI 3.14159265358979323846

namespace sf {
	typedef Vector2<size_t> Vector2ul;

	// returns the length of a sf::Vector2
	template<typename T> double length(const Vector2<T>& v) {
		return sqrt(pow(v.x, 2) + pow(v.y, 2));
	}

	// returns a normalised sf::Vector2
	template<typename T> Vector2<T> normalize(const Vector2<T> &v) {
		double l = length(v);
		return l != 0 ? Vector2<T>(v.x / l, v.y / l) : Vector2<T>(0, 0);
	}

	// casts a given sf::Vector2 to another type of sf::Vector2
	template<typename T, typename U> Vector2<T> vectorCast(const Vector2<U> &v) {
		return Vector2<T>(static_cast<T>(v.x), static_cast<T>(v.y));
	}

	// converts degrees to radians
	static double deg2rad(double degrees) {
		return degrees * M_PI / 180.0;
	}

	// rotates a sf::Vector2 by a given angle (in degrees)
	template<typename T> Vector2<T> rotate(const Vector2<T> &v, const double degrees) {
		const double theta = deg2rad(degrees);
		const double cs = cos(theta);
		const double sn = sin(theta);
		return { (T)(v.x * cs - v.y * sn), (T)(v.x * sn + v.y * cs) };
	}

	template<typename T> std::ostream &operator<<(std::ostream &os, const Vector2<T> &v) {
		os << '(' << v.x << ", " << v.y << ')';
		return os;
	}
}
