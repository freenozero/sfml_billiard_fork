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
#include "Player.h"
#include "SampleBilliardObject.h"
#include "SampleBilliardGameBall.h"
#include "ScoreBoard.h"


class SampleGame : public BaseGame
{
public:
	// Sample Game �ʱ�ȭ 
	SampleGame(int width, int height, int fpsLimit, int option);

	// Sample Game ���� 
	virtual ~SampleGame(void);

	// Sample Game ���� ��Ʈ 
	static const sf::Font& getFont(void);

protected:
	// ��� Ŭ������ �ݵ�� �̺�Ʈ �ڵ鸵 �Լ� �����ؾ� �� 
	virtual void handle(sf::Event& ev);

	// ��� Ŭ������ �ݵ�� ���� ���� ���� �Լ� �����ؾ� �� 
	virtual void update(void);

	// ��� Ŭ������ �ݵ�� ��ü ������ �Լ� �����ؾ� �� 
	virtual void render(sf::RenderTarget& target);

private:
	// Sample Game ���� ���� ��� ���� ǥ��
	void renderDragpower(sf::RenderTarget& target);

	//Ÿ�̸� ������
	void PlayerTimerRender(sf::RenderTarget& target);

private:
	// Sample Game���� �����ϴ� ���� ������Ʈ�� (��, �籸�� ��) 
	std::vector<SampleBilliardObject*> gameObjects;
	std::vector<Player*> Players;

	// ���� UI
	sf::Texture tBackGround;
	sf::Sprite sBackGround;

	// ���� �ð� 
	sf::Clock clock;

	// ���콺 ��ġ
	sf::Vector2f mouseXY;

	// �� �߻�� 
	bool isDraggingBall;
	SampleBilliardGameBall* draggedBall;

	//�� �׽�Ʈ��
	bool isCatchingBall;
	SampleBilliardBall* catchedBall;

	// ���� ���� ��Ʈ 
	static sf::Font* font;

	//player ���� �ð�
	sf::Clock playerClock;
	float StopTimer;
};