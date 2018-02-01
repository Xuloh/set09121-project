// remove console window in release build
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <iostream>
#include "main.h"
#include "Box2D/Dynamics/b2World.h"
#include "Box2D/Common/b2Math.h"
#include "Box2D/Dynamics/b2Body.h"
#include "Box2D/Collision/Shapes/b2PolygonShape.h"
#include "Box2D/Dynamics/b2Fixture.h"
#include "Box2D/Collision/Shapes/b2ChainShape.h"

using namespace sf;
using namespace std;

Font font;
Text helpText;
Text gravityArrow;

Texture texture;
Sprite cube1;
Sprite cube2;
b2Body* square;
b2Body* square2;
b2Body* box;
RectangleShape boxShape;

b2Vec2 baseGravity(0.f, -9.81f);
b2World world(baseGravity);

float metersToPixels(float meters) {
	return meters * 50.f;
}

float pixelsToMeters(float pixels) {
	return pixels / 50.f;
}

float radiansToDegrees(float radians) {
	return -float(double(radians) * 180.0 / M_PI);
}

float degreesToRadians(float degrees) {
	return -float(double(degrees) * M_PI / 180.0);
}

void load() {
	
	if (!font.loadFromFile("res/fonts/FiraCode-Medium.ttf"))
		cerr << "could not load font :(" << endl;

	if (!texture.loadFromFile("res/img/companion-cube.png"))
		cerr << "could not load texture :(" << endl;

	helpText.setFont(font);
	helpText.setCharacterSize(12);
	helpText.setString("Use the arrows to change the gravity\nUse Ctrl+Left/Ctrl+Right to rotate the box\nPress Space to reset cube position\nPress return to reverse the gravity on one of the cubes");
	helpText.setPosition(12, 12);

	gravityArrow.setFont(font);
	gravityArrow.setCharacterSize(32);
	gravityArrow.setString("->");
	gravityArrow.setRotation(90.f);
	gravityArrow.setOrigin(gravityArrow.getGlobalBounds().width * .5f, gravityArrow.getGlobalBounds().height * .5f);

	// create the box that will be rendered by sfml
	boxShape = RectangleShape({ gameWidth - 100.f + boxBorderSize, gameHeight - 100.f + boxBorderSize });
	boxShape.setOrigin({ (gameWidth - 100.f + boxBorderSize) * .5f, (gameHeight - 100.f + boxBorderSize) * .5f });
	boxShape.setPosition(windowCenter);
	boxShape.setFillColor(Color::Black);
	boxShape.setOutlineColor(Color::Blue);
	boxShape.setOutlineThickness(boxBorderSize);

	// create the square that will be rendered by sfml
	cube1 = Sprite(texture);
	cube1.setOrigin({ squareSize * .5f, squareSize * .5f });
	cube1.setPosition(windowCenter);
	
	cube2 = Sprite(texture);
	cube2.setOrigin({ squareSize * .5f, squareSize * .5f });
	cube2.setPosition(windowCenter);

	const float halfWidth = pixelsToMeters(gameWidth * .5f);
	const float halfHeight = pixelsToMeters(gameHeight * .5f);
	const float boxCornerX = pixelsToMeters(gameWidth * .5f - 50.f);
	const float boxCornerY = pixelsToMeters(gameHeight * .5f - 50.f);

	// create the box that will be handled by box2d
	b2BodyDef boxBodyRef;
	boxBodyRef.position.Set(halfWidth, halfHeight);
	box = world.CreateBody(&boxBodyRef);
	b2Vec2 boxPoints[4];
	boxPoints[0].Set(-boxCornerX, boxCornerY);
	boxPoints[1].Set(boxCornerX, boxCornerY);
	boxPoints[2].Set(boxCornerX, -boxCornerY);
	boxPoints[3].Set(-boxCornerX, -boxCornerY);
	b2ChainShape boxShape;
	boxShape.CreateLoop(boxPoints, 4);
	box->CreateFixture(&boxShape, 0.f);

	// create the cube that will be handled by box2d
	const float halfSquareSize = pixelsToMeters(squareSize * .5f);
	b2BodyDef squareDef;
	squareDef.type = b2_dynamicBody;
	squareDef.position.Set(halfWidth, halfHeight);
	squareDef.allowSleep = false;
	square = world.CreateBody(&squareDef);
	b2PolygonShape squareBox;
	squareBox.SetAsBox(halfSquareSize, halfSquareSize);
	b2FixtureDef squareFixtureDef;
	squareFixtureDef.shape = &squareBox;
	squareFixtureDef.density = 2.f;
	squareFixtureDef.friction = 0.7f;
	square->CreateFixture(&squareFixtureDef);

	// cube 2
	square2 = world.CreateBody(&squareDef);
	b2PolygonShape square2Box;
	square2Box.SetAsBox(halfSquareSize, halfSquareSize);
	square2->CreateFixture(&squareFixtureDef);
}

