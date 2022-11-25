#include "StartGame.h"


StartGame::StartGame(int width, int height, int fpsLimit) 
	: Width(width), Height(height), FpsLimit(fpsLimit), option(0), isChangeBK(false), interval(0), time(0)
{	
	std::cout << "GameWindow Loading..." << std::endl;

	//음악설정
	if (!Startbuffer.loadFromFile("로비음악.wav"))
		std::cout << "음악을 재생할 수 없습니다." << std::endl;
	Startsound.setBuffer(Startbuffer);
	Startsound.play();
	Startsound.setLoop(true);

	//윈도우 초기값 설정
	window = new sf::RenderWindow(sf::VideoMode(500, 500), "Billiard Game", sf::Style::Default);
	window->setFramerateLimit(fpsLimit);

	system("cls");
	std::cout << "Loaded Game" << std::endl;

	texture.loadFromFile("logo.png", sf::IntRect(0, 0, 500, 500));
	ButtonT1.loadFromFile("ButtonT1.png", sf::IntRect(0, 0, 225, 25));
	ButtonT2.loadFromFile("ButtonT2.png", sf::IntRect(0, 0, 225, 25));
	ButtonT3.loadFromFile("ButtonT3.png", sf::IntRect(0, 0, 225, 25));
	ButtonT4.loadFromFile("ButtonT4.png", sf::IntRect(0, 0, 225, 25));
	BackGround.loadFromFile("empty.png", sf::IntRect(0, 0, 500, 500));
	sprite.setTexture(texture);
	sprite.setPosition(0, 0);

	//버튼 추가 >> 인자로 이미지이름,x좌표,y좌표,너비,높이를 받음
	GBBW.push_back(new GameButton("eightball_bw.png", 20, 40, 164, 149));
	GB.push_back(new GameButton("eightball.png",20,40,164,149));

	Tnum = T1;
}

bool StartGame::Close = false;

bool StartGame::isClose() {
	return Close;
}

StartGame::~StartGame(void)
{
	Startsound.stop(); //음악중지
	for (GameButton* g : GBBW) {
		delete g;
	}
	for (GameButton* g : GB) {
		delete g;
	}
	delete window;
	//창 닫고 게임 불러옴
	if(option>0){
		BaseGame&& game = SampleGame(Width, Height, FpsLimit,option);
		game.run();
		option = 0; //기본
	}
	std::cout << "Exit" << std::endl;
}

void StartGame::handle(sf::Event& ev)
{
	switch (ev.type) {
	case sf::Event::Closed:
		Close = true; //x버튼으로 닫을 때만 꺼지도록
		window->close();
		break;
	case sf::Event::MouseMoved:
		// 마우스 움직임 이벤트 
		mouseXY.x = (float)ev.mouseMove.x;
		mouseXY.y = (float)ev.mouseMove.y;
		
		for (GameButton* g : GB) {
			if (g->isIntersecting(mouseXY))
				g->setVisible(true);
			else
				g->setVisible(false);
		}
		for (GameButton* g : GBBW) {
			if (g->isIntersecting(mouseXY))
				g->setVisible(false);
			else
				g->setVisible(true);
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (ev.mouseButton.button == sf::Mouse::Left) {
			if(!isChangeBK){ //변경전
				//여기에서 게임 선택
				sprite.setTexture(BackGround);
				sprite.setPosition(0, 0);
				isChangeBK=true; //화면 변경
			}
			else { //변경되었을 때
				//버튼1 안에서 좌클릭 발생 시
				if (GB[0]->isIntersecting(mouseXY)){
					option = EIGHTBALL; //EIGHTBALL게임 실행
					window->close();
				}
				else {
					//버튼 2, 3 에 대한 옵션	
				}
			}
		}
		if (ev.mouseButton.button == sf::Mouse::Right) {
			if (!isChangeBK) { 
				//nothing...
			}
			else { 
				sprite.setTexture(texture);
				sprite.setPosition(0, 0);
				isChangeBK = false; //화면 변경
			}
		}
	}
}

void StartGame::Info(void)
{
	if (window == nullptr)
	{
		//윈도우 생성여부
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

		// 렌더링 (자식 클래스 구현) 
		render(*window);

		// 렌더링 결과 표시
		window->display();
	}
}

void StartGame::render(sf::RenderTarget& target)
{	
	float xPosition = 130;
	float yPosition = 450;
	//배경 변경 전
	if (!isChangeBK)
	{
		time = clock.getElapsedTime().asSeconds();
		interval += time;
		if (interval > 0.1)
		{
			switch (Tnum) {
			case T1:
			{
				Bsprite.setTexture(ButtonT2);
				Bsprite.setPosition(xPosition, yPosition);
				Tnum = T2;
			}
			break;
			case T2:
			{
				Bsprite.setTexture(ButtonT3);
				Bsprite.setPosition(xPosition, yPosition);
				Tnum = T3;
			}
			break;
			case T3:
			{
				Bsprite.setTexture(ButtonT4);
				Bsprite.setPosition(xPosition, yPosition);
				Tnum = T4;
			}
			break;
			case T4:
			{
				Bsprite.setTexture(ButtonT1);
				Bsprite.setPosition(xPosition, yPosition);
				Tnum = T1;
			}
			break;
			}
			interval = 0;
		}
		target.draw(sprite);
		target.draw(Bsprite);
		clock.restart(); //재시작
	}
	else { //게임 선택 화면
		target.draw(sprite);
		for (GameButton* g : GB)
			g->render(target);
		for (GameButton* g : GBBW)
			g->render(target);
	}
}