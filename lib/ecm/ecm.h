#pragma once
#include <memory>
#include <vector>
#include <SFML/System.hpp>

namespace ecm {
	class Component;
	class Entity;
	class EntityManager;
	class Scene;
}

class ecm::Entity {
protected:
	std::vector<std::shared_ptr<Component>> components;

	sf::Vector2f position;
	sf::Vector2f origin;
	float rotation;

	bool alive; // whether the Entity should be updated
	bool visible; // whether the Entity should be rendered
	bool forDeletion; // whether the Entity should be deleted

public:
	Entity();
	virtual ~Entity();

	// update all the Entity's Components and delete those marked for deletion
	virtual void update(double dt);
	// render all the Entity's Components
	virtual void render();

	const sf::Vector2f& getPosition() const;
	void setPosition(const sf::Vector2f& position);

	const sf::Vector2f& getOrigin() const;
	void setOrigin(const sf::Vector2f& origin);

	float getRotation() const;
	void setRotation(float rotation);

	bool isAlive() const;
	void setAlive(bool alive);

	bool isVisible() const;
	void setVisible(bool visible);

	bool isForDeletion() const;
	void setForDeletion();

	// adds a Component of the given type to the Entity, passing any given parameters to the Component's constructor
	template<typename T, typename... Targs> std::shared_ptr<T> addComponent(Targs... params) {
		static_assert(std::is_base_of<Component, T>::value, "given type is not a Component");
		std::shared_ptr<T> component = std::make_shared<T>(this, params...);
		components.push_back(component);
		return component;
	}

	// returns a pointer to the first Component of the given type that was found, or an empty pointer if none was found
	template<typename T> std::shared_ptr<T> getComponent() {
		static_assert(std::is_base_of<Component, T>::value, "given type is not a Component");
		for (const auto& component : components) {
			std::shared_ptr<T> castComponent = std::dynamic_pointer_cast<T>(component);
			if (castComponent && castComponent.use_count() != 0)
				return castComponent;
		}
		return std::shared_ptr<T>(nullptr);
	}

	// returns a vector of pointers to all the Components of the given type, or an empty vector if none were found
	template<typename T> std::vector<std::shared_ptr<T>> getAllComponents() {
		static_assert(std::is_base_of<Component, T>::value, "given type is not a Component");
		std::vector<std::shared_ptr<T>> components;
		for (const auto& component : this->components) {
			std::shared_ptr<T> castComponent = std::dynamic_pointer_cast<T>(component);
			if (castComponent && castComponent.use_count() != 0)
				components.push_back(castComponent);
		}
		return components;
	}
};

class ecm::Component {
protected:
	Entity * const parent;
	bool forDeletion; // should be deleted
	explicit Component(Entity *const parent);

public:
	Component() = delete;
	virtual ~Component();

	bool isForDeletion() const;
	void setForDeletion();

	virtual void update(double dt) = 0;
	virtual void render() = 0;
};

class ecm::EntityManager {
public:
	std::vector<std::shared_ptr<Entity>> entities;

	// update all the Manager's Entities that are alive and delete those marked for deletion
	void update(double dt);
	// render all the Manager's Entities that are visible
	void render() const;
};

class ecm::Scene {
protected:
	EntityManager entityManager;

public:
	Scene();
	virtual ~Scene();

	virtual void load() = 0;
	virtual void update(double dt);
	virtual void render();

	std::vector<std::shared_ptr<Entity>> &getEntities();
};
