#pragma once
#include<iostream>
#include <string>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "SampleBilliardObject.h"
#include "SampleBilliardBoard.h"
#include "SampleBilliardGameBall.h"

#define BREAKSHOT -1
#define UNKNOWN 0
#define SOLIDS 1
#define STRIPES 9

enum {
	STOP=-1,BASIC,MOVE
};

class Player : public SampleBilliardObject{
public:
	Player();
	Player(int num,bool turn);
	virtual ~Player(void);

	void update(SampleBilliardGameBall& Ball, SampleBilliardObject& eightBall, int V);

	// Sample Game의 객체들은 반드시 상태 갱신 함수 구현해야 함 
	virtual void update(float timeElapsed);

	// Sample Game의 객체들은 반드시 충돌 물리 구현해야 함
	virtual void collide(SampleBilliardObject& other);

	// Sample Game의 객체들은 반드시 렌더링 함수 구현해야 함  
	virtual void render(sf::RenderTarget& target);

	static void setSizePocket(int size);
	static int getSizePocket();

	void setTurn(bool turn); //턴 설정
	bool isTurn() const; //현재 턴인지

	void setPhase(int Phase); //공을 쳤는지 여부 설정
	int isPhase() const; //페이즈 상태
	
	void setScore(int param);
	void setScore(std::string param);
	std::string getScore() const;
	
	void yourWin(bool win); //승리
	void yourLose(bool lose); //패배
	bool isWin() const; //승패가 결정되었는지

	void setBallType(int type); //공 타입
	int getBallType() const;

	void setNextP(Player& p);
	Player& getNextP() const;

private:
	static int PocketSize; //포켓의 요소 수를 저장함(포켓 크기의 변화를 판단할 때 사용할 것.
	bool turn;
	int Phase; // 공 치기 전 : 0 || 공을 친 상태 : 1 || 공을 치고 정지 : -1 
	bool win; //기본 false
	int score;
	int ballType; //-1: 브레이크 샷(초구) || 0: 미정  | 1: Solids  | 9:  Stripes 
	int PlayerNum; //플레이어 넘버
	Player* NextP; //다음 플레이어의 주소
};