#include "SampleGame.h"
#include "SampleBilliardBoard.h"
#include "SampleBilliardGameBall.h"
#include "BilliardPocket.h"
#include "SampleBilliardObject.h"
#include "ScoreBoard.h"
#include "StartGame.h"


SampleGame::SampleGame(int width, int height, int fpsLimit,int option)
	: BaseGame(width, height, fpsLimit,option), playerBall(nullptr), isDraggedMouse(false)
{
	//게임 로그 남기도록
	system("cls");
	std::cout << "[Log]" << std::endl;
	std::cout << "Start Game" << std::endl;
	srand(time(NULL));

	//배경로드
	tBackGround.loadFromFile("배경.png");
	sBackGround.setTexture(tBackGround);

	//큐대 로드
	CueTexture.loadFromFile("cue.png");
	CueSprite.setTexture(CueTexture);
	CueSprite.setScale(0.2f, 0.2f);
	CueSprite.setOrigin(0, 65.f);
	//큐대의 방향벡터
	CueDirvec = { 1,0 };

	//턴 시간
	TurnTime = 10;

	makeBilliardObject();

	ExitWindow = new GameExit(window->getSize().x, window->getSize().y);
}

void SampleGame::makeBilliardObject()
{
	// 테스트 코드 변수 초기화
	//isCatchingBall = false;
	//catchedBall = nullptr;

	// SampleGame을 위한 인터페이스 생성 및 등록 

	// SampleGame을 위한 당구대 생성 및 등록 
	gameObjects.push_back(new SampleBilliardBoard()); //0번

	// 점수판 생성 및 등록 => 오브젝트 렌더링이 추가한 순서라서 먼저해야 됨.
	gameObjects.push_back(new ScoreBoard());

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
	
	//1번공 기준 좌표
	sf::Vector2f one = { 800,300 };
	//반경
	float R = 12;
	float Sqr3 = sqrt(3); //루트3

	//플레이어볼
	SampleBilliardGameBall* PlayerBall =
		new SampleBilliardGameBall(sf::Vector2f(one.x, one.y + 300), R, sf::Color::White);
	playerBall = PlayerBall;
	PlayerBall->setOwner("P");
	PlayerBall->setPlayable(true);
	PlayerBall->setFoul(false);
	PlayerBall->DeSelect();
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
	//나머지는 배치표에 맞게 난수로 생성 

	std::vector<int> sol = { 1,2,3,4,5,6 };
	std::vector<int> str = { 8,9,10,11,12,13,14 };

	sf::Vector2f Cord[15] =
	{ one,{one.x - R, one.y - Sqr3 * R},{one.x + R, one.y - Sqr3 * R},
	{one.x - 2 * R,one.y - 2 * Sqr3 * R },{one.x + 2 * R,one.y - 2 * Sqr3 * R},{one.x - 3 * R, one.y - 3 * Sqr3 * R},
	{one.x - R,one.y - 3 * Sqr3 * R},{one.x,one.y - 2 * Sqr3 * R},{one.x + R,one.y - 3 * Sqr3 * R },
	{one.x + 3 * R,one.y - 3 * Sqr3 * R},{one.x - 4 * R,one.y - 4 * Sqr3 * R},{one.x - 2 * R, one.y - 4 * Sqr3 * R},
	{one.x, one.y - 4 * Sqr3 * R},{one.x + 2 * R, one.y - 4 * Sqr3 * R},{one.x + 4 * R, one.y - 4 * Sqr3 * R} };

	//배치표
	int table[15] = { SOLIDS,SOLIDS,STRIPES,
		SOLIDS,STRIPES,SOLIDS,STRIPES,
		UNKNOWN,SOLIDS,STRIPES,STRIPES,
		SOLIDS,STRIPES,STRIPES,SOLIDS };

	//배열 초기화
	for (int i = 0; i < 15; ++i) {
		int k = -1;
		if (i == 0 || i == 7) //고정타입 센티널
			k = i;
		else {
			if (table[i] == STRIPES) { //스트라이프
				int val = rand() % str.size(); //난수 추출
				k = str[val];
				str.erase(remove(str.begin(), str.end(), str[val]), str.end()); //해당 요소 제거
			}
			else { //솔리드
				int val = rand() % sol.size();
				k = sol[val];
				sol.erase(remove(sol.begin(), sol.end(), sol[val]), sol.end());
			}
		}
		ball[i] = new SampleBilliardBall(Cord[i], R, color[i]);
		ball[i]->setOwner(std::to_string(k + 1));
		gameObjects.push_back(ball[i]);
	}
	//플레이어 정보
	int PlayerCnt = 2; //플레이어 수
	Player* p;
	//첫번째 턴 난수
	int FirstTurn = rand() % PlayerCnt;
	for (int i = 0; i < 2; ++i) {
		if (FirstTurn == i) {
			p = new Player(i + 1, true);
		}
		else {
			p = new Player(i + 1, false);
		}
		Players.push_back(p);
	}

	// 플레이어 서로의 주소 추가
	if (&(Players[0]->getNextP()) == nullptr) Players[0]->setNextP(*Players[1]);
	if (&(Players[1]->getNextP()) == nullptr) Players[1]->setNextP(*Players[0]);

	//파워
	power = new Power();
	//위치벡터의 초기 위치 설정
	float rfx = rand() % 2 ? 1.0f : -1.0f;
	float rfy = rand() % 2 ? 1.0f : -1.0f;
	Posvec.x = PlayerBall->getPosition().x + rfx * (R + 5);
	Posvec.y = PlayerBall->getPosition().y + rfy * (R + 5);
}

