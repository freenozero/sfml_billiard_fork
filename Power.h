#pragma once
#include "SampleBilliardGameBall.h"

//�ڵ��� x����
#define HDX2 469
#define HDX1 69
//�ڵ� y����
#define HDY 739
class Power {
private:
	//Ʋ
	sf::Texture	Frame;
	sf::Sprite	Fsprite;
	//������
	sf::Texture	PHandle;
	sf::Sprite	PHsprite;

	sf::Vector2f HandlePos;
	float Hwidth;
	float Hheight;

	//�Ŀ� ����
	bool statePower; //�Ŀ��ڵ��� ����
	float DraggedDistance; //���� �Ÿ�

	//�����̽��� �ɼ�
	bool SpaceKey;
	//�ڵ� ���⺯��
	bool ChangeDir;

public:
	Power();
	void render(sf::RenderTarget& target);
	float getHwidth();
	float getHheight();
	void InitHandlePosition();
	void setHandlePosition(sf::Vector2f pos);
	sf::Vector2f getHandlePosition() const;
	void FixRange(); //�ڵ��� ���� ������ ����
	bool inHandle(sf::Vector2f mouse); //���콺�� �ڵ� �ȿ� ��������
	void setDragPower(bool state);
	bool isDraggingPower() const;
	float getDraggedDistance(); //���� �Ÿ�
	void InitDraggedDistance(); //�Ÿ� �ʱ�ȭ
	void setPressedSpace(bool v); //�����̽��� ���ȴ��� ����
	bool isPressedSpace();
	void setChangeDir(bool v); //�����̽��� �ӵ� ���� ����
	bool isChangeDir();
};