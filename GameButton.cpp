#include"GameButton.h"

GameButton::GameButton(const char* bk, float posX, float posY, float width, float height) 
	:x(posX),y(posY),width(width),height(height) {
	visible = false;
	//파라미터로 받은 이미지로 버튼을 만든다.
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
	// mouse의 XY가 버튼 안에 들어오는지 검사 
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