#include "SampleBilliardBoard.h"

SampleBilliardBoard::SampleBilliardBoard(void)
{
	// Sample Game을 위한 당구대 텍스처 이미지 로드 
	texture.loadFromFile("textureBoard.png", sf::IntRect(1, 1, 454, 848));

	// 스프라이트 설정 
	sprite.setTexture(texture);
	sprite.setPosition(572.5f, 25.f);

	// 당구대 경계 초기화
	
	//공의 반경만큼 당구대 범위가 수정됩니다.
	/*
		내부 x,y좌표하고
		외부 x,y좌표
		공 반경만 수정해서 사용가능합니다.
	*/
	float Radius = 20.f;
	Radius = Radius + 5.f; //공의 반경 + 여분
	
	//내부 경계
	float inX[2] = { 602.5f,992.f };
	float inY[2] = { 55.f,840.f };
	typedef SampleBilliardBoard::Border B;
	B inLineTop(inX[0]+Radius, inY[0], inX[1]- Radius, inY[0]);
	B inLineBottom(inX[0]+ Radius, inY[1], inX[1]- Radius, inY[1]);

	B inLineLeft_Up(inX[0], inY[0]+ Radius, inX[0], ((inY[1] - inY[0]) / 2) + inY[0] - Radius);
	B inLineLeft_Down(inX[0], ((inY[1] - inY[0]) / 2) + inY[0] + Radius, inX[0], inY[1]-Radius);
	
	B inLineRight_Up(inX[1], inY[0] + Radius, inX[1], ((inY[1] - inY[0]) / 2) + inY[0] - Radius);
	B inLineRight_Down(inX[1], ((inY[1] - inY[0]) / 2) + inY[0] + Radius, inX[1], inY[1] - Radius);

	//외부경계
	float outX[2] = { inX[0]-20.f,inX[1] + 25.f};
	float outY[2] = { inY[0]-20.f,inY[1] + 20.f};

	B outLineTop(outX[0], outY[0], outX[1], outY[0]);
	B outLineBottom(outX[0], outY[1], outX[1], outY[1]);
	B outLineLeft(outX[0], outY[0], outX[0], outY[1]);
	B outLineRight(outX[1], outY[0], outX[1], outY[1]);

	//포켓 경계 :T(top), L(left),R(right),B(bottom) ,M(mid), O(out), I(in)
	B TLOI_Up(outX[0] + Radius,outY[0], inX[0] + Radius, inY[0]); // 위쪽 라인 왼쪽 부분 경계 위 |
	B TLOI_Down(outX[0], outY[0] + Radius, inX[0], inY[0] + Radius); // 위쪽 라인 왼쪽 부분 경계 아래 -
	
	B TROI_Up(outX[1] - Radius, outY[0], inX[1] - Radius, inY[0]); // 위쪽 라인 오른쪽 부분 경계 위 |
	B TROI_Down(inX[1], inY[0] + Radius, outX[1], outY[0] + Radius); // 위쪽 라인 오른쪽 부분 경계 아래 -

	B MLOI_Up(outX[0], ((inY[1] - inY[0]) / 2) + inY[0] - Radius, 
		inX[0], ((inY[1] - inY[0]) / 2) + inY[0] - Radius); // 왼쪽 라인 가운데 위쪽 -
	B MLOI_Down(outX[0], ((inY[1] - inY[0]) / 2) + inY[0] + Radius, 
		inX[0], ((inY[1] - inY[0]) / 2) + inY[0] + Radius); // 왼쪽 라인 가운데 아래쪽 -

	B MROI_Up(inX[1], ((inY[1] - inY[0]) / 2) + inY[0] - Radius, 
		outX[1], ((inY[1] - inY[0]) / 2) + inY[0] - Radius); // 오른쪽 라인 가운데 위쪽 -
	B MROI_Down(inX[1], ((inY[1] - inY[0]) / 2) + inY[0] + Radius,
		outX[1], ((inY[1] - inY[0]) / 2) + inY[0] + Radius); // 오른쪽 라인 가운데 아래쪽 -

	B BLOI_Up(outX[0], outY[1] - Radius, inX[0], inY[1] - Radius); // 아래쪽 라인 왼쪽 부분 경계 위 -
	B BLOI_Down(inX[0] + Radius, inY[1], outX[0] + Radius, outY[1]); // 위쪽 라인 왼쪽 부분 경계 아래 |

	B BROI_Up(inX[1], inY[1] - Radius,outX[1], outY[1] - Radius); // 위쪽 라인 오른쪽 부분 경계 위 -
	B BROI_Down(inX[1] - Radius, inY[1], outX[1] - Radius, outY[1]); // 위쪽 라인 오른쪽 부분 경계 아래 |

	borderLines.push_back(inLineTop);
	borderLines.push_back(inLineBottom);
	borderLines.push_back(inLineLeft_Up);
	borderLines.push_back(inLineLeft_Down);
	borderLines.push_back(inLineRight_Up);
	borderLines.push_back(inLineRight_Down);

	borderLines.push_back(outLineTop);
	borderLines.push_back(outLineBottom);
	borderLines.push_back(outLineLeft);
	borderLines.push_back(outLineRight);

	borderLines.push_back(TLOI_Up);
	borderLines.push_back(TLOI_Down);
	borderLines.push_back(TROI_Up);
	borderLines.push_back(TROI_Down);

	borderLines.push_back(MLOI_Up);
	borderLines.push_back(MLOI_Down);
	borderLines.push_back(MROI_Up);
	borderLines.push_back(MROI_Down);

	borderLines.push_back(BLOI_Up);
	borderLines.push_back(BLOI_Down);
	borderLines.push_back(BROI_Up);
	borderLines.push_back(BROI_Down);
}

// 소멸자 
SampleBilliardBoard::~SampleBilliardBoard(void)
{
	borderLines.clear();
}

// Sample Game의 객체들은 반드시 상태 갱신 함수 구현해야 함 
void SampleBilliardBoard::update(float timeElapsed)
{
	// Sample Game의 당구대는 상태 갱신 X 
}

// Sample Game의 객체들은 반드시 충돌 물리 구현해야 함
void SampleBilliardBoard::collide(SampleBilliardObject& other)
{
	// Sample Game의 당구대는 고정 오브젝트이기 때문에 충돌 X 
}

// Sample Game의 객체들은 반드시 렌더링 함수 구현해야 함  
void SampleBilliardBoard::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}

// 당구대를 구성하는 경계 요소 반환 
const std::vector<SampleBilliardBoard::Border>& SampleBilliardBoard::getBorders(void) const
{
	return borderLines;
}
