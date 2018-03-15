#pragma once

#include <ecm.h>
#include <memory>

extern std::shared_ptr<ecm::Scene> activeScene;
extern std::shared_ptr<ecm::Scene> mainMenu;

class MainMenuScene : public ecm::Scene {
public:
	MainMenuScene() = default;

	void load() override;
	void update(double dt) override;
	void render() override;

	static void playOnClick(const sf::Event& event);
	static void optionsOnClick(const sf::Event& event);
};
