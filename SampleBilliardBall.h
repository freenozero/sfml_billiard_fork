#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include "SampleBilliardObject.h"
#include "SampleBilliardBoard.h"

class SampleBilliardBall : public SampleBilliardObject
{
public:
	// ������ ���� ���� ��� 
	const float VISCOSITY;

	// �������� ���� ���ؽ� ��� 
	const int NUMVERTICES;

public:
	// ������ 
	SampleBilliardBall(void);
	SampleBilliardBall(sf::Vector2f center, float radius, sf::Color color);
	SampleBilliardBall(const SampleBilliardBall&);

	// �Ҹ��� 
	virtual ~SampleBilliardBall(void);

	// ������ 
	SampleBilliardBall& operator=(const SampleBilliardBall&);

	// Sample Game�� ��ü���� �ݵ�� ���� ���� �Լ� �����ؾ� �� 
	virtual void update(float timeElapsed);

	// Sample Game�� ��ü���� �ݵ�� �浹 ���� �����ؾ� ��
	virtual void collide(SampleBilliardObject& other);

	// Sample Game�� ��ü���� �ݵ�� ������ �Լ� �����ؾ� ��  
	virtual void render(sf::RenderTarget& target);

	// setter 
	void setPosition(float x, float y);
	void setPosition(sf::Vector2f position);
	void setAcceleration(sf::Vector2f acceleration);
	void setVelocity(float x, float y);
	void setVelocity(sf::Vector2f velocity);
	void setAngle(float angle);
	void setRadius(float radius);
	void setMass(float mass);
	void setColor(sf::Color color);

	// getter 
	sf::Vector2f	getPosition(void) const;
	sf::Vector2f	getAcceleration(void) const;
	sf::Vector2f	getVelocity(void) const;
	float getAngle(void) const;
	float getRadius(void) const;
	float getMass(void) const;
	sf::Color getColor(void) const;
	const sf::VertexArray& getVertices(void) const;
	void setOwner(std::string owner);
	bool isOwner(std::string owner);
	std::string getOwner(void); 

private:
	// ���� �浹�� ��� 
	void collideWithBall(SampleBilliardBall& other);

	// �籸��� �浹�� ��� 
	void collideWithBoard(SampleBilliardBoard& other);

private:
	sf::Vector2f position;
	sf::Vector2f acceleration;
	sf::Vector2f velocity;

	float angle;
	float radius;
	float mass;

	sf::VertexArray vertices;
	sf::Color color;

	std::string owner;
};