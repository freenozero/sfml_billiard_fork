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

	// Sample Game�� ��ü���� �ݵ�� ���� ���� �Լ� �����ؾ� �� 
	virtual void update(float timeElapsed);

	// Sample Game�� ��ü���� �ݵ�� �浹 ���� �����ؾ� ��
	virtual void collide(SampleBilliardObject& other);

	// Sample Game�� ��ü���� �ݵ�� ������ �Լ� �����ؾ� ��  
	virtual void render(sf::RenderTarget& target);

	int getPlayerNum() const; //�÷��̾� �ѹ�

	static void setSizePocket(int size);
	static int getSizePocket();

	static Player& WhoisTurn(); //������ ���ΰ�? Player��ȯ

	void setTurn(bool turn); //�� ����
	bool isTurn() const; //���� ������

	void setPhase(int Phase); //���� �ƴ��� ���� ����
	int isPhase() const; //������ ����
	
	void setScore(int param);
	void setScore(std::string param);
	std::string getScore() const;
	
	void yourLose(); //�й�
	void yourWin(); //�¸�
	
	void setWin(int n); //�¸� �ܰ� ����
	int isWin() const; //�¸� �ܰ�

	bool isLast(); //�÷��̾ ������ �ܰ����� Ȯ�� 

	void setBallType(int type); //�� Ÿ��
	int getBallType() const;

	//�� Ÿ�Կ� ���� ����
	void IfTypeSet(int DiffSize); 

	void setPutBallCnt(int num); //�÷��̾ ���� ���� ��
	int getPutBallCnt() const; 

	void setNextP(Player& p);
	Player& getNextP() const;

	//�÷��̾� ���� �ʱ�ȭ
	void Init();

private:
	static Player* TurnPlayer; //��������� ���� ������ �ִ� �÷��̾ �ִ´�.	
	static int PocketSize; //������ ��� ���� ������(���� ũ���� ��ȭ�� �Ǵ��� �� ����� ��.
	bool turn;
	int Phase; // �� ġ�� �� : 0 || ���� ģ ���� : 1 || ���� ġ�� ���� : -1 
	int win; // �¸� : 1 || �¸��� : 0 || �⺻: -1
	int score;
	int ballType; //-1: �극��ũ ��(�ʱ�) || 0: ����  | 1: Solids  | 9:  Stripes 
	int PlayerNum; //�÷��̾� �ѹ�
	int PutBallCnt; //���� ���� �� : �� �� ����
	Player* NextP; //���� �÷��̾��� �ּ�
};