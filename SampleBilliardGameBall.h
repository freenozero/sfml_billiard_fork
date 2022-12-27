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

	// ���� ���� ���ڸ� �׸��� ���� virtual �Լ� ������ 
	void render(sf::RenderTarget& window);

	void setOwner(std::string owner);
	bool isOwner(std::string owner);
	std::string getOwner(void);

	//�Ŀ� ���� Ȯ��
	void setFoul(bool f);
	bool isFoul();

	//�� ���� ���� Ȯ��
	void Select();
	void DeSelect();
	bool isSelected();

	//�籸�� �浹 Ƚ��
	void setCollideBoardCnt(int val);
	int getCollideBoardCnt();

	//���ο� ������ �浹 �� �Ǵ�
	void setNewCollideBall(SampleBilliardBall& ball);
	bool isNewCollideBall();
	void InitNewCollideBall();


	//�׽�Ʈ�ڵ�
	void setTrueCB();
private:
	SampleBilliardBall* CollideBall;
	SampleBilliardBall* CollideBall1;
	bool playable;
	bool Foul;
	bool Selected;
	int CollideBoardCnt; //�籸��� �浹�� Ƚ��
	bool NewCollideBall; //������ 2�� �浹�ߴ��� ����
};
