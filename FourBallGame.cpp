#include "FourBallGame.h"
#include "SampleBilliardBoard.h"
#include "SampleBilliardGameBall.h"
#include "SampleBilliardObject.h"
#include "ScoreBoard.h"
#include "StartGame.h"


FourBallGame::FourBallGame(int width, int height, int fpsLimit, int option)
	: BaseGame(width, height, fpsLimit, option), playerBall(nullptr),playerBall1(nullptr), isDraggedMouse(false)
{
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

void FourBallGame::makeBilliardObject()
{
	// 테스트 코드 변수 초기화
	isCatchingBall = false;
	catchedBall = nullptr;

	// ThreeBallGame을 위한 인터페이스 생성 및 등록 

	// ThreeBallGame을 위한 당구대 생성 및 등록 
	gameObjects.push_back(new SampleBilliardBoard(0));

	float R = 13;
	//플레이어볼
	SampleBilliardGameBall* PlayerBall =
		new SampleBilliardGameBall(sf::Vector2f(910, 500), R, sf::Color::White);
	playerBall = PlayerBall;
	PlayerBall->setOwner("P1");
	PlayerBall->setPlayable(true);
	PlayerBall->setFoul(false);
	PlayerBall->DeSelect();
	gameObjects.push_back(PlayerBall);

	SampleBilliardGameBall* PlayerBall1 =
		new SampleBilliardGameBall(sf::Vector2f(870, 250), R, sf::Color::Yellow);
	playerBall1 = PlayerBall1;
	PlayerBall1->setOwner("P2");
	PlayerBall1->setPlayable(true);
	PlayerBall1->setFoul(false);
	PlayerBall1->DeSelect();
	gameObjects.push_back(PlayerBall1);
	

	//적구
	SampleBilliardBall* ball[2];
	typedef sf::Color C;

	//색 매핑
	C color[2] = { C::Red, C::Red };

	//공 좌표 매핑
	sf::Vector2f Cord[2] = { {900,320},{850,320} };

	for (int i = 0; i < 2; ++i) {
		ball[i] = new SampleBilliardBall(Cord[i], R, color[i]);
		ball[i]->setOwner(std::to_string(i + 1));
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
	Posvec1.x = PlayerBall1->getPosition().x + rfx * (R + 5);
	Posvec1.y = PlayerBall1->getPosition().y + rfy * (R + 5);
}

FourBallGame::~FourBallGame(void)
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
	if (power != nullptr)
		delete power;
	if (ExitWindow != nullptr)
		delete ExitWindow;
}

sf::Font* FourBallGame::font = nullptr;
const sf::Font& FourBallGame::getFont(void)
{
	if (font == nullptr) {
		font = new sf::Font;
		font->loadFromFile("THE_Oegyeinseolmyeongseo.ttf");
	}
	return *font;
}

// 상속 클래스는 반드시 이벤트 핸들링 함수 구현해야 함 
void FourBallGame::handle(sf::Event& ev)
{
	// ThreeBallGame을 위한 입력 이벤트 처리 
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
				if (StopTimer == 0)
					StopTimer = playerClock.getElapsedTime().asSeconds();
				ExitWindow->Open();
			}

		}
		if (ExitWindow->isOpen())
			break;
		if (ev.key.code == sf::Keyboard::R) {
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
			if (Player::WhoisTurn().isPhase() != MOVE && Player::WhoisTurn().isWin() != WIN) {
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
			if (playerBall->isSelected() && Players[0]->isTurn() == true) {
				if (SampleBilliardBoard::inBoard(mouseXY)) {
					playerBall->setPosition(mouseXY);
					playerBall->setVelocity(0, 0);
				}
			}
			if (playerBall1->isSelected() && Players[1]->isTurn() == true) {
				if (SampleBilliardBoard::inBoard(mouseXY)) {
					playerBall1->setPosition(mouseXY);
					playerBall1->setVelocity(0, 0);
				}
			}
			if (power->isDraggingPower()) { //파워가 끌렸다면
				power->setHandlePosition(mouseXY); //포지션 설정
				power->FixRange(); //범위 수정
			}
			if (!playerBall->isSelected() && isDraggedMouse == true) {
				Posvec = mouseXY;
			}
			if (!playerBall1->isSelected() && isDraggedMouse == true) {
				Posvec1 = mouseXY;
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
				if(playerBall1->isIntersecting(mouseXY) && Player::WhoisTurn().isPhase() != MOVE) {
					if (playerBall1->isFoul())
						playerBall1->Select();
				}
				//파워를 잡았을 때
				if (power->inHandle(mouseXY) && Player::WhoisTurn().isPhase() != MOVE) {
					power->setDragPower(true);
					isDraggedMouse = false;
				}
				else if ((!playerBall->isSelected()) && (Players[0]->isTurn() == true)) {
					isDraggedMouse = true;
				}
				else if ((!playerBall1->isSelected()) && (Players[1]->isTurn() == true)) {
					isDraggedMouse = true;
				}
			}
			//강제로 공을 포켓에 넣는 기능 테스트용 
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
		}
		break;
	case sf::Event::MouseButtonReleased:
		if (ExitWindow->isOpen())
			break;
		// 마우스 버튼 뗌 이벤트 
		if (playerBall->isFoul())
			playerBall->DeSelect();
		else {
			playerBall->DeSelect();
			
		}
		if (playerBall1->isFoul()) {
			playerBall1->DeSelect();
		}
		else {
			playerBall1->DeSelect();
		}
		if (isDraggedMouse == true)
			isDraggedMouse = false;
		if (ev.mouseButton.button == sf::Mouse::Right && isCatchingBall) { //잡혀있다면
			isCatchingBall = false;
		}
		if (ev.mouseButton.button == sf::Mouse::Left && power->isDraggingPower()) { //파워 뗐을 때
			power->setDragPower(false);
		}
		break;
	}
}

