#include "SampleBilliardGameBall.h"
#include "SampleGame.h"

bool SampleBilliardGameBall::isIntersecting(sf::Vector2f mouse)
{
	// mouse의 XY가 radius 안에 들어오는지 검사 

	if ((std::powf(mouse.x - getPosition().x, 2.f) + std::powf(mouse.y - getPosition().y, 2.f))
		<= getRadius() * getRadius())
	{
		return true;
	}
	return false;
}

bool SampleBilliardGameBall::isPlayable(void)
{
	return playable;
}

void SampleBilliardGameBall::setPlayable(bool playable)
{
	this->playable = playable;
}

bool SampleBilliardGameBall::collideWithFourBall(SampleBilliardGameBall& other) {

	sf::Vector2f distance = getPosition() - other.getPosition();
	float distanceBetween = (sqrtf((distance.x * distance.x) + (distance.y * distance.y)));

	// 두 공이 겹치는지 검사 
	if (distanceBetween < (getRadius() + other.getRadius()))
	{
		return false;
	}
	else {
		return true;
	}
}

void SampleBilliardGameBall::render(sf::RenderTarget& target)
{
	// 렌더링 윈도우에 원을 구성하는 버텍스를 그림 
	target.draw(getVertices());

	// SampleGame에서는 각 공에 플레이어 문자 표시 
	sf::Text ballText;
	ballText.setFont(SampleGame::getFont());
	ballText.setFillColor(sf::Color::Black);
	ballText.setString(getOwner());
	ballText.setCharacterSize(13);
	ballText.setPosition(getPosition() - sf::Vector2f(4, 9));
	target.draw(ballText);
}


void SampleBilliardGameBall::setOwner(std::string owner)
{
	return SampleBilliardBall::setOwner(owner);
}

bool SampleBilliardGameBall::isOwner(std::string owner)
{
	return SampleBilliardBall::isOwner(owner);
}

std::string SampleBilliardGameBall::getOwner(void)
{
	return SampleBilliardBall::getOwner();
}

void SampleBilliardGameBall::setFoul(bool f) {
	Foul = f;
}
bool SampleBilliardGameBall::isFoul() {
	return Foul;
}

void SampleBilliardGameBall::Select() {
	Selected = true;
}
void SampleBilliardGameBall::DeSelect() {
	Selected = false;
}
bool SampleBilliardGameBall::isSelected() {
	return Selected;
}

void SampleBilliardGameBall::setCollideBoardCnt(int val) {
	CollideBoardCnt = val;
}
int SampleBilliardGameBall::getCollideBoardCnt() {
	return CollideBoardCnt;
}

void SampleBilliardGameBall::setNewCollideBall(SampleBilliardBall& ball) {
	if (NewCollideBall) //적구 2개를 다 맞췄다면
		return;
	if (CollideBall == nullptr) {
		CollideBall = &ball;
		NewCollideBall = false; //초기
	}
	else if (CollideBall != &ball) { //서로다른 적구일 경우
		NewCollideBall = true;
		CollideBall = nullptr;
	}
	else
		NewCollideBall = false;
}


bool SampleBilliardGameBall::isNewCollideBall() {
	return NewCollideBall;
}

void SampleBilliardGameBall::InitNewCollideBall() {
	CollideBall = nullptr;
	NewCollideBall = false;
}



void SampleBilliardGameBall::setTrueCB() {
	NewCollideBall = true;
}

