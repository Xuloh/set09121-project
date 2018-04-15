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
#include <renderer/renderer-system.h>

using namespace std;
using namespace sf;
using namespace gui;
using namespace ecm;
using namespace event;

// *** MainMenuScene class *** //

void MainMenuScene::load() {
    const auto font = resources::get<Font>("FiraCode-Medium.ttf");
	auto guiFactory = GUIFactory({ Color::White, Color::Cyan, font.get(), 32.f });

    // main menu
	auto title = guiFactory.makeLabel("MOIM : Massive Objects Influence Motion");
	title->setOrigin({ 0.5f, 0.5f });

    auto playButton = guiFactory.makeButton("Play", make_shared<eventFunctionType>(
        [](const Event& event) {
            cout << "clicked the play button" << endl;
            scene::load("test-level");
        }
    ));
	playButton->setOrigin({ 0.5f, 0.5f });

    auto optionsButton = guiFactory.makeButton("Options", make_shared<eventFunctionType>(
        [this](const Event& event) {
            cout << "clicked the options button" << endl;
            mainMenuLayout->setAlive(false);
            mainMenuLayout->setVisible(false);

            auto mainLayout = mainMenuLayout->getComponent<LayoutComponent>();
            mainLayout->setItemsAlive(false);
            mainLayout->setItemsVisible(false);

            optionsMenuLayout->setAlive(true);
            optionsMenuLayout->setVisible(true);

            auto optionsLayout = optionsMenuLayout->getComponent<LayoutComponent>();
            optionsLayout->setItemsAlive(true);
            optionsLayout->setItemsVisible(true);
        }
    ));
	optionsButton->setOrigin({ 0.5f, 0.5f });

	auto exitButton = guiFactory.makeButton("Exit", make_shared<eventFunctionType>(&closeWindow));
	exitButton->setOrigin({ 0.5f, 0.5f });

	mainMenuLayout = make_shared<Entity>();
	auto mainLayout = mainMenuLayout->addComponent<LayoutComponent>(1.f, 1.f);
    mainLayout->addItem(title, 0.5f, 0.2f);
    mainLayout->addItem(playButton, 0.5f, 0.4f);
    mainLayout->addItem(optionsButton, 0.5f, 0.6f);
    mainLayout->addItem(exitButton, 0.5f, 0.8f);

    // options menu
    auto options = guiFactory.makeLabel("Options");
    options->setOrigin({ .5f, .5f });

    auto fullscreenButton = guiFactory.makeButton("Fullscreen", make_shared<eventFunctionType>(
        [this](const Event& event) {
            cout << "clicked fullscreen" << endl;
            static auto fullscreen = false;
            auto& window = renderer::getWindow();

            if (!fullscreen) {
                auto fullscreenModes = VideoMode::getFullscreenModes();
                window.create(fullscreenModes[0], "MOIM", Style::Fullscreen);
            }
            else
                window.create(VideoMode::getDesktopMode(), "MOIM");
            fullscreen = !fullscreen;
            renderer::initialise(window);
            mainMenuLayout->getComponent<LayoutComponent>()->updateSize();
            optionsMenuLayout->getComponent<LayoutComponent>()->updateSize();
        }
    ));
    fullscreenButton->setOrigin({ .5f, .5f });

    auto optionsBackButton = guiFactory.makeButton("Back", make_shared<eventFunctionType>(
        [this](const Event& event) {
            cout << "clicked the back button" << endl;
            optionsMenuLayout->setAlive(false);
            optionsMenuLayout->setVisible(false);

            auto optionsLayout = optionsMenuLayout->getComponent<LayoutComponent>();
            optionsLayout->setItemsAlive(false);
            optionsLayout->setItemsVisible(false);

            mainMenuLayout->setAlive(true);
            mainMenuLayout->setVisible(true);

            auto mainLayout = mainMenuLayout->getComponent<LayoutComponent>();
            mainLayout->setItemsAlive(true);
            mainLayout->setItemsVisible(true);
        }
    ));
    optionsBackButton->setOrigin({ .5f, .5f });

    optionsMenuLayout = make_shared<Entity>();
    optionsMenuLayout->setAlive(false);
    optionsMenuLayout->setVisible(false);
    auto optionsLayout = optionsMenuLayout->addComponent<LayoutComponent>(1.f, 1.f);
    optionsLayout->addItem(options, 0.5f, 0.2f);
    optionsLayout->addItem(fullscreenButton, 0.5f, 0.4f);
    optionsLayout->addItem(optionsBackButton, .9f, .9f);
    optionsLayout->setItemsAlive(false);
    optionsLayout->setItemsVisible(false);

	entityManager.entities.push_back(mainMenuLayout);
	entityManager.entities.push_back(title);
	entityManager.entities.push_back(playButton);
	entityManager.entities.push_back(optionsButton);
	entityManager.entities.push_back(exitButton);
    entityManager.entities.push_back(optionsMenuLayout);
    entityManager.entities.push_back(options);
    entityManager.entities.push_back(fullscreenButton);
    entityManager.entities.push_back(optionsBackButton);
}

void MainMenuScene::update(double dt) {
	Scene::update(dt);
}

void MainMenuScene::render() {
	Scene::render();
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
    auto playerPhysics = player->addComponent<PlayerPhysicsComponent>(Vector2f(64.f, 128.f));
    auto playerSprite = player->addComponent<SpriteComponent>();
    playerSprite->setSprite();
    auto playerAnimator = player->addComponent<SpritesheetAnimatorComponent>("res/sprites/PlayerIdle.png");

	playerAnimator->setSpriteSize({ 64, 128 });
    playerAnimator->setAnimationTime(.2f);
	
	//added keyframe variable
	playerAnimator->setKeyFrame(0);

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

