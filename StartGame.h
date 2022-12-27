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

//게임 추가 될 때 여기에 옵션 추가
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

	//텍스쳐 시간 조정
	sf::Clock clock;
	float time;
	float interval;

	// 게임 UI 
	// 게임 화면
	sf::Texture			texture;
	sf::Sprite			sprite;

	// 게임 UI셋
	sf::Texture			Btexture;
	sf::Sprite			Bsprite;

	sf::Texture	ButtonT1;
	sf::Texture	ButtonT2;
	sf::Texture	ButtonT3;
	sf::Texture	ButtonT4;

	//게임버튼
	std::vector<GameButton*> GBBW; //흑백 버튼
	std::vector<GameButton*> GB; //색 버튼

	//백그라운드 텍스쳐
	sf::Texture			BackGround;

	// 현재 적용 텍스쳐 번호
	int Tnum;
	static bool Close;

	// 마우스 위치
	sf::Vector2f mouseXY;

	//화면 넘김 여부
	bool isChangeBK;

	// 게임 렌더링 타겟
	sf::RenderWindow* window;

	// 게임BGM
	sf::SoundBuffer Startbuffer;
	sf::Sound Startsound;
};