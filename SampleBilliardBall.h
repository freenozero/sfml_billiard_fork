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
	// 마찰에 따른 감속 상수 
	const float VISCOSITY;

	// 렌더링을 위한 버텍스 상수 
	const int NUMVERTICES;

public:
	// 생성자 
	SampleBilliardBall(void);
	SampleBilliardBall(sf::Vector2f center, float radius, sf::Color color);
	SampleBilliardBall(const SampleBilliardBall&);

	// 소멸자 
	virtual ~SampleBilliardBall(void);

	// 연산자 
	SampleBilliardBall& operator=(const SampleBilliardBall&);

	// Sample Game의 객체들은 반드시 상태 갱신 함수 구현해야 함 
	virtual void update(float timeElapsed);

	// Sample Game의 객체들은 반드시 충돌 물리 구현해야 함
	virtual void collide(SampleBilliardObject& other);

	// Sample Game의 객체들은 반드시 렌더링 함수 구현해야 함  
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
	// 공과 충돌한 경우 
	void collideWithBall(SampleBilliardBall& other);

	// 당구대와 충돌한 경우 
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