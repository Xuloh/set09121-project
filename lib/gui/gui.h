#pragma once
#include <event-system.h>
#include <SFML/Graphics.hpp>

namespace gui {
	class GUIElement : public event::EventHandler {
	protected:
		bool mouseInArea = false;
	public:
		std::shared_ptr<sf::FloatRect> hitArea;

		explicit GUIElement(const sf::FloatRect& hitArea);
		GUIElement() = default;
		~GUIElement() = default;

		virtual void render() = 0;

		virtual void onMouseEnter(const sf::Event& event) = 0;
		virtual void onMouseExit(const sf::Event& event) = 0;
		virtual void onClick(const sf::Event& event) = 0;
		void handleEvent(const sf::Event& event) override;
	};

	class Label : public GUIElement {
	public:
		std::shared_ptr<sf::Text> text;
		
		explicit Label(const sf::FloatRect& hitArea);
		Label() = default;
		~Label() = default;

		void render() override;

		void onMouseEnter(const sf::Event& event) override;
		void onMouseExit(const sf::Event& event) override;
		void onClick(const sf::Event& event) override;
		void handleEvent(const sf::Event& event) override;
	};

	class Button : public GUIElement {
	public:
		std::shared_ptr<sf::Color> baseColor;
		std::shared_ptr<sf::Color> hoverColor;
		
		std::shared_ptr<sf::Text> text;

		event::eventFunction onClickHandler = nullptr;

		explicit Button(const sf::FloatRect& hitArea);
		Button() = default;
		~Button();

		void render() override;
		void onMouseEnter(const sf::Event& event) override;
		void onMouseExit(const sf::Event& event) override;
		void onClick(const sf::Event& event) override;
	};

	class GUIFactory {
	public:
		std::shared_ptr<sf::Font> font;
		std::shared_ptr<sf::Color> baseColor;
		std::shared_ptr<sf::Color> hoverColor;
		float baseCharacterSize;

		explicit GUIFactory(float baseCharacterSize);
		~GUIFactory() = default;

		std::shared_ptr<Label> makeLabel(const std::string& text, const sf::Vector2f& position, float characterSize = 1.f) const;
		std::shared_ptr<Button> makeButton(const std::string& text, const sf::Vector2f& position, event::eventFunction onClickHandler = nullptr, float characterSize = 1.f) const;
	};
}