// 상속 클래스는 반드시 게임 상태 갱신 함수 구현해야 함 
void FourBallGame::update(void)
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
		if (StopTimer == 0)
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
		StopTimer = 0;
		playerClock.restart();
	}
	else //움직이지 않을 때
		StopTimer = 0;

	//모든 공의 속도
	int Velocity = 0;

	// 게임 오브젝트 업데이트 
	for (SampleBilliardObject* obj : gameObjects)
	{
		obj->update(clock.getElapsedTime().asSeconds());

		//공 오브젝트의 속도 구하기
		if (dynamic_cast<SampleBilliardBall*>(obj) != nullptr) {
			SampleBilliardBall& Ball = *dynamic_cast<SampleBilliardBall*>(obj);
			if (Ball.getVelocity() != sf::Vector2f(0.f, 0.f))
				Velocity = 1;
		}
	}
	//오브젝트 업데이트 후에 값이 없으면 종료
	if (playerBall == nullptr) {
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
	Player::WhoisTurn().FourBallupdate(*playerBall, *playerBall1,Velocity);

	/*  파워 관련 기능  */

	//스페이스로 핸들링
	if (power->isPressedSpace()) {
		//핸들 속도 조절
		float HVf = 3.f;
		if (power->isChangeDir())
			power->setHandlePosition(power->getHandlePosition() + sf::Vector2f(-HVf, 0));
		else
			power->setHandlePosition(power->getHandlePosition() + sf::Vector2f(HVf, 0));
		//핸들 진행 방향 변경
		if (power->getHandlePosition().x > HDX2)
			power->setChangeDir(true);
		else if (power->getHandlePosition().x < HDX1)
			power->setChangeDir(false);
		power->FixRange();
	}

	//핸들링 종료 시
	if (!(power->isPressedSpace() || power->isDraggingPower()) && power->getDraggedDistance() != 0) {
		//속도 조절 인수
		float V = 4.f; //속도 배수 조정
		if (!playerBall->isSelected() && Players[0]->isTurn() == true) {
			playerBall->setVelocity(V * power->getDraggedDistance() * Dirvec);
		}
		if (!playerBall1->isSelected() && Players[1]->isTurn() == true) {
			playerBall1->setVelocity(V * power->getDraggedDistance() * Dirvec);
		}
		//끌린 거리 초기화
		power->InitDraggedDistance();
		power->InitHandlePosition();
	}

	// 다음 단위 시간을 위해 초기화 
	clock.restart();
}

