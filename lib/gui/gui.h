#pragma once
#include <event-system.h>
#include <SFML/Graphics.hpp>
#include <ecm.h>
#include <memory>

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
}
