#include "SampleGame.h"
#include "SampleBilliardBoard.h"
#include "SampleBilliardGameBall.h"
#include "BilliardPocket.h"
#include "SampleBilliardObject.h"
#include "ScoreBoard.h"

SampleGame::SampleGame(int width, int height, int fpsLimit)
	:BaseGame(width, height, fpsLimit), isDraggingBall(false), draggedBall(nullptr)
{
	// SampleGame을 위한 인터페이스 생성 및 등록 

	// SampleGame을 위한 당구대 생성 및 등록 
	gameObjects.push_back(new SampleBilliardBoard()); //0번

	//포켓 1~6번
	BilliardPocket* Pocket[6];
	//포켓 x,y 좌표 한 번에 변경 가능하도록 해놨습니다.
	float x[2] = { 595,1002 };
	float y[3] = { 48,447,850 };

	sf::Vector2f PocketCord[6] = {
	{x[0], y[0]},{x[0], y[1]},{x[0], y[2]},
	{x[1], y[0]},{x[1], y[1]},{x[1], y[2]},
	};
	for (int i = 0; i < 6; ++i) {
		Pocket[i] = new BilliardPocket(PocketCord[i], 20, sf::Color::Black);
		Pocket[i]->setOwner("0");
		gameObjects.push_back(Pocket[i]);
	}

	// SampleGame을 위한 당구공 생성 및 등록 
	
	//플레이어볼
	SampleBilliardGameBall* PlayerBall =
		new SampleBilliardGameBall(sf::Vector2f(800, 500), 10, sf::Color::White);
	PlayerBall->setOwner("P");
	PlayerBall->setPlayable(true);
	gameObjects.push_back(PlayerBall);

	//게임볼 8~22번까지 
	SampleBilliardBall* ball[15];
	typedef sf::Color C;

	//색 매핑
	//1~7번 공은 단색공 = 총 7개
    //9번~15번 공은 줄무늬 공 = 총 7개
    //노랑, 남색, 빨강, 보라(핑크), 주황, 초록, 갈색  = 총 7개
	C color[15] = {
    C::Yellow, C::Color(0,0,128), C::Red,
    C::Color(77,55,123), C::Color(255,165,0), C::Green,
    C::Color(111, 79, 48),C::Black,C::Yellow,
    C::Color(0,0,128), C::Red,C::Color(77,55,123),
    C::Color(255,165,0), C::Green,  C::Color(111, 79, 48),
	};
	//공 좌표 매핑
	//1번은 맨 앞에, 8번은 가운데
    //2번과 3번은 양쪽 끝에
    //1번 다음 배열은 띠공-띠공
    //중간배열은 색꽁-띠공 번갈아가면서
	sf::Vector2f Cord[15] =
	{ {800, 300},{840, 260},{760, 260},
	{780, 280},{810, 270},{770, 270},
	{800, 260},{800, 280},{830, 270},
	{790, 290},{780, 260},{790, 270},
	{820, 260},{810, 290},{820, 280} };

	//배열 초기화
	for (int i = 0; i < 15; ++i) {
		ball[i] = new SampleBilliardBall(Cord[i], 10, color[i]);
		ball[i]->setOwner(std::to_string(i+1)); 
		gameObjects.push_back(ball[i]);
	}

	//플레이어 정보
	int PlayerCnt = 2; //플레이어 수
	srand(time(NULL));
	Player* p;
	//첫번째 턴 난수
	int FirstTurn = rand() % PlayerCnt; 
	for (int i = 0; i < 2; ++i) {
		if(FirstTurn==i){
			p = new Player(i+1, true);
		}
		else{
			p = new Player(i+1, false);
		}
		Players.push_back(p);
	}


	// 점수판 생성 및 등록
	gameObjects.push_back(new ScoreBoard());
	
}

SampleGame::~SampleGame(void)
{
	// UI 인스턴스 해제  
	// 게임 오브젝트들 해제 
	for (SampleBilliardObject* obj : gameObjects)
	{
		delete obj;
	}
	for (Player* p : Players) {
		delete p;
	}

}