// 상속 클래스는 반드시 객체 렌더링 함수 구현해야 함 
void FourBallGame::render(sf::RenderTarget& target)
{
	// 화면 클리어 
	window->clear(sf::Color(75, 103, 163));

	// 배경 추가
	target.draw(sBackGround);

	// 플레이어 정보 렌더링
	for (Player* p : Players)
	{
		p->FourBallrender(target);
	}

	// 게임 오브젝트 렌더링 
	for (SampleBilliardObject* obj : gameObjects)
	{
		obj->render(target);
	}

	// 게임 UI 렌더링


	if (Player::WhoisTurn().isPhase() != MOVE && Players[0]->isTurn() == true) {
		power->render(target);
		renderDirection1(target);
	}
	if (Player::WhoisTurn().isPhase() != MOVE && Players[1]->isTurn() == true) {
		power->render(target);
		renderDirection2(target);
	}
	

	

	//플레이어 시간
	PlayerTimerRender(target);

	if (ExitWindow->isOpen())
		ExitWindow->render(target);
}

void FourBallGame::renderDirection1(sf::RenderTarget& target) {
	if (!playerBall->isSelected()) {
		sf::Vector2f distance = (Posvec - playerBall->getPosition());
		float distanceBetween = sqrt(distance.x * distance.x + distance.y * distance.y); //거리를 구함
		float resize = 2000.f; //크기를 최대한 늘림
		Dirvec = distance / distanceBetween; //단위 방향벡터
		sf::Vector2f toResizing(resize * Dirvec);
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
			+ (power->getDraggedDistance() / (HDX2 - HDX1)) * (200 - MinPos));
		sf::Vector2f CuePosvec = playerBall->getPosition() + rotationX;
		CueSprite.setPosition(CuePosvec);
		//큐대의 회전각도 => cos각도 +-를 구분할 수 없음. 
		//따라서 rotationX의 y값이 음수; 큐대의 방향벡터보다 위에 있는지를 판단해서 각도에 -부여. 
		float CueAngle = 180 * (acosf(InnerPd / resize) / M_PI); //Radian to Degree

		if (rotationX.y < 0)
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
void FourBallGame::renderDirection2(sf::RenderTarget& target) {
	if (!playerBall1->isSelected()) {
		sf::Vector2f distance = (Posvec1 - playerBall1->getPosition());
		float distanceBetween = sqrt(distance.x * distance.x + distance.y * distance.y); //거리를 구함
		float resize = 2000.f; //크기를 최대한 늘림
		Dirvec = distance / distanceBetween; //단위 방향벡터
		sf::Vector2f toResizing(resize * Dirvec);
		Posvec1 = toResizing + playerBall1->getPosition();
		sf::Color directionColor = sf::Color(255, 0, 0);

		Posvec1 = FixVec1(Posvec1); //해당 벡터의 충돌 시 충돌범위까지 수정

		sf::Vertex start = sf::Vertex(playerBall1->getPosition(), directionColor);
		sf::Vertex end = sf::Vertex(Posvec1, directionColor);

		//큐대의 위치
		sf::Vector2f X(Posvec1 - playerBall1->getPosition());
		sf::Vector2f rotationX(X.x * cos(M_PI) - X.y * sin(M_PI), X.x * sin(M_PI) + X.y * cos(M_PI));
		//각도를 구하기 위해 내적값을 미리 구함
		float InnerPd = rotationX.x * CueDirvec.x + rotationX.y * CueDirvec.y;
		resize = sqrt(rotationX.x * rotationX.x + rotationX.y * rotationX.y);
		//파워에 따라 위치가  ~ 200.f까지 변하도록
		float MinPos = playerBall1->getRadius();
		rotationX = (rotationX / resize) * (MinPos
			+ (power->getDraggedDistance() / (HDX2 - HDX1)) * (200 - MinPos));
		sf::Vector2f CuePosvec = playerBall1->getPosition() + rotationX;
		CueSprite.setPosition(CuePosvec);
		//큐대의 회전각도 => cos각도 +-를 구분할 수 없음. 
		//따라서 rotationX의 y값이 음수; 큐대의 방향벡터보다 위에 있는지를 판단해서 각도에 -부여. 
		float CueAngle = 180 * (acosf(InnerPd / resize) / M_PI); //Radian to Degree

		if (rotationX.y < 0)
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

void FourBallGame::PlayerTimerRender(sf::RenderTarget& target) {
	sf::RectangleShape TimerBar;
	TimerBar.setFillColor(sf::Color::Green);
	TimerBar.setOutlineColor(sf::Color::Black);
	TimerBar.setOutlineThickness(1);
	TimerBar.setPosition(30.f, 50.f);

	if (StopTimer != 0) //턴 넘겨질 때 정지된 시간
		TimerBar.setSize(sf::Vector2f(300 - StopTimer * (300.f / TurnTime), 20));
	else
		TimerBar.setSize(sf::Vector2f(300 - playerClock.getElapsedTime().asSeconds() * (300.f / TurnTime), 20));
	target.draw(TimerBar);
}

sf::Vector2f FourBallGame::FixVec(sf::Vector2f vec) {
	sf::Vector2f result; //선분 벡터
	sf::Vector2f Start = playerBall->getPosition();
	sf::Vector2f End = vec;
	sf::Vector2f Direction = End - Start; //방향벡터
	float distance = sqrt(powf(Direction.x, 2.f) + powf(Direction.y, 2.f));
	Direction = Direction / distance;
	for (float i = 0; i <= distance; ++i) {
		//시작점에서 끝점까지의 선분 집합을 구함
		result = i * Direction + Start;
		//포켓에 없는 공 중에서 
		for (SampleBilliardObject* obj : gameObjects) {
			if (nullptr != dynamic_cast<SampleBilliardGameBall*>(obj))
				continue;
			else if (nullptr == dynamic_cast<SampleBilliardBall*>(obj))
				continue;
			SampleBilliardBall& ball = *dynamic_cast<SampleBilliardBall*>(obj);
			//해당 공 안에 벡터가 들어오는가를 판단, 해당 점을 return 
			if ((std::powf(result.x - ball.getPosition().x, 2.f) + std::powf(result.y - ball.getPosition().y, 2.f))
				<= ball.getRadius() * ball.getRadius())
			{
				return result;
			}
		}
		//처음으로 경계에 벗어나는 지점을 반환
		if (!(result.x >= INLEFT && result.x <= INRIGHT &&
			result.y >= INTOP && result.y <= INBOTTOM)) {
			break;
		}
	}
	return result;
}

sf::Vector2f FourBallGame::FixVec1(sf::Vector2f vec) {
	sf::Vector2f result; //선분 벡터
	sf::Vector2f Start = playerBall1->getPosition();
	sf::Vector2f End = vec;
	sf::Vector2f Direction = End - Start; //방향벡터
	float distance = sqrt(powf(Direction.x, 2.f) + powf(Direction.y, 2.f));
	Direction = Direction / distance;
	for (float i = 0; i <= distance; ++i) {
		//시작점에서 끝점까지의 선분 집합을 구함
		result = i * Direction + Start;
		//포켓에 없는 공 중에서 
		for (SampleBilliardObject* obj : gameObjects) {
			if (nullptr != dynamic_cast<SampleBilliardGameBall*>(obj))
				continue;
			else if (nullptr == dynamic_cast<SampleBilliardBall*>(obj))
				continue;
			SampleBilliardBall& ball = *dynamic_cast<SampleBilliardBall*>(obj);
			//해당 공 안에 벡터가 들어오는가를 판단, 해당 점을 return 
			if ((std::powf(result.x - ball.getPosition().x, 2.f) + std::powf(result.y - ball.getPosition().y, 2.f))
				<= ball.getRadius() * ball.getRadius())
			{
				return result;
			}
		}
		//처음으로 경계에 벗어나는 지점을 반환
		if (!(result.x >= INLEFT && result.x <= INRIGHT &&
			result.y >= INTOP && result.y <= INBOTTOM)) {
			break;
		}
	}
	return result;
}