#pragma once

#include <engine.h>

class MainMenuScene : public ecm::Scene {
public:
	MainMenuScene() = default;

	void load() override;
	void update(double dt) override;
	void render() override;

	static void playOnClick(const sf::Event& event);
	static void optionsOnClick(const sf::Event& event);
};

class TestLevelScene : public ecm::Scene {
public:
    TestLevelScene() = default;

    void load() override;
    void update(double dt) override;
    void render() override;
};
