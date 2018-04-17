#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <iostream>
#include <resources/resources-manager.h>
#include <ecm/scene-manager.h>
#include "Scenes.h"
#include "main.h"
#include "ShapeComponent.h"
#include "PlayerPhysicsComponent.h"
#include "SpriteComponent.h"
#include "SpritesheetAnimatorComponent.h"
#include "GravityFieldComponent.h"
#include "RotateGravityComponent.h"
#include <renderer/renderer-system.h>
#include "tilemap-system.h"
#include "OxygenTimerComponent.h"

using namespace std;
using namespace sf;
using namespace gui;
using namespace ecm;
using namespace event;

// *** MainMenuScene class *** //

//git changes

void MainMenuScene::load() {
	const auto font = resources::get<Font>("Xolonium-Bold.ttf");
	const GUISettings settings = { Color::White, Color::Cyan, font.get(), 32.f };
	auto guiFactory = GUIFactory(settings);

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
		[](const Event& event) {
	        cout << "clicked the options button" << endl;
	        scene::load("options-menu");
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


	entityManager.entities.push_back(mainMenuLayout);
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


// *** options scene *** ///
OptionsScene::OptionsScene() {
	registerHandler(Event::KeyPressed, this);
}

OptionsScene::~OptionsScene() {
	unregisterHandler(Event::KeyPressed, this);
	Scene::~Scene();
}

void OptionsScene::load() {
	const auto font = resources::get<Font>("Xolonium-Bold.ttf");
	const GUISettings settings = { Color::White, Color::Cyan, font.get(), 32.f };
	auto guiFactory = GUIFactory(settings);

	auto options = guiFactory.makeLabel("Options");
	options->setOrigin({ .5f, .5f });

	auto fullscreenButton = guiFactory.makeButton("Fullscreen", make_shared<eventFunctionType>(
		[this](const Event& event) {
		renderer::setFullscreen(!renderer::isFullscreen());
		//mainMenuLayout->getComponent<LayoutComponent>()->updateSize();
		optionsMenuLayout->getComponent<LayoutComponent>()->updateSize();
	}
	));
	fullscreenButton->setOrigin({ .5f, .5f });

	auto selectResolution = guiFactory.makeButton("800x600", make_shared<eventFunctionType>(
		[this](const Event& event) {
		auto& window = renderer::getWindow();
		auto& currentMode = VideoMode::getFullscreenModes()[currentVideoMode];
		renderer::setResolution(currentMode);
		//mainMenuLayout->getComponent<LayoutComponent>()->updateSize();
		optionsMenuLayout->getComponent<LayoutComponent>()->updateSize();
	}
	));
	selectResolution->setOrigin({ .5f, .5f });

	auto previousResolution = guiFactory.makeButton("<", make_shared<eventFunctionType>(
		[this, selectResolution](const Event& event) {
		const auto& videoModes = VideoMode::getFullscreenModes();
		auto& text = selectResolution->getComponent<TextComponent>()->getText();
		currentVideoMode = currentVideoMode != 0 ? currentVideoMode - 1 : videoModes.size() - 1;
		auto currentMode = videoModes[currentVideoMode];
		text.setString(to_string(currentMode.width) + "x" + to_string(currentMode.height));
	}
	));
	previousResolution->setOrigin({ 1.f, .5f });

	auto nextResolution = guiFactory.makeButton(">", make_shared<eventFunctionType>(
		[this, selectResolution](const Event& event) {
		const auto& videoModes = VideoMode::getFullscreenModes();
		auto& text = selectResolution->getComponent<TextComponent>()->getText();
		currentVideoMode = (currentVideoMode + 1) % videoModes.size();
		auto currentMode = videoModes[currentVideoMode];
		text.setString(to_string(currentMode.width) + "x" + to_string(currentMode.height));
	}
	));
	nextResolution->setOrigin({ 0.f, .5f });

	auto useControllerButton = make_shared<Entity>();
	useControllerButton->setOrigin({ .5f, .5f });
	auto controllerButtonText = useControllerButton->addComponent<TextComponent>();
	controllerButtonText->setText();
	controllerButtonText->getText().setFont(*settings.font);
	controllerButtonText->getText().setCharacterSize(settings.characterSize);
	controllerButtonText->getText().setFillColor(settings.baseColor);
	controllerButtonText->getText().setString("Controller disabled");
	auto controllerButtonHover = useControllerButton->addComponent<MouseHoverComponent>();
	controllerButtonHover->baseColor = settings.baseColor;
	controllerButtonHover->hoverColor = settings.hoverColor;
	auto controllerButtonClick = useControllerButton->addComponent<ClickComponent>();
	controllerButtonClick->onClick = make_shared<eventFunctionType>(
		[controllerButtonText](const Event& event) {
		input::setUseController(!input::usingController());
		if (input::usingController())
			controllerButtonText->getText().setString("Controller enabled");
		else
			controllerButtonText->getText().setString("Controller disabled");
	}
	);

	auto azertyButton = guiFactory.makeButton("Set azerty ", make_shared<eventFunctionType>(
		[](const Event& event) {
		input::setAzertyActive();
	}
	));
	azertyButton->setOrigin({ 1.f, .5f });

	auto qwertyButton = guiFactory.makeButton(" Set qwerty", make_shared<eventFunctionType>(
		[](const Event& event) {
		input::setQwertyActive();
	}
	));
	qwertyButton->setOrigin({ 0.f, .5f });

	currentControl = input::getControls().begin();
	auto selectControl = guiFactory.makeButton("Remap controls", make_shared<eventFunctionType>(
		[this](const Event& event) {
		cout << "clicked current control" << endl;
		keyBind = true;
	}
	));
	selectControl->getComponent<TextComponent>()->getText().setCharacterSize(24.f);
	selectControl->setOrigin({ .5f, .5f });

	auto previousControl = guiFactory.makeButton("<", make_shared<eventFunctionType>(
		[this, selectControl](const Event& event) {
		if (currentControl == input::getControls().begin())
			currentControl = input::getControls().end();
		--currentControl;
		selectControl->getComponent<TextComponent>()->getText().setString(*currentControl + " : " + input::to_string(input::getKey(*currentControl)));
	}
	));
	previousControl->setOrigin({ .5f, .5f });

	auto nextControl = guiFactory.makeButton(">", make_shared<eventFunctionType>(
		[this, selectControl](const Event& event) {
		if (currentControl != input::getControls().end())
			++currentControl;
		if (currentControl == input::getControls().end())
			currentControl = input::getControls().begin();
		selectControl->getComponent<TextComponent>()->getText().setString(*currentControl + " : " + input::to_string(input::getKey(*currentControl)));
	}
	));
	nextControl->setOrigin({ .5f, .5f });

	auto displayFpsButton = make_shared<Entity>();
	displayFpsButton->setOrigin({ .5f, .5f });
	auto displayFpsText = displayFpsButton->addComponent<TextComponent>();
	displayFpsText->setText();
	displayFpsText->getText().setFont(*settings.font);
	displayFpsText->getText().setCharacterSize(settings.characterSize);
	displayFpsText->getText().setFillColor(settings.baseColor);
	displayFpsText->getText().setString("FPS counter disabled");
	auto displayFpsHover = displayFpsButton->addComponent<MouseHoverComponent>();
	displayFpsHover->baseColor = settings.baseColor;
	displayFpsHover->hoverColor = settings.hoverColor;
	auto displayFpsClick = displayFpsButton->addComponent<ClickComponent>();
	displayFpsClick->onClick = make_shared<eventFunctionType>(
		[displayFpsText](const Event& event) {
		displayFramerate = !displayFramerate;
		if (displayFramerate)
			displayFpsText->getText().setString("FPS counter enabled");
		else
			displayFpsText->getText().setString("FPS counter disabled");
	}
	);

	auto optionsBackButton = guiFactory.makeButton("Back", make_shared<eventFunctionType>(
		[this](const Event& event) {
		cout << "clicked the back button" << endl;
		scene::load("main-menu");
	}
	));
	optionsBackButton->setOrigin({ .5f, .5f });

	optionsMenuLayout = make_shared<Entity>();
	auto optionsLayout = optionsMenuLayout->addComponent<LayoutComponent>(1.f, 1.f);
	optionsLayout->addItem(options, .5f, 1.f / 8.f);
	optionsLayout->addItem(fullscreenButton, .5f, 2.f / 8.f);
	optionsLayout->addItem(previousResolution, .3f, 3.f / 8.f);
	optionsLayout->addItem(selectResolution, .5f, 3.f / 8.f);
	optionsLayout->addItem(nextResolution, .7f, 3.f / 8.f);
	optionsLayout->addItem(useControllerButton, .5f, 4.f / 8.f);
	optionsLayout->addItem(azertyButton, .5f, 5.f / 8.f);
	optionsLayout->addItem(qwertyButton, .5f, 5.f / 8.f);
	optionsLayout->addItem(previousControl, .3f, 6.f / 8.f);
	optionsLayout->addItem(selectControl, .5f, 6.f / 8.f);
	optionsLayout->addItem(nextControl, .7f, 6.f / 8.f);
	optionsLayout->addItem(displayFpsButton, .5f, 7.f / 8.f);
	optionsLayout->addItem(optionsBackButton, .9f, .9f);

	entityManager.entities.push_back(optionsMenuLayout);
	entityManager.entities.push_back(options);
	entityManager.entities.push_back(fullscreenButton);
	entityManager.entities.push_back(previousResolution);
	entityManager.entities.push_back(selectResolution);
	entityManager.entities.push_back(nextResolution);
	entityManager.entities.push_back(useControllerButton);
	entityManager.entities.push_back(azertyButton);
	entityManager.entities.push_back(qwertyButton);
	entityManager.entities.push_back(previousControl);
	entityManager.entities.push_back(selectControl);
	entityManager.entities.push_back(nextControl);
	entityManager.entities.push_back(displayFpsButton);
	entityManager.entities.push_back(optionsBackButton);
}

void OptionsScene::update(double dt) {
	Scene::update(dt);
}

void OptionsScene::render() {
	Scene::render();
}

void OptionsScene::handleEvent(const sf::Event& event) {
	// if requested by the player
	// rebind the current control to the first key that is pressed
	if (keyBind) {
		input::setCustomActive();
		input::unbindKey(*currentControl);
		input::bindKey(*currentControl, event.key.code);
		keyBind = false;
	}
}

// *** TestLevelScene class *** //

void TestLevelScene::load() {
	/*const auto groundSize = Vector2f(500.f, 50.f);
	auto ground = make_shared<Entity>();
	ground->setOrigin({ .5f, .5f });
	ground->setPosition({ 500.f, -50.f });
	auto groundShape = ground->addComponent<ShapeComponent>();
	groundShape->setShape<RectangleShape>(groundSize);
	auto groundPhysics = ground->addComponent<physics::PhysicsComponent>(false, groundSize);
	groundPhysics->setRestitution(0.f);

	auto ceil = make_shared<Entity>();
	ceil->setOrigin({ .5f, .5f });
	ceil->setPosition({ 500.f, -550.f });
	auto ceilShape = ceil->addComponent<ShapeComponent>();
	ceilShape->setShape<RectangleShape>(groundSize);
	ceilShape->getShape().setFillColor(Color::Magenta);
	auto ceilPhysics = ceil->addComponent<physics::PhysicsComponent>(false, groundSize);
	ceilPhysics->setRestitution(0.f);

	const auto wallSize = Vector2f(50.f, 600.f);
	auto wallL = make_shared<Entity>();
	wallL->setOrigin({ .5f, .5f });
	wallL->setPosition({ 600.f, -350.f });
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
    */
    physics::resetGravity();
    tilemap::getTilemap()->load("res/testLevel.txt");
	auto player = make_shared<Entity>();
	player->setOrigin({ .5f, .5f });
	player->setPosition({ 100.f, 100.f });
	auto playerPhysics = player->addComponent<PlayerPhysicsComponent>(Vector2f(64.f, 128.f));
	auto playerSprite = player->addComponent<SpriteComponent>();
	playerSprite->setSprite();
	auto playerAnimator = player->addComponent<SpritesheetAnimatorComponent>(resources::get<Texture>("PlayerIdle.png"));
	playerAnimator->setSpriteSize({ 64, 128 });
	playerAnimator->setAnimationTime(.2f);

	auto playerField = player->addComponent<GravityFieldComponent>(20.f);
	playerField->setForce(20.f);
	auto gravityRotate = player->addComponent<RotateGravityComponent>();

    auto playerOxygen = player->addComponent<OxygenTimerComponent>(5.f);

    /*
	auto box = make_shared<Entity>();
	box->setOrigin({ .5f, .5f });
	box->setPosition({ 400.f, -300.f });
	auto boxPhysics = box->addComponent<physics::PhysicsComponent>(true, Vector2f(32.f, 32.f));
	auto boxShape = box->addComponent<ShapeComponent>();
	boxShape->setShape<RectangleShape>(Vector2f(32.f, 32.f));

	entityManager.entities.push_back(ground);
	entityManager.entities.push_back(wallL);
	entityManager.entities.push_back(wallR);
	entityManager.entities.push_back(ceil);*/
	entityManager.entities.push_back(player);
	//entityManager.entities.push_back(box);
}

void TestLevelScene::update(double dt) {
	Scene::update(dt);
}

void TestLevelScene::render() {
    tilemap::render();
	Scene::render();
}

// *** Game Over Scene *** //

void GameOverScene::load() {
    const auto font = resources::get<Font>("Xolonium-Bold.ttf");
    const GUISettings settings = { Color::White, Color::Cyan, font.get(), 32.f };
    auto guiFactory = GUIFactory(settings);

    auto gameOverLabel = guiFactory.makeLabel("Game Over");
    gameOverLabel->setOrigin({ .5f, .5f });

    auto hintLabel = guiFactory.makeLabel("Press Escape to return to main menu ...");
    hintLabel->getComponent<TextComponent>()->getText().setCharacterSize(16.f);
    hintLabel->setOrigin({ .5f, .5f });

    auto _layout = make_shared<Entity>();
    auto layout = _layout->addComponent<LayoutComponent>(1.f, 1.f);
    layout->addItem(gameOverLabel, .5f, .5f);
    layout->addItem(hintLabel, .5f, .6f);

    entityManager.entities.push_back(_layout);
    entityManager.entities.push_back(gameOverLabel);
    entityManager.entities.push_back(hintLabel);

    heartBeatSound = Sound(*resources::get<SoundBuffer>("heartbeat-slow-reverb.ogg"));
    heartBeatSound.play();
}

void GameOverScene::update(const double dt) {
    Scene::update(dt);
}

void GameOverScene::render() {
    Scene::render();
}

