#include "SampleBilliardBoard.h"

SampleBilliardBoard::SampleBilliardBoard(void)
{
	// Sample Game�� ���� �籸�� �ؽ�ó �̹��� �ε� 
	texture.loadFromFile("textureBoard.png", sf::IntRect(1, 1, 454, 848));

	// ��������Ʈ ���� 
	sprite.setTexture(texture);
	sprite.setPosition(572.5f, 25.f);

	// �籸�� ��� �ʱ�ȭ
	
	//���� �ݰ游ŭ �籸�� ������ �����˴ϴ�.
	/*
		���� x,y��ǥ�ϰ�
		�ܺ� x,y��ǥ
		�� �ݰ游 �����ؼ� ��밡���մϴ�.
	*/
	float Radius = 20.f;
	Radius = Radius + 5.f; //���� �ݰ� + ����
	
	//���� ���
	float inX[2] = { 602.5f,992.f };
	float inY[2] = { 55.f,840.f };
	typedef SampleBilliardBoard::Border B;
	B inLineTop(inX[0]+Radius, inY[0], inX[1]- Radius, inY[0]);
	B inLineBottom(inX[0]+ Radius, inY[1], inX[1]- Radius, inY[1]);

	B inLineLeft_Up(inX[0], inY[0]+ Radius, inX[0], ((inY[1] - inY[0]) / 2) + inY[0] - Radius);
	B inLineLeft_Down(inX[0], ((inY[1] - inY[0]) / 2) + inY[0] + Radius, inX[0], inY[1]-Radius);
	
	B inLineRight_Up(inX[1], inY[0] + Radius, inX[1], ((inY[1] - inY[0]) / 2) + inY[0] - Radius);
	B inLineRight_Down(inX[1], ((inY[1] - inY[0]) / 2) + inY[0] + Radius, inX[1], inY[1] - Radius);

	//�ܺΰ��
	float outX[2] = { inX[0]-20.f,inX[1] + 25.f};
	float outY[2] = { inY[0]-20.f,inY[1] + 20.f};

	B outLineTop(outX[0], outY[0], outX[1], outY[0]);
	B outLineBottom(outX[0], outY[1], outX[1], outY[1]);
	B outLineLeft(outX[0], outY[0], outX[0], outY[1]);
	B outLineRight(outX[1], outY[0], outX[1], outY[1]);

	//���� ��� :T(top), L(left),R(right),B(bottom) ,M(mid), O(out), I(in)
	B TLOI_Up(outX[0] + Radius,outY[0], inX[0] + Radius, inY[0]); // ���� ���� ���� �κ� ��� �� |
	B TLOI_Down(outX[0], outY[0] + Radius, inX[0], inY[0] + Radius); // ���� ���� ���� �κ� ��� �Ʒ� -
	
	B TROI_Up(outX[1] - Radius, outY[0], inX[1] - Radius, inY[0]); // ���� ���� ������ �κ� ��� �� |
	B TROI_Down(inX[1], inY[0] + Radius, outX[1], outY[0] + Radius); // ���� ���� ������ �κ� ��� �Ʒ� -

	B MLOI_Up(outX[0], ((inY[1] - inY[0]) / 2) + inY[0] - Radius, 
		inX[0], ((inY[1] - inY[0]) / 2) + inY[0] - Radius); // ���� ���� ��� ���� -
	B MLOI_Down(outX[0], ((inY[1] - inY[0]) / 2) + inY[0] + Radius, 
		inX[0], ((inY[1] - inY[0]) / 2) + inY[0] + Radius); // ���� ���� ��� �Ʒ��� -

	B MROI_Up(inX[1], ((inY[1] - inY[0]) / 2) + inY[0] - Radius, 
		outX[1], ((inY[1] - inY[0]) / 2) + inY[0] - Radius); // ������ ���� ��� ���� -
	B MROI_Down(inX[1], ((inY[1] - inY[0]) / 2) + inY[0] + Radius,
		outX[1], ((inY[1] - inY[0]) / 2) + inY[0] + Radius); // ������ ���� ��� �Ʒ��� -

	B BLOI_Up(outX[0], outY[1] - Radius, inX[0], inY[1] - Radius); // �Ʒ��� ���� ���� �κ� ��� �� -
	B BLOI_Down(inX[0] + Radius, inY[1], outX[0] + Radius, outY[1]); // ���� ���� ���� �κ� ��� �Ʒ� |

	B BROI_Up(inX[1], inY[1] - Radius,outX[1], outY[1] - Radius); // ���� ���� ������ �κ� ��� �� -
	B BROI_Down(inX[1] - Radius, inY[1], outX[1] - Radius, outY[1]); // ���� ���� ������ �κ� ��� �Ʒ� |

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

// �Ҹ��� 
SampleBilliardBoard::~SampleBilliardBoard(void)
{
	borderLines.clear();
}

// Sample Game�� ��ü���� �ݵ�� ���� ���� �Լ� �����ؾ� �� 
void SampleBilliardBoard::update(float timeElapsed)
{
	// Sample Game�� �籸��� ���� ���� X 
}

// Sample Game�� ��ü���� �ݵ�� �浹 ���� �����ؾ� ��
void SampleBilliardBoard::collide(SampleBilliardObject& other)
{
	// Sample Game�� �籸��� ���� ������Ʈ�̱� ������ �浹 X 
}

// Sample Game�� ��ü���� �ݵ�� ������ �Լ� �����ؾ� ��  
void SampleBilliardBoard::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}

// �籸�븦 �����ϴ� ��� ��� ��ȯ 
const std::vector<SampleBilliardBoard::Border>& SampleBilliardBoard::getBorders(void) const
{
	return borderLines;
}
