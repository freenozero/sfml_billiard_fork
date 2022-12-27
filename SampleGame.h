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

class SampleGame : public BaseGame
{
public:
	// Sample Game 초기화 
	SampleGame(int width, int height, int fpsLimit, int option);

	// Sample Game 해제 
	virtual ~SampleGame(void);

	// Sample Game 전역 폰트 
	static const sf::Font& getFont(void);

protected:
	// 상속 클래스는 반드시 이벤트 핸들링 함수 구현해야 함 
	virtual void handle(sf::Event& ev);

	// 상속 클래스는 반드시 게임 상태 갱신 함수 구현해야 함 
	virtual void update(void);

	// 상속 클래스는 반드시 객체 렌더링 함수 구현해야 함 
	virtual void render(sf::RenderTarget& target);

private:

	//공의 방향 랜더링
	void renderDirection(sf::RenderTarget& target);

	//타이머 랜더링
	void PlayerTimerRender(sf::RenderTarget& target);

	//당구대 경계 안으로 벡터 수정
	sf::Vector2f FixVec(sf::Vector2f vec);

	//오브젝트 초기화
	void makeBilliardObject();

private:
	// Sample Game에서 관리하는 게임 오브젝트들 (공, 당구대 등) 
	std::vector<SampleBilliardObject*> gameObjects;
	std::vector<Player*> Players;
	
	//게임 종료창
	GameExit* ExitWindow;

	// 게임 UI
	sf::Texture tBackGround;
	sf::Sprite sBackGround;

	// 게임 시간 
	sf::Clock clock;

	// 마우스 위치
	sf::Vector2f mouseXY;
	//마우스로 끌었는지 여부
	bool isDraggedMouse;

	//파워
	Power* power;
	//위치벡터
	sf::Vector2f Posvec;
	//방향 벡터
	sf::Vector2f Dirvec;

	//큐대 텍스쳐
	sf::Texture CueTexture;
	sf::Sprite CueSprite;
	//방향 벡터(고정값)
	sf::Vector2f CueDirvec;

	SampleBilliardGameBall* playerBall;

	//공 테스트용
	//bool isCatchingBall;
	//SampleBilliardBall* catchedBall;

	// 게임 전역 폰트 
	static sf::Font* font;

	//player 게임 시간
	sf::Clock playerClock;
	float StopTimer;
	float TurnTime;
};