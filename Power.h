#pragma once
#include "SampleBilliardGameBall.h"

//핸들의 x범위
#define HDX2 469
#define HDX1 69
//핸들 y고정
#define HDY 739
class Power {
private:
	//틀
	sf::Texture	Frame;
	sf::Sprite	Fsprite;
	//손잡이
	sf::Texture	PHandle;
	sf::Sprite	PHsprite;

	sf::Vector2f HandlePos;
	float Hwidth;
	float Hheight;

	//파워 제어
	bool statePower; //파워핸들의 상태
	float DraggedDistance; //끌린 거리

	//스페이스바 옵션
	bool SpaceKey;
	//핸들 방향변경
	bool ChangeDir;

public:
	Power();
	void render(sf::RenderTarget& target);
	float getHwidth();
	float getHheight();
	void InitHandlePosition();
	void setHandlePosition(sf::Vector2f pos);
	sf::Vector2f getHandlePosition() const;
	void FixRange(); //핸들이 범위 없으면 교정
	bool inHandle(sf::Vector2f mouse); //마우스가 핸들 안에 들어오는지
	void setDragPower(bool state);
	bool isDraggingPower() const;
	float getDraggedDistance(); //끌린 거리
	void InitDraggedDistance(); //거리 초기화
	void setPressedSpace(bool v); //스페이스바 눌렸는지 여부
	bool isPressedSpace();
	void setChangeDir(bool v); //스페이스바 속도 방향 변경
	bool isChangeDir();
};