SampleGame::~SampleGame(void)
{	
	// UI 인스턴스 해제  
	// 게임 오브젝트들 해제

	// static멤버는 소멸자 발생해도 남아있음.
	BilliardPocket::initPocket(); 
	for (SampleBilliardObject* obj : gameObjects)
	{
		delete obj;
	}
	for (Player* p : Players) {
		delete p;
	}
	if(power!=nullptr)
		delete power;
	if (ExitWindow != nullptr)
		delete ExitWindow;
}

sf::Font* SampleGame::font = nullptr;
const sf::Font& SampleGame::getFont(void)
{
	if (font == nullptr) {
		font = new sf::Font;
		font->loadFromFile("THE_Oegyeinseolmyeongseo.ttf");
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
		if (ev.key.code == sf::Keyboard::Escape)
		{
			if (ExitWindow->isOpen()) {
				StopTimer = 0;
				ExitWindow->Close();
			}
			else {
				if(StopTimer==0)
					StopTimer = playerClock.getElapsedTime().asSeconds();
				ExitWindow->Open();
			}
		}
		if (ExitWindow->isOpen())
			break;
		if (ev.key.code == sf::Keyboard::R) {
			BilliardPocket::initPocket();
			for (SampleBilliardObject* obj : gameObjects)
			{
				delete obj;
			}
			for (Player* p : Players) {
				delete p;
			}
			if (power != nullptr)
				delete power;
			playerBall = nullptr; 
			isDraggedMouse = false;
			power = nullptr;
			gameObjects.clear();
			Players.clear();
			makeBilliardObject();
			StopTimer = 0;
			playerClock.restart();
			clock.restart();
			system("cls");
			std::cout << "[LOG]\nRestart" << std::endl;
		}
		if (ev.key.code == sf::Keyboard::Space) {
			if (Player::WhoisTurn().isPhase() != MOVE && Player::WhoisTurn().isWin() != WIN) {
				power->setPressedSpace(true);
			}
		}
		break;
	case sf::Event::KeyReleased:
		if (ExitWindow->isOpen())
			break;
		if (ev.key.code == sf::Keyboard::Space) {
			if (Player::WhoisTurn().isPhase() != MOVE&&Player::WhoisTurn().isWin() != WIN) {
				power->setPressedSpace(false);
			}
		}
		break;
	case sf::Event::MouseMoved:
		// 마우스 움직임 이벤트 
		mouseXY.x = (float)ev.mouseMove.x;
		mouseXY.y = (float)ev.mouseMove.y;
		if (ExitWindow->isOpen())
			ExitWindow->update(mouseXY);
		if (Player::WhoisTurn().isWin() != WIN) {
			if (playerBall->isSelected()) {
				if(SampleBilliardBoard::inBoard(mouseXY)){
					playerBall->setPosition(mouseXY);
					playerBall->setVelocity(0,0);
				}
			}
			if (power->isDraggingPower()) { //파워가 끌렸다면
				power->setHandlePosition(mouseXY); //포지션 설정
				power->FixRange(); //범위 수정
			}
			if (!playerBall->isSelected() && isDraggedMouse == true) {
				Posvec = mouseXY;
			}
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (ev.mouseButton.button == sf::Mouse::Left && ExitWindow->isOpen()) {
			switch (ExitWindow->SelectedButton()) {
			case YES:
				window->close();
				break;
			case NO:
				StopTimer = 0;
				ExitWindow->Close();
				break;
			case NOTHING:
				break;
			}
		}
		if (ExitWindow->isOpen()) //exit창이 열리면 모든 이벤트 처리 취소
			break;
		// 마우스 버튼 누름 이벤트 
		if (Player::WhoisTurn().isWin() != WIN) { //승패가 결정되면 이벤트중단
			if (ev.mouseButton.button == sf::Mouse::Left)
			{
				//공이 움직이고 있는 상태라면 이벤트발생X
				if (playerBall->isIntersecting(mouseXY) && Player::WhoisTurn().isPhase() != MOVE) {
					if (playerBall->isFoul())
						playerBall->Select();
				}
				//파워를 잡았을 때
				if (power->inHandle(mouseXY) && Player::WhoisTurn().isPhase() != MOVE) {
					power->setDragPower(true);
					isDraggedMouse = false;
				}
				else if (!playerBall->isSelected()){
					isDraggedMouse = true;
				}
			}
			//강제로 공을 포켓에 넣는 기능 테스트용 
			/*
			else if (ev.mouseButton.button == sf::Mouse::Right) {
				for (SampleBilliardObject* obj : gameObjects)
				{
					// SampleBilliardBall의 인스턴스가 아닌 경우 pass
					SampleBilliardBall* gameBall = dynamic_cast<SampleBilliardBall*>(obj);
					if (gameBall == nullptr)
					{
						continue;
					}
					// 커서가 공의 내부가 아닌 경우 pass 
					if ((std::powf(mouseXY.x - gameBall->getPosition().x, 2.f)
						+ std::powf(mouseXY.y - gameBall->getPosition().y, 2.f))
					> gameBall->getRadius() * gameBall->getRadius())
					{
						continue;
					}
					//잡은 공 임시 저장 
					catchedBall = gameBall;
					isCatchingBall = true;
				}
			}
			*/
		}
		break;
	case sf::Event::MouseButtonReleased:
		if (ExitWindow->isOpen())
			break;
		// 마우스 버튼 뗌 이벤트 
		if (playerBall->isFoul())
			playerBall->DeSelect();
		if (isDraggedMouse == true)
			isDraggedMouse = false;
		/*
		if (ev.mouseButton.button == sf::Mouse::Right && isCatchingBall) { //잡혀있다면
			isCatchingBall = false;
		}
		*/
		if (ev.mouseButton.button == sf::Mouse::Left && power->isDraggingPower()){ //파워 뗐을 때
			power->setDragPower(false); 
		}
		break;
	}
}

// 상속 클래스는 반드시 게임 상태 갱신 함수 구현해야 함 
void SampleGame::update(void)
{
	//타이머
	sf::Time timeout = sf::seconds(TurnTime); //n초 설정
	sf::Time sec = playerClock.getElapsedTime(); //플레이어의 시간을 불러와서

	if (ExitWindow->isOpen()) {
		playerClock.restart();
	}
	//공이 움직이고 있을 때는 계속 게임 시간 restart
	else if (Players[0]->isPhase() == MOVE || Players[1]->isPhase() == MOVE) {  
		isDraggedMouse = false; //공이 움직일 때는 false로
		if(StopTimer==0)
			StopTimer = sec.asSeconds();
		playerClock.restart();
	}
	else if (Player::WhoisTurn().isWin() == WIN) { //이겼을 때 적용x
		if (StopTimer == 0)
			StopTimer = sec.asSeconds();
	}
	else if (timeout < sec) { //플레이어 시간이 n초를 넘기면
		Player::WhoisTurn().getNextP().setTurn(true); //다음 사람의 턴 true로
		Player::WhoisTurn().getNextP().setTurn(false); //현재 턴 아닌사람 false로
		//파워 초기화
		power->InitDraggedDistance();
		power->InitHandlePosition();
		StopTimer= 0;
		playerClock.restart();
	}
	else //움직이지 않을 때
		StopTimer = 0;
	
	//필요한 공
	SampleBilliardObject* eightBall = nullptr;

	//모든 공의 속도
	int Velocity = 0;
	
	// 게임 오브젝트 업데이트 
	for (SampleBilliardObject* obj : gameObjects)
	{
		obj->update(clock.getElapsedTime().asSeconds());

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
	if (playerBall == nullptr || eightBall == nullptr) {
		std::cout << "Exit[-1]: Error" << std::endl;
		exit(-1);
	}

	// 게임 오브젝트 충돌 검사
	for (SampleBilliardObject* obj1 : gameObjects)
	{
		for (SampleBilliardObject* obj2 : gameObjects)
		{
			obj1->collide(*obj2);
		}
	}


	//플레이어 업데이트, 점수판 업데이트
	Player::WhoisTurn().EightBallupdate(*playerBall, *eightBall, Velocity);


	//플레이어 볼의 속도와 잡힌 공을 포켓위치로 넣어버림.
	//테스트코드
	/*
	if (!isCatchingBall && catchedBall != nullptr) {
		playerBall->setVelocity(2, 2);
		catchedBall->setPosition(595, 447);
		catchedBall = nullptr;
	}
	*/

	/*  파워 관련 기능  */

	//스페이스로 핸들링
	if (power->isPressedSpace()){
		//핸들 속도 조절
		float HVf = 4.f;
		if(power->isChangeDir())
			power->setHandlePosition(power->getHandlePosition() + sf::Vector2f(-HVf, 0));
		else 
			power->setHandlePosition(power->getHandlePosition() + sf::Vector2f(HVf, 0));
		//핸들 진행 방향 변경
		if (power->getHandlePosition().x > HDX2)
			power->setChangeDir(true);
		else if(power->getHandlePosition().x < HDX1)
			power->setChangeDir(false);
		power->FixRange();
	}

	//핸들링 종료 시
	if (!(power->isPressedSpace() || power->isDraggingPower()) && power->getDraggedDistance() != 0) {
		//속도 조절 인수
		float V = 3.2f; //속도 배수 조정
		playerBall->setVelocity(V * power->getDraggedDistance() * Dirvec);
		//끌린 거리 초기화
		power->InitDraggedDistance();
		power->InitHandlePosition();
	}

	// 다음 단위 시간을 위해 초기화 
	clock.restart();
}

// 상속 클래스는 반드시 객체 렌더링 함수 구현해야 함 
void SampleGame::render(sf::RenderTarget& target)
{
	// 화면 클리어 
	window->clear(sf::Color(75, 103, 163));

	// 배경 추가
	target.draw(sBackGround);

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

	// 게임 UI 렌더링
	//방향 표시
	if (Player::WhoisTurn().isPhase() != MOVE) {
		power->render(target);
		renderDirection(target);
	}

	//플레이어 시간
	PlayerTimerRender(target);

	if(ExitWindow->isOpen())
		ExitWindow->render(target);
}

void SampleGame::renderDirection(sf::RenderTarget& target) {
	if (!playerBall->isSelected()) {
		sf::Vector2f distance = (Posvec - playerBall->getPosition());
		float distanceBetween = sqrt(distance.x * distance.x + distance.y * distance.y); //거리를 구함
		float resize = 2000.f; //크기를 최대한 늘림
		Dirvec = distance / distanceBetween; //방향벡터
		sf::Vector2f toResizing(resize *Dirvec);
		Posvec = toResizing + playerBall->getPosition();
		sf::Color directionColor = sf::Color(255, 0, 0);

		Posvec = FixVec(Posvec); //해당 벡터의 충돌 시 충돌범위까지 수정

		sf::Vertex start = sf::Vertex(playerBall->getPosition(), directionColor);
		sf::Vertex end = sf::Vertex(Posvec, directionColor);

		//큐대의 위치
		sf::Vector2f X(Posvec - playerBall->getPosition());
		sf::Vector2f rotationX(X.x * cos(M_PI) - X.y * sin(M_PI), X.x * sin(M_PI) + X.y * cos(M_PI));
		//각도를 구하기 위해 내적값을 미리 구함
		float InnerPd = rotationX.x * CueDirvec.x + rotationX.y * CueDirvec.y;
		resize = sqrt(rotationX.x * rotationX.x + rotationX.y * rotationX.y);
		//파워에 따라 위치가  ~ 200.f까지 변하도록
		float MinPos = playerBall->getRadius();
		rotationX = (rotationX / resize) * (MinPos 
			+ (power->getDraggedDistance() / (HDX2 - HDX1)) * (200-MinPos));
		sf::Vector2f CuePosvec = playerBall->getPosition() + rotationX;
		CueSprite.setPosition(CuePosvec);
		//큐대의 회전각도 => cos각도 +-를 구분할 수 없음. 
		//따라서 rotationX의 y값이 음수; 큐대의 방향벡터보다 위에 있는지를 판단해서 각도에 -부여. 
		float CueAngle = 180 * (acosf(InnerPd / resize) / M_PI); //Radian to Degree
		if (rotationX.y<0)
			CueAngle = CueAngle * -1.0f;
		CueSprite.setRotation(CueAngle);

		sf::VertexArray points;
		points.setPrimitiveType(sf::LineStrip);
		points.append(start);
		points.append(end);
		target.draw(points);
		target.draw(CueSprite);
	}
}

void SampleGame::PlayerTimerRender(sf::RenderTarget& target) {
	sf::RectangleShape TimerBar;
	TimerBar.setFillColor(sf::Color::Green);
	TimerBar.setOutlineColor(sf::Color::Black);
	TimerBar.setOutlineThickness(1);
	TimerBar.setPosition(30.f, 50.f);

	if(StopTimer!=0) //턴 넘겨질 때 정지된 시간
		TimerBar.setSize(sf::Vector2f(300- StopTimer*(300.f/TurnTime), 20));
	else
		TimerBar.setSize(sf::Vector2f(300 - playerClock.getElapsedTime().asSeconds()*(300.f / TurnTime), 20));
	target.draw(TimerBar);
}

sf::Vector2f SampleGame::FixVec(sf::Vector2f vec) {
	sf::Vector2f result; //선분 벡터
	sf::Vector2f Start = playerBall->getPosition();
	sf::Vector2f End = vec;
	sf::Vector2f Direction = End - Start; //방향벡터
	float distance = sqrt(powf(Direction.x, 2.f) + powf(Direction.y, 2.f));
	Direction = Direction / distance; 
	for (float i = 0; i <= distance; ++i) {
		//시작점에서 끝점까지의 선분 집합을 구함
		result = i*Direction + Start;
		//포켓에 없는 공 중에서 
		for (SampleBilliardObject* obj : gameObjects) {
			if (nullptr != dynamic_cast<BilliardPocket*>(obj))
				continue;
			else if (nullptr != dynamic_cast<SampleBilliardGameBall*>(obj))
				continue;
			else if (nullptr == dynamic_cast<SampleBilliardBall*>(obj))
				continue;
			SampleBilliardBall& ball = *dynamic_cast<SampleBilliardBall*>(obj);
			//해당 공에 대해 포켓에 들어있는 공인지 확인
			if (BilliardPocket::InPocket(ball) != -1) 
				continue;
			//해당 공 안에 벡터가 들어오는가를 판단, 해당 점을 return 
			if ((std::powf(result.x - ball.getPosition().x, 2.f) + std::powf(result.y - ball.getPosition().y, 2.f))
				<= ball.getRadius() * ball.getRadius())
			{
				return result;
			}
		}
		//처음으로 경계에 벗어나는 지점을 반환
		if (!(result.x >= INLEFT && result.x <= INRIGHT &&
			result.y >= INTOP && result.y <= INBOTTOM)){
			break;
		}
	}
	return result;
}