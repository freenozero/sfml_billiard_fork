#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
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
#include "Power.h"
#include "GameExit.h"

class FourBallGame : public BaseGame
{
public:
	//3�� �ʱ�ȭ 
	FourBallGame(int width, int height, int fpsLimit, int option);

	//���� 
	virtual ~FourBallGame(void);

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
	//���� ���� ������
	void renderDirection1(sf::RenderTarget& target);
	void renderDirection2(sf::RenderTarget& target);
	//Ÿ�̸� ������
	void PlayerTimerRender(sf::RenderTarget& target);

	//�籸�� ��� ������ ���� ����
	sf::Vector2f FixVec(sf::Vector2f vec);

	sf::Vector2f FixVec1(sf::Vector2f vec);

	//������Ʈ �ʱ�ȭ
	void makeBilliardObject();

private:
	// Sample Game���� �����ϴ� ���� ������Ʈ�� (��, �籸�� ��) 
	std::vector<SampleBilliardObject*> gameObjects;
	std::vector<Player*> Players;

	//���� ����â
	GameExit* ExitWindow;

	// ���� UI
	sf::Texture tBackGround;
	sf::Sprite sBackGround;

	// ���� �ð� 
	sf::Clock clock;

	// ���콺 ��ġ
	sf::Vector2f mouseXY;
	//���콺�� �������� ����
	bool isDraggedMouse;

	//PlayerBall�� �籸�� �浹Ƚ��	
	int CollideBoardCnt;

	//�Ŀ�
	Power* power;
	//��ġ����
	sf::Vector2f Posvec;
	sf::Vector2f Posvec1;
	//���� ����
	sf::Vector2f Dirvec;
	sf::Vector2f Dirvec1;

	//ť�� �ؽ���
	sf::Texture CueTexture;
	sf::Sprite CueSprite;
	//���� ����(������)
	sf::Vector2f CueDirvec;

	SampleBilliardGameBall* playerBall;
	SampleBilliardGameBall* playerBall1;

	//�� �׽�Ʈ��
	bool isCatchingBall;
	SampleBilliardBall* catchedBall;

	// ���� ���� ��Ʈ 
	static sf::Font* font;

	//player ���� �ð�
	sf::Clock playerClock;
	float StopTimer;
	float TurnTime;
};