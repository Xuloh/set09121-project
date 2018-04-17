#pragma once

#include <engine.h>
#include <memory>
#include <set>
#include <string>
#include <event/event-system.h>

// the main menu, with a button to start the game, a button to access the options menu and a button to exit the game
class MainMenuScene : public ecm::Scene {
protected:
	std::shared_ptr<ecm::Entity> mainMenuLayout;
public:
	MainMenuScene() = default;
	~MainMenuScene() = default;
	//git changes
	void load() override;
	void update(double dt) override;
	void render() override;
};

// the options menu, with all the controls needed to change the game's options and a button to go back to the main menu
class OptionsScene : public ecm::Scene, public event::EventHandler {
protected:
	std::shared_ptr<ecm::Entity> optionsMenuLayout;
	int currentVideoMode = 0;
	std::set<std::string>::iterator currentControl;
	bool keyBind = false;
public:
	OptionsScene();
	~OptionsScene() override;

	void load() override;
	void update(double dt) override;
	void render() override;
	void handleEvent(const sf::Event& event) override;
};

// a scene used to test gameplay elements
class TestLevelScene : public ecm::Scene {
public:
	TestLevelScene() = default;

	void load() override;
	void update(double dt) override;
	void render() override;
};

// the game over screen, plays a slow heart beat sound when loaded to signify the death of the player
class GameOverScene : public ecm::Scene {
protected:
    sf::Sound heartBeatSound;
public:
    GameOverScene() = default;

    void load() override;
    void update(double dt) override;
    void render() override;
};
