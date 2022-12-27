#pragma once
#include <cmath>
#include <vector>
#include "SampleBilliardBall.h"
#include "Player.h"


using std::vector;

class BilliardPocket : public SampleBilliardBall
{
public:
	using SampleBilliardBall::SampleBilliardBall;
	// ������ �Լ� ������  
	virtual void render(sf::RenderTarget& target);
	// �浹 ������
	virtual void collide(SampleBilliardObject& other);
	virtual ~BilliardPocket();
	void putBall(SampleBilliardObject& Ball); //�� �ֱ�
	static SampleBilliardObject& outBall(int index); //�� ��������

	//���Ͼȿ� �ش� ���� �ִ��� �˻� �� �ε��� ��ȯ(������ -1)
	static int InPocket(SampleBilliardObject& Ball); 
	//������ ũ�� ��ȯ
	static int getSizePocket();
	//stripes�� ���� ��ȯ
	static int getCntStripes();
	//solids�� ���� ��ȯ
	static int getCntSolids();
	//�ش� �ε����� �� ����
	static SampleBilliardBall& BallRef(int index);
	//���� �ʱ�ȭ
	static void initPocket();

	//���� �׽�Ʈ�ڵ�
	void printBall(sf::RenderTarget& target);

private:
	void collideWithBall(SampleBilliardBall& other);
	static vector<SampleBilliardObject*> Pocket; //���� ����
};