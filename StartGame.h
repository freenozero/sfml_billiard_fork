#pragma once
#include <iostream> 
#include <vector> 
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "BaseGame.h"
#include "SampleBilliardGameBall.h"
class StartGame : public BaseGame {
public:
	StartGame(int width, int height, int fpsLimit);
 
	virtual ~StartGame(void);

	static const sf::Font& getFont(void);

protected:
	// ��� Ŭ������ �ݵ�� �̺�Ʈ �ڵ鸵 �Լ� �����ؾ� �� 
	virtual void handle(sf::Event& ev);

	// ��� Ŭ������ �ݵ�� ���� ���� ���� �Լ� �����ؾ� �� 
	virtual void update(void);

	// ��� Ŭ������ �ݵ�� ��ü ������ �Լ� �����ؾ� �� 
	virtual void render(sf::RenderTarget& target);

private:
	std::vector<SampleBilliardObject*> gameObjects;

	// ���� UI 
	// ���� ȭ��
	sf::Texture			texture;
	sf::Sprite			sprite;

	// ���� �ð� 
	sf::Clock clock;

	// ���콺 ��ġ
	sf::Vector2f mouseXY;

	// ���� ���� ��Ʈ 
	static sf::Font* font;
};