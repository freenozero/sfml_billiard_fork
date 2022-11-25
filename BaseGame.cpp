#include "StartGame.h"
#include "BaseGame.h"

BaseGame::BaseGame(int width, int height, int fpsLimit, int option) : window(nullptr)
{
	switch (option) {
	case EIGHTBALL: //8Ball 
	{
		PlayBGM("��������.wav");
		window = new sf::RenderWindow(sf::VideoMode(width, height), "EightBall", sf::Style::Default);
		window->setFramerateLimit(fpsLimit);

	}
	break;
	default:
	{
		//PlayBGM("????");
		window = new sf::RenderWindow(sf::VideoMode(width, height), "???????", sf::Style::Default);
		window->setFramerateLimit(fpsLimit);
	}
	break;
	}
}

BaseGame::~BaseGame(void)
{
	//��������
	Gamesound.stop(); 
	// ���� ������ ���� 
	delete window;
}

void BaseGame::run(void)
{
	if (window == nullptr)
	{
		// ���� �����찡 �����Ǿ� �־�� �� 
		return;
	}

	// ���� ���� ���� 
	while (window->isOpen())
	{
		sf::Event ev;
		if (window->pollEvent(ev))
		{
			// �Է� �̺�Ʈ ó�� (�ڽ� Ŭ���� ����) 
			handle(ev);
		}

		// ���� ������Ʈ (�ڽ� Ŭ���� ����) 
		update();

		// ������ (�ڽ� Ŭ���� ����) 
		render(*window);

		// ������ ��� ǥ��
		window->display();
	}
}

void BaseGame::PlayBGM(const char* BGM) {
	//������� �ε�
	if (!Gamebuffer.loadFromFile(BGM))
		std::cout << "������ ����� �� �����ϴ�." << std::endl;
	Gamesound.setBuffer(Gamebuffer);
	Gamesound.play();
	Gamesound.setLoop(true);
}
