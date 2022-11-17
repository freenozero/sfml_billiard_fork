#pragma once

class SampleBilliardObject
{
public:
	// Sample Game의 객체들은 반드시 상태 갱신 함수 구현해야 함 
	virtual void update(float timeElapsed) = 0;

	// Sample Game의 객체들은 반드시 충돌 물리 구현해야 함
	virtual void collide(SampleBilliardObject& other) = 0;

	// Sample Game의 객체들은 반드시 렌더링 함수 구현해야 함  
	virtual void render(sf::RenderTarget& target) = 0;
};