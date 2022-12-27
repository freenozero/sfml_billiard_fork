#include "StartGame.h"
#include "BaseGame.h"

BaseGame::BaseGame(int width, int height, int fpsLimit, int option) : window(nullptr)
{
	const sf::Vector2i WinPos(500, 0);

	//�� �浹 �Ҹ�
	if (!BallBuff.loadFromFile("billiard_ball.wav"))
		std::cout << "����Ʈ ���带 �ε��� �� �����ϴ�." << std::endl;
	BallSound.setBuffer(BallBuff);
	BallSound.setLoop(false);

	//���� ���� �Ҹ�
	if (!PocketBuff.loadFromFile("billiard_goal.wav"))
		std::cout << "����Ʈ ���带 �ε��� �� �����ϴ�." << std::endl;
	PocketSound.setBuffer(PocketBuff);
	PocketSound.setLoop(false);

	switch (option) {
	case EIGHTBALL: //8Ball 
	{
		PlayBGM("��������.wav");
		window = new sf::RenderWindow(sf::VideoMode(width, height), "EightBall", sf::Style::Default);
		window->setPosition(WinPos);
		window->setFramerateLimit(fpsLimit);
	}
	break;
	case THREEBALL:
	{
		PlayBGM("��������.wav");
		window = new sf::RenderWindow(sf::VideoMode(width, height), "ThreeBall", sf::Style::Default);
		window->setPosition(WinPos);
		window->setFramerateLimit(fpsLimit);
	}
	break;
	default:
	{
		PlayBGM("��������.wav");
		window = new sf::RenderWindow(sf::VideoMode(width, height), "FourBall", sf::Style::Default);
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

//�� ȿ����
sf::SoundBuffer	BaseGame::PocketBuff;
sf::SoundBuffer	BaseGame::BallBuff;
sf::Sound BaseGame::BallSound;
sf::Sound BaseGame::PocketSound;

void BaseGame::effectBallSound() {
	BallSound.play();
}

void BaseGame::effectPocketSound() {
	PocketSound.play();
}