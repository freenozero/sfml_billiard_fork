#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class BaseGame
{
public: 
	// ���� ��ü �ʱ�ȭ 
	BaseGame(int width, int height, int fpsLimit);

	// ���� ��ü ���� 
	virtual ~BaseGame(void);

	// ���� ���� ���� ���� 
	void run(void); 

protected: 
	// ��� Ŭ������ �ݵ�� �̺�Ʈ �ڵ鸵 �Լ� �����ؾ� �� 
	virtual void handle(sf::Event& ev) = 0;

	// ��� Ŭ������ �ݵ�� ���� ���� ���� �Լ� �����ؾ� �� 
	virtual void update(void) = 0;

	// ��� Ŭ������ �ݵ�� ��ü ������ �Լ� �����ؾ� �� 
	virtual void render(sf::RenderTarget& target) = 0;

protected:
	// ���� ������ Ÿ��
	sf::RenderWindow* window;
};