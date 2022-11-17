#include "ScoreBoard.h"

ScoreBoard::ScoreBoard()
{
	//��ü���� ������
	sf::RectangleShape rec;
	rec.setSize(sf::Vector2f(500, 200));
	rec.setOutlineColor(sf::Color::Black);
	rec.setOutlineThickness(5);
	rec.setPosition(5.f, 450.f);

	this->mainScoreBoard = rec;

	//payer1
	sf::RectangleShape rec2;
	rec2.setSize(sf::Vector2f(450, 100));
	rec2.setOutlineColor(sf::Color::Black);
	rec2.setOutlineThickness(5);
	rec2.setPosition(30.f, 450.f);

	this->player1Board = rec2;

	//payer2
	sf::RectangleShape rec3;
	rec3.setSize(sf::Vector2f(450, 100));
	rec3.setOutlineColor(sf::Color::Black);
	rec3.setOutlineThickness(5);
	rec3.setPosition(30.f, 550.f);
	this->player2Board = rec3;
}

ScoreBoard::~ScoreBoard(void)
{

}

// Sample Game�� ��ü���� �ݵ�� ���� ���� �Լ� �����ؾ� �� 
void ScoreBoard::update(float timeElapsed)
{
	//payer���� ���� ���� ��� ���� ����

}

// Sample Game�� ��ü���� �ݵ�� �浹 ���� �����ؾ� ��
void ScoreBoard::collide(SampleBilliardObject& other)
{
	// Sample Game�� ������ ���� ������Ʈ�̱� ������ �浹 X 
}

// Sample Game�� ��ü���� �ݵ�� ������ �Լ� �����ؾ� ��  
void ScoreBoard::render(sf::RenderTarget& target)
{
	target.draw(mainScoreBoard);
	target.draw(player1Board);
	target.draw(player2Board);
}

