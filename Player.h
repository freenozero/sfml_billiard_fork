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

enum {
	DEFAULT=-1,LAST,WIN
};

class Player : public SampleBilliardObject{
public:
	Player();
	Player(int num,bool turn);
	virtual ~Player(void);

	void EightBallupdate(SampleBilliardGameBall& Ball, SampleBilliardObject& eightBall, int V);

	// Sample Game의 객체들은 반드시 상태 갱신 함수 구현해야 함 
	virtual void update(float timeElapsed);

	// Sample Game의 객체들은 반드시 충돌 물리 구현해야 함
	virtual void collide(SampleBilliardObject& other);

	// Sample Game의 객체들은 반드시 렌더링 함수 구현해야 함  
	virtual void render(sf::RenderTarget& target);

	int getPlayerNum() const; //플레이어 넘버

	static void setSizePocket(int size);
	static int getSizePocket();

	static Player& WhoisTurn(); //누구의 턴인가? Player반환

	void setTurn(bool turn); //턴 설정
	bool isTurn() const; //현재 턴인지

	void setPhase(int Phase); //공을 쳤는지 여부 설정
	int isPhase() const; //페이즈 상태
	
	void setScore(int param);
	void setScore(std::string param);
	std::string getScore() const;
	
	void yourLose(); //패배
	void yourWin(); //승리
	
	void setWin(int n); //승리 단계 설정
	int isWin() const; //승리 단계

	bool isLast(); //플레이어가 마지막 단계인지 확인 

	void setBallType(int type); //공 타입
	int getBallType() const;

	//공 타입에 따른 설정
	void IfTypeSet(int DiffSize); 

	void setPutBallCnt(int num); //플레이어가 넣은 공의 수
	int getPutBallCnt() const; 

	void setNextP(Player& p);
	Player& getNextP() const;

	//플레이어 정보 초기화
	void Init();

private:
	static Player* TurnPlayer; //정적멤버로 턴을 가지고 있는 플레이어를 넣는다.	
	static int PocketSize; //포켓의 요소 수를 저장함(포켓 크기의 변화를 판단할 때 사용할 것.
	bool turn;
	int Phase; // 공 치기 전 : 0 || 공을 친 상태 : 1 || 공을 치고 정지 : -1 
	int win; // 승리 : 1 || 승리전 : 0 || 기본: -1
	int score;
	int ballType; //-1: 브레이크 샷(초구) || 0: 미정  | 1: Solids  | 9:  Stripes 
	int PlayerNum; //플레이어 넘버
	int PutBallCnt; //넣은 공의 수 : 흰 공 제외
	Player* NextP; //다음 플레이어의 주소
};