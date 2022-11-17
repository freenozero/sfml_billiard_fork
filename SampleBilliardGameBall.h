#pragma once
#include <cmath>
#include<cstdlib>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "SampleBilliardBall.h"

class SampleBilliardGameBall : public SampleBilliardBall
{
public:
	using SampleBilliardBall::SampleBilliardBall;

	bool isIntersecting(sf::Vector2f point);
	bool isPlayable(void);
	void setPlayable(bool playable);

	// 게임 공에 숫자를 그리기 위해 virtual 함수 재정의 
	void render(sf::RenderTarget& window);

	void setOwner(std::string owner);
	bool isOwner(std::string owner);
	std::string getOwner(void);

	//여기에 플레이어 턴 가져오는 함수 추가

private:
	bool playable;
};