sf::Font* SampleGame::font = nullptr;
const sf::Font& SampleGame::getFont(void)
{
	if (font == nullptr) {
		font = new sf::Font;
		font->loadFromFile("Arial.ttf");
	}
	return *font;
}

// 상속 클래스는 반드시 이벤트 핸들링 함수 구현해야 함 
void SampleGame::handle(sf::Event& ev)
{
	// SampleGame을 위한 입력 이벤트 처리 
	switch (ev.type)
	{
	case sf::Event::Closed:
		// 윈도우의 x 버튼 누르면 종료한다 
		window->close();
		break;
	case sf::Event::KeyPressed:
		// 키보드 이벤트 
		if (ev.key.code == sf::Keyboard::Escape)
		{
			// TODO: game paused 
			// TODO: call sample GUI 
		}
		break;
	case sf::Event::MouseMoved:
		// 마우스 움직임 이벤트 
		mouseXY.x = (float)ev.mouseMove.x;
		mouseXY.y = (float)ev.mouseMove.y;
		break;
	case sf::Event::MouseButtonPressed:
		// 마우스 버튼 누름 이벤트
		if (ev.mouseButton.button == sf::Mouse::Left)
		{
			for (SampleBilliardObject* obj : gameObjects)
			{
				//공이 움직이고 있는 상태라면 버튼 누름 이벤트발생X
				if (Players[0]->isPhase() == MOVE || Players[1]->isPhase() == MOVE)
					continue;

				// SampleBilliardBall의 인스턴스가 아닌 경우 pass
				SampleBilliardGameBall* gameBall = dynamic_cast<SampleBilliardGameBall*>(obj);
				if (gameBall == nullptr)
				{
					continue;
				}

				// 커서가 공의 내부가 아닌 경우 pass 
				if (!gameBall->isIntersecting(mouseXY))
				{
					continue;
				}

				// 공이 Playable이 아닌 경우 pass 
				if (!gameBall->isPlayable()) {
					continue;
				}

				// 드래그 가능한 공 임시 저장 
				draggedBall = gameBall;
				isDraggingBall = true;
			}
		}
		break;
	case sf::Event::MouseButtonReleased:
		// 마우스 버튼 뗌 이벤트 
		if (ev.mouseButton.button == sf::Mouse::Left && isDraggingBall)
		{
			isDraggingBall = false;
		}
		break;
	}
}

// 상속 클래스는 반드시 게임 상태 갱신 함수 구현해야 함 
void SampleGame::update(void)
{
	// 플레이어 서로의 주소 추가
	if (&(Players[0]->getNextP()) == nullptr) Players[0]->setNextP(*Players[1]);
	if (&(Players[1]->getNextP()) == nullptr) Players[1]->setNextP(*Players[0]);

	//필요한 공
	SampleBilliardGameBall* playerBall = nullptr;
	SampleBilliardObject* eightBall = nullptr;
	
	//모든 공의 속도
	int Velocity = 0;
	
	// 게임 오브젝트 업데이트 
	for (SampleBilliardObject* obj : gameObjects)
	{
		obj->update(clock.getElapsedTime().asSeconds());

		//플레이어볼 추출
		if (playerBall==nullptr)
			playerBall = dynamic_cast<SampleBilliardGameBall*>(obj);
		//8번공 추출
		if (eightBall == nullptr) {
			SampleBilliardBall* eight = dynamic_cast<SampleBilliardBall*>(obj);
			if (eight!=nullptr&&eight->getOwner().compare("8") == 0)
				eightBall = obj;
		}
		//공 오브젝트의 속도 구하기
		if (dynamic_cast<SampleBilliardBall*>(obj) != nullptr
			&& dynamic_cast<BilliardPocket*>(obj) == nullptr) {
			SampleBilliardBall& Ball = *dynamic_cast<SampleBilliardBall*>(obj);
			if (Ball.getVelocity() != sf::Vector2f(0.f, 0.f))
				Velocity = 1;
		}
	}
	//오브젝트 업데이트 후에 값이 없으면 종료
	if (playerBall == nullptr|| eightBall == nullptr) exit(-1);  

	//플레이어 업데이트, 점수판 업데이트
	for (Player* p : Players)
	{
		p->update(*playerBall, *eightBall, Velocity);
	}
	
	// 게임 오브젝트 충돌 검사
	for (SampleBilliardObject* obj1 : gameObjects)
	{
		for (SampleBilliardObject* obj2 : gameObjects)
		{
			obj1->collide(*obj2);
		}
	}

	// 끌었다가 놓은 공에 속도를 지정하고 표시 해제
	if (!isDraggingBall && draggedBall != nullptr)
	{
		draggedBall->setVelocity(draggedBall->getPosition().x - mouseXY.x, draggedBall->getPosition().y - mouseXY.y);
		draggedBall = nullptr;
	}



	// 다음 단위 시간을 위해 초기화 
	clock.restart();
}


