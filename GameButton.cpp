#include"GameButton.h"

GameButton::GameButton(const char* bk, float posX, float posY, float width, float height) 
	:x(posX),y(posY),width(width),height(height) {
	visible = false;
	//�Ķ���ͷ� ���� �̹����� ��ư�� �����.
	Button.loadFromFile(bk, sf::IntRect(0, 0, width, height));
	sprite.setTexture(Button);
	sprite.setPosition(x, y);
}

void GameButton::render(sf::RenderTarget& target) {
	if(visible)
		target.draw(sprite);
}

bool GameButton::inButton(sf::Vector2f mouse)
{
	// mouse�� XY�� ��ư �ȿ� �������� �˻� 
	if ((mouse.x <= x + width && mouse.x >= x) && (mouse.y <= y + height && mouse.y >= y))
		return true;
	return false;
}

float GameButton::getPosX() const {
	return x;
}
float GameButton::getPosY() const {
	return y;
}

float GameButton::getWidth() const {
	return width;
}
float GameButton::getHeight() const {
	return height;
}

void GameButton::setVisible(bool visible) {
	this->visible = visible;
}
bool GameButton::isVisible() const {
	return visible;
}