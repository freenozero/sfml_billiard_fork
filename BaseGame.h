#pragma once

#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

class BaseGame
{
public:
	// 게임 객체 초기화 
	BaseGame(int width, int height, int fpsLimit, int option);

	// 게임 객체 해제 
	virtual ~BaseGame(void);

	// 메인 게임 실행 루프 
	void run(void);

	//BGM실행
	void PlayBGM(const char* BGM);

protected:
	// 상속 클래스는 반드시 이벤트 핸들링 함수 구현해야 함 
	virtual void handle(sf::Event& ev) = 0;

	// 상속 클래스는 반드시 게임 상태 갱신 함수 구현해야 함 
	virtual void update(void) = 0;

	// 상속 클래스는 반드시 객체 렌더링 함수 구현해야 함 
	virtual void render(sf::RenderTarget& target) = 0;

protected:
	// 게임 렌더링 타겟
	sf::RenderWindow* window;

private:
	//BGM
	sf::SoundBuffer Gamebuffer;
	sf::Sound Gamesound;
};