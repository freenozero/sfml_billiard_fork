#pragma once
#include <iostream>
#include <vector> 

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "SampleBilliardObject.h"
#include "Player.h"

class ScoreBoard : public SampleBilliardObject
{
public:
	// ������ 
	ScoreBoard();

	// �Ҹ��� 
	~ScoreBoard(void);

	// Sample Game�� ��ü���� �ݵ�� ���� ���� �Լ� �����ؾ� �� 
	virtual void update(float timeElapsed);

	// Sample Game�� ��ü���� �ݵ�� �浹 ���� �����ؾ� ��
	virtual void collide(SampleBilliardObject& other);

	// Sample Game�� ��ü���� �ݵ�� ������ �Լ� �����ؾ� ��  
	virtual void render(sf::RenderTarget& target);

private:
	//ū ������
	sf::RectangleShape mainScoreBoard;
	//�÷��̾� ������
	sf::RectangleShape player1Board;
	sf::RectangleShape player2Board;
};