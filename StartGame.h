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
	// 상속 클래스는 반드시 이벤트 핸들링 함수 구현해야 함 
	virtual void handle(sf::Event& ev);

	// 상속 클래스는 반드시 게임 상태 갱신 함수 구현해야 함 
	virtual void update(void);

	// 상속 클래스는 반드시 객체 렌더링 함수 구현해야 함 
	virtual void render(sf::RenderTarget& target);

private:
	std::vector<SampleBilliardObject*> gameObjects;

	// 게임 UI 
	// 게임 화면
	sf::Texture			texture;
	sf::Sprite			sprite;

	// 게임 시간 
	sf::Clock clock;

	// 마우스 위치
	sf::Vector2f mouseXY;

	// 게임 전역 폰트 
	static sf::Font* font;
};