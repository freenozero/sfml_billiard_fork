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

	bool collideWithFourBall(SampleBilliardGameBall& other);

	// 게임 공에 숫자를 그리기 위해 virtual 함수 재정의 
	void render(sf::RenderTarget& window);

	void setOwner(std::string owner);
	bool isOwner(std::string owner);
	std::string getOwner(void);

	//파울 여부 확인
	void setFoul(bool f);
	bool isFoul();

	//공 선택 여부 확인
	void Select();
	void DeSelect();
	bool isSelected();

	//당구대 충돌 횟수
	void setCollideBoardCnt(int val);
	int getCollideBoardCnt();

	//새로운 적구와 충돌 시 판단
	void setNewCollideBall(SampleBilliardBall& ball);
	bool isNewCollideBall();
	void InitNewCollideBall();


	//테스트코드
	void setTrueCB();
private:
	SampleBilliardBall* CollideBall;
	SampleBilliardBall* CollideBall1;
	bool playable;
	bool Foul;
	bool Selected;
	int CollideBoardCnt; //당구대와 충돌한 횟수
	bool NewCollideBall; //적구와 2번 충돌했는지 여부
};