void reset() {

}

void update(RenderWindow &window) {
	static Clock clock;
	const float dt = clock.restart().asSeconds();

	Event event;
	while (window.pollEvent(event)) {
		if (event.type == Event::Closed)
			window.close();
	}

	if (Keyboard::isKeyPressed(Keyboard::Escape))
		window.close();

	if (Keyboard::isKeyPressed(resetCubes)) {
		square->SetTransform({ pixelsToMeters(float(gameWidth) * .5f), pixelsToMeters(float(gameHeight) * .5f) }, 0.f);
		square->SetLinearVelocity({ 0.f, 0.f });
		square->SetAngularVelocity(0.f);

		square2->SetTransform({ pixelsToMeters(float(gameWidth) * .5f), pixelsToMeters(float(gameHeight) * .5f) }, 0.f);
		square2->SetLinearVelocity({ 0.f, 0.f });
		square2->SetAngularVelocity(0.f);
	}

	if(Keyboard::isKeyPressed(reverseGravityKey)) {
		reverseGravityCube = !reverseGravityCube;
		if (reverseGravityCube)
			square2->SetGravityScale(0.f);
		else
			square2->SetGravityScale(1.f);
	}

	if (Keyboard::isKeyPressed(Keyboard::LControl)) {
		// Rotate box
		if (Keyboard::isKeyPressed(rotateBoxLeft))
			box->SetTransform(box->GetPosition(), box->GetAngle() + degreesToRadians(-boxRotateSpeed) * dt);

		if (Keyboard::isKeyPressed(rotateBoxRight))
			box->SetTransform(box->GetPosition(), box->GetAngle() + degreesToRadians(boxRotateSpeed) * dt);
	}
	else {
		// Change gravity on key press
		if (Keyboard::isKeyPressed(gravityUpKey)) {
			world.SetGravity(gravityUp);
			gravityArrow.setRotation(270.f);
		}

		if (Keyboard::isKeyPressed(gravityLeftKey)) {
			world.SetGravity(gravityLeft);
			gravityArrow.setRotation(180.f);
		}

		if (Keyboard::isKeyPressed(gravityDownKey)) {
			world.SetGravity(gravityDown);
			gravityArrow.setRotation(90.f);
		}

		if (Keyboard::isKeyPressed(gravityRightKey)) {
			world.SetGravity(gravityRight);
			gravityArrow.setRotation(0.f);
		}
	}

	gravityArrow.setPosition(gameWidth * .5f - gravityArrow.getLocalBounds().width * .5f, gameHeight * .5f - gravityArrow.getLocalBounds().height * .5f);

	if(reverseGravityCube)
		square2->ApplyForceToCenter(square2->GetMass() * -world.GetGravity(), true);
	
	world.Step(dt, velocityIterations, positionIterations);

	const auto squarePosition = square->GetPosition();
	const auto squareAngle = square->GetAngle();
	const auto square2Position = square2->GetPosition();
	const auto square2Angle = square2->GetAngle();
	const auto boxAngle = box->GetAngle();
	
	//cout << "square transform : " << squarePosition.x << " " << squarePosition.y << " " << squareAngle << endl;

	cube1.setPosition({ metersToPixels(squarePosition.x), gameHeight - metersToPixels(squarePosition.y) });
	cube1.setRotation(radiansToDegrees(squareAngle));

	cube2.setPosition({ metersToPixels(square2Position.x), gameHeight - metersToPixels(square2Position.y) });
	cube2.setRotation(radiansToDegrees(square2Angle));

	boxShape.setRotation(radiansToDegrees(boxAngle));

	//cout << "shape transform  : " << cube1.getPosition().x << " " << cube1.getPosition().y << " " << cube1.getRotation() << endl;
}

void render(RenderWindow &window) {
	window.draw(boxShape);
	window.draw(gravityArrow);
	window.draw(cube1);
	window.draw(cube2);
	window.draw(helpText);
}

int main() {
	RenderWindow window(VideoMode(gameWidth, gameHeight), "Dynamic Gravity Test", Style::Titlebar | Style::Close);

	load();
	reset();

	while (window.isOpen()) {
		window.clear();
		update(window);
		render(window);
		window.display();
	}

	return 0;
}