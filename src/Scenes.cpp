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
	title->setPosition({ 24, 24 });

	auto playButton = guiFactory.makeButton("Play", &playOnClick);
	playButton->setPosition({ 48, 48 + title->getPosition().y + title->getComponent<TextComponent>()->getText().getLocalBounds().height });

	auto optionsButton = guiFactory.makeButton("Options", &optionsOnClick);
	optionsButton->setPosition({ 48, 48 + playButton->getPosition().y + playButton->getComponent<TextComponent>()->getText().getLocalBounds().height });

	auto exitButton = guiFactory.makeButton("Exit", &closeWindow);
	exitButton->setPosition({ 48, 48 + optionsButton->getPosition().y + optionsButton->getComponent<TextComponent>()->getText().getLocalBounds().height });

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
