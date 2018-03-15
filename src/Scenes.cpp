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
	auto title = make_shared<Entity>();
	title->setPosition({ 24, 24 });

	auto titleText = title->addComponent<TextComponent>();
	titleText->setText();
	titleText->getText().setFont(font);
	titleText->getText().setCharacterSize(24);
	titleText->getText().setFillColor(Color::White);
	titleText->getText().setString("MOIM : Massive Objects Influence Motion");

	auto playButton = make_shared<Entity>();
	playButton->setPosition({48, 48 + title->getPosition().y + titleText->getText().getLocalBounds().height});

	auto playText = playButton->addComponent<TextComponent>();
	playText->setText();
	playText->getText().setFont(font);
	playText->getText().setCharacterSize(24);
	playText->getText().setFillColor(Color::White);
	playText->getText().setString("Play");

	auto playClick = playButton->addComponent<ClickComponent>();
	playClick->onClick = &playOnClick;

	auto playHover = playButton->addComponent<MouseHoverComponent>();
	playHover->baseColor = Color::White;
	playHover->hoverColor = Color::Cyan;

	auto optionsButton = make_shared<Entity>();
	optionsButton->setPosition({ 48, 48 + playButton->getPosition().y + playText->getText().getLocalBounds().height });

	auto optionsText = optionsButton->addComponent<TextComponent>();
	optionsText->setText();
	optionsText->getText().setFont(font);
	optionsText->getText().setCharacterSize(24);
	optionsText->getText().setFillColor(Color::White);
	optionsText->getText().setString("Options");

	auto optionsClick = optionsButton->addComponent<ClickComponent>();
	optionsClick->onClick = &optionsOnClick;

	auto optionsHover = optionsButton->addComponent<MouseHoverComponent>();
	optionsHover->baseColor = Color::White;
	optionsHover->hoverColor = Color::Cyan;

	auto exitButton = make_shared<Entity>();
	exitButton->setPosition({ 48, 48 + optionsButton->getPosition().y + optionsText->getText().getLocalBounds().height });

	auto exitText = exitButton->addComponent<TextComponent>();
	exitText->setText();
	exitText->getText().setFont(font);
	exitText->getText().setCharacterSize(24);
	exitText->getText().setFillColor(Color::White);
	exitText->getText().setString("Exit");

	auto exitClick = exitButton->addComponent<ClickComponent>();
	exitClick->onClick = &closeWindow;

	auto exitHover = exitButton->addComponent<MouseHoverComponent>();
	exitHover->baseColor = Color::White;
	exitHover->hoverColor = Color::Cyan;

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
