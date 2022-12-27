#include "StartGame.h"
#include "BaseGame.h"

BaseGame::BaseGame(int width, int height, int fpsLimit, int option) : window(nullptr)
{
	const sf::Vector2i WinPos(500, 0);

	//공 충돌 소리
	if (!BallBuff.loadFromFile("billiard_ball.wav"))
		std::cout << "이펙트 사운드를 로딩할 수 없습니다." << std::endl;
	BallSound.setBuffer(BallBuff);
	BallSound.setLoop(false);

	//포켓 들어가는 소리
	if (!PocketBuff.loadFromFile("billiard_goal.wav"))
		std::cout << "이펙트 사운드를 로딩할 수 없습니다." << std::endl;
	PocketSound.setBuffer(PocketBuff);
	PocketSound.setLoop(false);

	switch (option) {
	case EIGHTBALL: //8Ball 
	{
		PlayBGM("게임음악.wav");
		window = new sf::RenderWindow(sf::VideoMode(width, height), "EightBall", sf::Style::Default);
		window->setPosition(WinPos);
		window->setFramerateLimit(fpsLimit);
	}
	break;
	case THREEBALL:
	{
		PlayBGM("게임음악.wav");
		window = new sf::RenderWindow(sf::VideoMode(width, height), "ThreeBall", sf::Style::Default);
		window->setPosition(WinPos);
		window->setFramerateLimit(fpsLimit);
	}
	break;
	default:
	{
		PlayBGM("게임음악.wav");
		window = new sf::RenderWindow(sf::VideoMode(width, height), "FourBall", sf::Style::Default);
		window->setFramerateLimit(fpsLimit);
	}
	break;
	}
}

BaseGame::~BaseGame(void)
{
	//음악중지
	Gamesound.stop();
	// 게임 윈도우 해제 
	delete window;
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
		if (window->pollEvent(ev))
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

void BaseGame::PlayBGM(const char* BGM) {
	//배경음악 로드
	if (!Gamebuffer.loadFromFile(BGM))
		std::cout << "음악을 재생할 수 없습니다." << std::endl;
	Gamesound.setBuffer(Gamebuffer);
	Gamesound.play();
	Gamesound.setLoop(true);
}

//공 효과음
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