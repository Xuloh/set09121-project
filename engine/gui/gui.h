#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "event/event-system.h"
#include "ecm/ecm.h"

// regroups a bunch of Components that can be used to create simple GUIs
namespace gui {

	// a struct that holds values to be used by a GUIFactory
	struct GUISettings {
		sf::Color baseColor;
		sf::Color hoverColor;
		sf::Font* font;
		float characterSize;
	};

	// a class to make the creation of GUI elements easier
	class GUIFactory {
	public:
		GUISettings settings;

		GUIFactory() = delete;
		GUIFactory(GUISettings settings);

		~GUIFactory() = default;

		std::shared_ptr<ecm::Entity> makeLabel(sf::String text) const;
		std::shared_ptr<ecm::Entity> makeButton(sf::String text, event::eventFunction onClickHandler) const;
	};

	// a Component that handles a single sf::Text
	class TextComponent : public ecm::Component {
	protected:
		std::shared_ptr<sf::Text> text;
	public:
		
		TextComponent() = delete;
		explicit TextComponent(ecm::Entity* parent);
		
		~TextComponent() = default;

		void update(double dt) override;
		void render() override;

		sf::Text& getText() const;

		template<typename... Targs> void setText(Targs... params) {
			text.reset(new sf::Text(params...));
		}
	};

	// a Component that reacts to clicks on its sibling TextComponent (must be bound to an Entity that has a TextComponent)
	class ClickComponent : public ecm::Component, public event::EventHandler {
	protected:
		sf::Text& targetText;
	public:
		event::eventFunction onClick;

		ClickComponent() = delete;
		explicit ClickComponent(ecm::Entity* parent);
		
		~ClickComponent();

		void update(double dt) override;
		void render() override;

		void handleEvent(const sf::Event& event) override;
	};

	// a Component that reacts to mouse movements over its sibling TextComponent (must be bound to an Entity that has a TextComponent)
	class MouseHoverComponent : public ecm::Component, public event::EventHandler {
	protected:
		sf::Text& targetText;
		bool isMouseInArea;
	public:
		sf::Color baseColor;
		sf::Color hoverColor;

		MouseHoverComponent() = delete;
		explicit MouseHoverComponent(ecm::Entity* parent);

		~MouseHoverComponent();

		void update(double dt) override;
		void render() override;

		void handleEvent(const sf::Event& event) override;
	};

	// associates an Entity with its position within a LayoutComponent
	struct LayoutItem {
		std::shared_ptr<ecm::Entity> entity;
		// the entity's horizontal position relative the layout's width
		float x;
		// the entity's vertical position relative to the layout's height
		float y;
	};

	// a simple layout system, this component can contain other entities and position them relative to its own height and width
	class LayoutComponent : public ecm::Component, public event::EventHandler {
	protected:
		std::vector<LayoutItem> items;

		float width;
		float height;

		bool updateLayout;

		void updateSize();
	public:
		// the layout's width relative to the window's width (ex: 0.5 is half the window's width)
		float x;
		// the layout's height relative to the window's height (ex: 0.5 is half the window's height)
		float y;

		LayoutComponent() = delete;
		explicit LayoutComponent(ecm::Entity* parent);
		explicit LayoutComponent(ecm::Entity* parent, float x, float y);

		~LayoutComponent();

		void update(double dt) override;
		void render() override;

		void handleEvent(const sf::Event& event) override;

		void addItem(std::shared_ptr<ecm::Entity> entity, float x, float y);
	};
}
