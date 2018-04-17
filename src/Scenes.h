#pragma once

#include <engine.h>
#include <memory>
#include <set>
#include <string>
#include <event/event-system.h>

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

class TestLevelScene : public ecm::Scene {
public:
	TestLevelScene() = default;

	void load() override;
	void update(double dt) override;
	void render() override;
};

class GameOverScene : public ecm::Scene {
protected:
    sf::Sound heartBeatSound;
public:
    GameOverScene() = default;

    void load() override;
    void update(double dt) override;
    void render() override;
};
