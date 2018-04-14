#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <resources/resources-manager.h>
#include <ecm/scene-manager.h>
#include "Scenes.h"
#include "main.h"
#include "ShapeComponent.h"
#include "physics/PhysicsComponent.h"
#include "PlayerPhysicsComponent.h"
#include "SpriteComponent.h"
#include "SpritesheetAnimatorComponent.h"
#include "GravityFieldComponent.h"
#include "RotateGravityComponent.h"

using namespace std;
using namespace sf;
using namespace gui;
using namespace ecm;

// *** MainMenuScene class *** //

void MainMenuScene::load() {
    const auto font = resources::get<Font>("FiraCode-Medium.ttf");
	auto guiFactory = GUIFactory({ Color::White, Color::Cyan, font.get(), 32.f });

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
    scene::load("test-level");
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

    auto ceil = make_shared<Entity>();
    ceil->setOrigin({ .5f, .5f });
    ceil->setPosition({ 500.f, -500.f });
    auto ceilShape = ceil->addComponent<ShapeComponent>();
    ceilShape->setShape<RectangleShape>(groundSize);
    ceilShape->getShape().setFillColor(Color::Magenta);
    auto ceilPhysics = ceil->addComponent<physics::PhysicsComponent>(false, groundSize);
    ceilPhysics->setRestitution(0.f);

    const auto wallSize = Vector2f(50.f, 300.f);
    auto wallL = make_shared<Entity>();
    wallL->setOrigin({ .5f, .5f });
    wallL->setPosition({ 350.f, -350.f });
    auto wallLShape = wallL->addComponent<ShapeComponent>();
    wallLShape->setShape<RectangleShape>(wallSize);
    wallLShape->getShape().setFillColor(Color::Cyan);
    auto wallLPhysics = wallL->addComponent<physics::PhysicsComponent>(false, wallSize);
    wallLPhysics->setRestitution(0.f);

    auto wallR = make_shared<Entity>();
    wallR->setOrigin({ .5f, .5f });
    wallR->setPosition({ 650.f, -350.f });
    auto wallRShape = wallR->addComponent<ShapeComponent>();
    wallRShape->setShape<RectangleShape>(wallSize);
    wallRShape->getShape().setFillColor(Color::Yellow);
    auto wallRPhysics = wallR->addComponent<physics::PhysicsComponent>(false, wallSize);
    wallRPhysics->setRestitution(0.f);

    auto player = make_shared<Entity>();
    player->setOrigin({ .5f, .5f });
    player->setPosition({ 550.f, -300.f });
    auto playerPhysics = player->addComponent<PlayerPhysicsComponent>(Vector2f(64.f, 64.f));
    auto playerSprite = player->addComponent<SpriteComponent>();
    playerSprite->setSprite();
    auto playerAnimator = player->addComponent<SpritesheetAnimatorComponent>("res/sprites/PlayerRun.png");
    playerAnimator->setSpriteSize({ 64, 64 });
    playerAnimator->setAnimationTime(.2f);
    auto playerField = player->addComponent<GravityFieldComponent>(20.f);
    playerField->setForce(20.f);
    auto gravityRotate = player->addComponent<RotateGravityComponent>();

    auto box = make_shared<Entity>();
    box->setOrigin({ .5f, .5f });
    box->setPosition({ 400.f, -300.f });
    auto boxPhysics = box->addComponent<physics::PhysicsComponent>(true, Vector2f(32.f, 32.f));
    auto boxShape = box->addComponent<ShapeComponent>();
    boxShape->setShape<RectangleShape>(Vector2f(32.f, 32.f));

    entityManager.entities.push_back(ground);
    entityManager.entities.push_back(wallL);
    entityManager.entities.push_back(wallR);
    entityManager.entities.push_back(ceil);
    entityManager.entities.push_back(player);
    entityManager.entities.push_back(box);
}

void TestLevelScene::update(double dt) {
    Scene::update(dt);
}

void TestLevelScene::render() {
    Scene::render();
}

