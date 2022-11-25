#pragma once
#include <iostream>
#include <vector> 

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "SampleBilliardObject.h"
#include "Player.h"

class ScoreBoard : public SampleBilliardObject
{
public:
	// 생성자 
	ScoreBoard();

	// 소멸자 
	~ScoreBoard(void);

	// Sample Game의 객체들은 반드시 상태 갱신 함수 구현해야 함 
	virtual void update(float timeElapsed);

	// Sample Game의 객체들은 반드시 충돌 물리 구현해야 함
	virtual void collide(SampleBilliardObject& other);

	// Sample Game의 객체들은 반드시 렌더링 함수 구현해야 함  
	virtual void render(sf::RenderTarget& target);

private:
	//큰 점수판
	sf::RectangleShape mainScoreBoard;
	//플레이어 점수판
	sf::RectangleShape player1Board;
	sf::RectangleShape player2Board;
};