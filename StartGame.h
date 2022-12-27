#pragma once
#include <iostream> 
#include <vector> 
#include <string>
#include <cstdlib>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "GameButton.h"
#include "SampleGame.h"
#include "ThreeBallGame.h"
#include "FourBallGame.h"
#include "BaseGame.h"
#include "SampleBilliardGameBall.h"

enum {
	T1 = 1, T2, T3, T4
};

//���� �߰� �� �� ���⿡ �ɼ� �߰�
enum {
	DEF = 0, EIGHTBALL, THREEBALL, FOURBALL
};

class StartGame {
public:
	StartGame(int width, int height, int fpsLimit);

	~StartGame(void);

	void handle(sf::Event& ev);

	void render(sf::RenderTarget& target);

	void Info(void);

	static bool isClose();

private:
	int option;
	int Width;
	int Height;
	int FpsLimit;

	//�ؽ��� �ð� ����
	sf::Clock clock;
	float time;
	float interval;

	// ���� UI 
	// ���� ȭ��
	sf::Texture			texture;
	sf::Sprite			sprite;

	// ���� UI��
	sf::Texture			Btexture;
	sf::Sprite			Bsprite;

	sf::Texture	ButtonT1;
	sf::Texture	ButtonT2;
	sf::Texture	ButtonT3;
	sf::Texture	ButtonT4;

	//���ӹ�ư
	std::vector<GameButton*> GBBW; //��� ��ư
	std::vector<GameButton*> GB; //�� ��ư

	//��׶��� �ؽ���
	sf::Texture			BackGround;

	// ���� ���� �ؽ��� ��ȣ
	int Tnum;
	static bool Close;

	// ���콺 ��ġ
	sf::Vector2f mouseXY;

	//ȭ�� �ѱ� ����
	bool isChangeBK;

	// ���� ������ Ÿ��
	sf::RenderWindow* window;

	// ����BGM
	sf::SoundBuffer Startbuffer;
	sf::Sound Startsound;
};