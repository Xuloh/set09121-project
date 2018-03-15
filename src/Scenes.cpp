#include <SFML/Graphics.hpp>
#include <gui.h>
#include <ecm.h>
#include <memory>
#include <iostream>
#include "Scenes.h"
#include "main.h"

using namespace std;
using namespace sf;
using namespace gui;
using namespace ecm;

shared_ptr<Scene> activeScene;
shared_ptr<Scene> mainMenu;

// *** MainMenuScene class *** //

void MainMenuScene::load() {
	auto guiFactory = GUIFactory({ Color::White, Color::Cyan, &font, 32.f });

	auto title = guiFactory.makeLabel("MOIM : Massive Objects Influence Motion");
	title->setOrigin({ 0.5f, 0.5f });

	auto playButton = guiFactory.makeButton("Play", &playOnClick);
	playButton->setOrigin({ 0.5f, 0.5f });

	auto optionsButton = guiFactory.makeButton("Options", &optionsOnClick);
	optionsButton->setOrigin({ 0.5f, 0.5f });

	auto exitButton = guiFactory.makeButton("Exit", &closeWindow);
	exitButton->setOrigin({ 0.5f, 0.5f });

	auto layoutContainer = make_shared<Entity>();
	auto layout = layoutContainer->addComponent<LayoutComponent>(1.f, 1.f);
	layout->addItem(title, 0.5f, 0.2f);
	layout->addItem(playButton, 0.5f, 0.4f);
	layout->addItem(optionsButton, 0.5f, 0.6f);
	layout->addItem(exitButton, 0.5f, 0.8f);

	entityManager.entities.push_back(layoutContainer);
	entityManager.entities.push_back(title);
	entityManager.entities.push_back(playButton);
	entityManager.entities.push_back(optionsButton);
	entityManager.entities.push_back(exitButton);
}

void MainMenuScene::update(double dt) {
	Scene::update(dt);
}

void MainMenuScene::render() {
	Scene::render();
}

void MainMenuScene::playOnClick(const sf::Event& event) {
	cout << "clicked the play button" << endl;
}

void MainMenuScene::optionsOnClick(const sf::Event& event) {
	cout << "clicked the options button" << endl;
}
