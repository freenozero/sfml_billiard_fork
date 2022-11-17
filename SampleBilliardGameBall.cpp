#include "SampleBilliardGameBall.h"
#include "SampleGame.h"

bool SampleBilliardGameBall::isIntersecting(sf::Vector2f mouse)
{
	// mouse�� XY�� radius �ȿ� �������� �˻�a

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