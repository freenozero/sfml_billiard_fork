#include "ScoreBoard.h"

ScoreBoard::ScoreBoard()
{
	//전체적인 점수판
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

// Sample Game의 객체들은 반드시 상태 갱신 함수 구현해야 함 
void ScoreBoard::update(float timeElapsed)
{
	//payer들이 공을 넣을 경우 상태 갱신

}

// Sample Game의 객체들은 반드시 충돌 물리 구현해야 함
void ScoreBoard::collide(SampleBilliardObject& other)
{
	// Sample Game의 점수판 고정 오브젝트이기 때문에 충돌 X 
}

// Sample Game의 객체들은 반드시 렌더링 함수 구현해야 함  
void ScoreBoard::render(sf::RenderTarget& target)
{
	target.draw(mainScoreBoard);
	target.draw(player1Board);
	target.draw(player2Board);
}

