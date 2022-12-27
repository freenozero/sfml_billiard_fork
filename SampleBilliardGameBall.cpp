#include "SampleBilliardGameBall.h"
#include "SampleGame.h"

bool SampleBilliardGameBall::isIntersecting(sf::Vector2f mouse)
{
	// mouse�� XY�� radius �ȿ� �������� �˻� 

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

	// �� ���� ��ġ���� �˻� 
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
	// ������ �����쿡 ���� �����ϴ� ���ؽ��� �׸� 
	target.draw(getVertices());

	// SampleGame������ �� ���� �÷��̾� ���� ǥ�� 
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
	if (NewCollideBall) //���� 2���� �� ����ٸ�
		return;
	if (CollideBall == nullptr) {
		CollideBall = &ball;
		NewCollideBall = false; //�ʱ�
	}
	else if (CollideBall != &ball) { //���δٸ� ������ ���
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

