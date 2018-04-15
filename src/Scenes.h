#pragma once

#include <engine.h>
#include <memory>

class MainMenuScene : public ecm::Scene {
protected:
    std::shared_ptr<ecm::Entity> mainMenuLayout;
    std::shared_ptr<ecm::Entity> optionsMenuLayout;
    int currentVideoMode = 0;
public:
	MainMenuScene() = default;

	void load() override;
	void update(double dt) override;
	void render() override;
};

class TestLevelScene : public ecm::Scene {
public:
    TestLevelScene() = default;

    void load() override;
    void update(double dt) override;
    void render() override;
};
