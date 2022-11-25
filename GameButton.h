#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class GameButton {
private:
	//버튼텍스쳐
	sf::Texture			Button;
	sf::Sprite			sprite;
	float width;
	float height;
	float x;
	float y;
	bool visible; //버튼 시각화 여부
public:
	float getPosX() const;
	float getPosY() const;
	float getWidth() const;
	float getHeight() const;
	GameButton(const char* bk, float posX, float posY, float width, float height);
	bool isIntersecting(sf::Vector2f mouse);
	void render(sf::RenderTarget& target);

	void setVisible(bool visible);
	bool isVisible() const;
};