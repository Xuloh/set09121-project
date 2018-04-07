#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "Scenes.h"
#include "main.h"
#include "ShapeComponent.h"
#include "physics/PhysicsComponent.h"
#include "PlayerPhysicsComponent.h"
#include "SpriteComponent.h"
#include "SpritesheetAnimatorComponent.h"

using namespace std;
using namespace sf;
using namespace gui;
using namespace ecm;

shared_ptr<Scene> activeScene;
shared_ptr<Scene> mainMenu;
shared_ptr<Scene> testLevel;

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
    testLevel->load();
    activeScene = testLevel;
}

void MainMenuScene::optionsOnClick(const sf::Event& event) {
	cout << "clicked the options button" << endl;
}

// *** TestLevelScene class *** //

void TestLevelScene::load() {
    const auto groundSize = Vector2f(300.f, 50.f);
    auto ground = make_shared<Entity>();
    ground->setOrigin({ .5f, .5f });
    ground->setPosition({ 500.f, -200.f });
    auto groundShape = ground->addComponent<ShapeComponent>();
    groundShape->setShape<RectangleShape>(groundSize);
    auto groundPhysics = ground->addComponent<physics::PhysicsComponent>(false, groundSize);
    groundPhysics->setRestitution(0.f);

    auto circle = make_shared<Entity>();
    /*auto circleShape = circle->addComponent<ShapeComponent>();
    circleShape->setShape<CircleShape>(10.f);
    circleShape->getShape().setFillColor(Color::Red);*/
    circle->setOrigin({ .5f, .5f });
    circle->setPosition({ 550.f, -300.f });
    auto circlePhysics = circle->addComponent<PlayerPhysicsComponent>(Vector2f(64.f, 64.f));
    auto circleSprite = circle->addComponent<SpriteComponent>();
    circleSprite->setSprite();
    auto circleAnimator = circle->addComponent<SpritesheetAnimatorComponent>("res/sprites/PlayerRun.png");
    circleAnimator->setSpriteSize({ 64, 64 });
    circleAnimator->setAnimationTime(.2f);

    entityManager.entities.push_back(ground);
    entityManager.entities.push_back(circle);
}

void TestLevelScene::update(double dt) {
    Scene::update(dt);
}

void TestLevelScene::render() {
    Scene::render();
}