void SampleGame::run(void)
{
	if (window == nullptr)
	{
		// 게임 윈도우가 생성되어 있어야 함 
		return;
	}

	// 메인 게임 루프 
	while (window->isOpen())
	{
		sf::Time timeout = sf::seconds(10); //10초면 다음 플레이어에게 턴 넘김
		sf::Time sec = playerClock.getElapsedTime();

		if (timeout < sec) {
			
			if (Players[0]->isTurn()) {
				Players[0]->setTurn(false); //턴 종료
				Players[0]->getNextP().setTurn(true); //다음 플레이어에게 턴넘김
			}
			else {
				Players[1]->setTurn(false); //턴 종료
				Players[1]->getNextP().setTurn(true); //다음 플레이어에게 턴넘김
			}
			playerClock.restart();
		}

		if (Players[0]->isPhase() == MOVE || Players[1]->isPhase() == MOVE) //공이 움직이고 있을 때는 계속 게임 시간 restart
			playerClock.restart();

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


// 상속 클래스는 반드시 객체 렌더링 함수 구현해야 함 
void SampleGame::render(sf::RenderTarget& target)
{
	// 화면 클리어 
	window->clear(sf::Color(75, 103, 163));

	// 플레이어 정보 렌더링
	for (Player* p : Players)
	{
		p->render(target);
	}

	// 게임 오브젝트 렌더링 
	for (SampleBilliardObject* obj : gameObjects)
	{
		obj->render(target);
	}

	// 공을 드래그 하면 세기 표시 (길이 및 색)
	renderDragpower(target);

	// 타이머 표시
	sf::Text Timer;
	Timer.setFont(SampleGame::getFont());
	Timer.setFillColor(sf::Color::White);
	Timer.setCharacterSize(30);

	Timer.setString(std::to_string(playerClock.getElapsedTime().asSeconds()));
	Timer.setPosition(20.f, 20.f);
	target.draw(Timer);
}

void SampleGame::renderDragpower(sf::RenderTarget& target)
{
	if (isDraggingBall)
	{
		sf::Vector2f distance = (mouseXY - draggedBall->getPosition());
		float distanceBetween = sqrt(distance.x * distance.x + distance.y * distance.y);
		sf::Vector2f invert = distance / distanceBetween; //단위벡터?
		sf::Color directionColor = sf::Color(255, (255 - ((int)distanceBetween / 2) % 255), 0);
		if (distanceBetween > 510)
		{
			directionColor = sf::Color::Red;
		}

		sf::Vector2f toSubtract(distanceBetween * invert.x, distanceBetween * invert.y); //반대방향
		sf::Vertex start = sf::Vertex(draggedBall->getPosition(), directionColor);
		sf::Vertex end = sf::Vertex(draggedBall->getPosition() - toSubtract, directionColor);

		sf::VertexArray points;
		points.setPrimitiveType(sf::LineStrip);
		points.append(start);
		points.append(end);
		target.draw(points);
	}
}