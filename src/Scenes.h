#pragma once

#include <engine.h>
#include <memory>
#include <set>
#include <string>
#include <event/event-system.h>

class MainMenuScene : public ecm::Scene, public event::EventHandler {
protected:
    std::shared_ptr<ecm::Entity> mainMenuLayout;
    std::shared_ptr<ecm::Entity> optionsMenuLayout;
    int currentVideoMode = 0;
    std::set<std::string>::iterator currentControl;
    bool keyBind = false;
public:
	MainMenuScene();
    ~MainMenuScene() override;

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
