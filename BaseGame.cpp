#include "BaseGame.h"

BaseGame::BaseGame(int width, int height, int fpsLimit) : window(nullptr)
{
	// 게임 윈도우 초기화 
	window = new sf::RenderWindow(sf::VideoMode(width, height), "Billiard Game", sf::Style::Default);
	window->setFramerateLimit(fpsLimit);
}

BaseGame::~BaseGame(void)
{
	// 게임 윈도우 해제 
	//delete window;
}

void BaseGame::run(void)
{
	if (window == nullptr)
	{
		// 게임 윈도우가 생성되어 있어야 함 
		return;
	}

	// 메인 게임 루프 
	while (window->isOpen())
	{
		sf::Event ev;
		while (window->pollEvent(ev))
		{
			// 입력 이벤트 처리 (자식 클래스 구현) 
			handle(ev);
		}

		// 게임 업데이트 (자식 클래스 구현) 
		update();

		// 렌더링 (자식 클래스 구현) 
		render(*window);

		// 렌더링 결과 표시
		window->display();
	}
}