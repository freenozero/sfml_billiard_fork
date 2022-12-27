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

	//���ε�
	tBackGround.loadFromFile("���.png");
	sBackGround.setTexture(tBackGround);

	//ť�� �ε�
	CueTexture.loadFromFile("cue.png");
	CueSprite.setTexture(CueTexture);
	CueSprite.setScale(0.2f, 0.2f);
	CueSprite.setOrigin(0, 65.f);
	//ť���� ���⺤��
	CueDirvec = { 1,0 };

	//�� �ð�
	TurnTime = 10;

	makeBilliardObject();

	ExitWindow = new GameExit(window->getSize().x, window->getSize().y);
}

void FourBallGame::makeBilliardObject()
{
	// �׽�Ʈ �ڵ� ���� �ʱ�ȭ
	isCatchingBall = false;
	catchedBall = nullptr;

	// ThreeBallGame�� ���� �������̽� ���� �� ��� 

	// ThreeBallGame�� ���� �籸�� ���� �� ��� 
	gameObjects.push_back(new SampleBilliardBoard(0));

	float R = 13;
	//�÷��̾
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
	

	//����
	SampleBilliardBall* ball[2];
	typedef sf::Color C;

	//�� ����
	C color[2] = { C::Red, C::Red };

	//�� ��ǥ ����
	sf::Vector2f Cord[2] = { {900,320},{850,320} };

	for (int i = 0; i < 2; ++i) {
		ball[i] = new SampleBilliardBall(Cord[i], R, color[i]);
		ball[i]->setOwner(std::to_string(i + 1));
		gameObjects.push_back(ball[i]);
	}

	//�÷��̾� ����
	int PlayerCnt = 2; //�÷��̾� ��
	Player* p;
	//ù��° �� ����
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

	// �÷��̾� ������ �ּ� �߰�
	if (&(Players[0]->getNextP()) == nullptr) Players[0]->setNextP(*Players[1]);
	if (&(Players[1]->getNextP()) == nullptr) Players[1]->setNextP(*Players[0]);

	//�Ŀ�
	power = new Power();
	//��ġ������ �ʱ� ��ġ ����
	float rfx = rand() % 2 ? 1.0f : -1.0f;
	float rfy = rand() % 2 ? 1.0f : -1.0f;
	Posvec.x = PlayerBall->getPosition().x + rfx * (R + 5);
	Posvec.y = PlayerBall->getPosition().y + rfy * (R + 5);
	Posvec1.x = PlayerBall1->getPosition().x + rfx * (R + 5);
	Posvec1.y = PlayerBall1->getPosition().y + rfy * (R + 5);
}

FourBallGame::~FourBallGame(void)
{
	// UI �ν��Ͻ� ����  
	// ���� ������Ʈ�� ����
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

// ��� Ŭ������ �ݵ�� �̺�Ʈ �ڵ鸵 �Լ� �����ؾ� �� 
void FourBallGame::handle(sf::Event& ev)
{
	// ThreeBallGame�� ���� �Է� �̺�Ʈ ó�� 
	switch (ev.type)
	{
	case sf::Event::Closed:
		// �������� x ��ư ������ �����Ѵ� 
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
		// ���콺 ������ �̺�Ʈ 
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
			if (power->isDraggingPower()) { //�Ŀ��� ���ȴٸ�
				power->setHandlePosition(mouseXY); //������ ����
				power->FixRange(); //���� ����
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
		if (ExitWindow->isOpen()) //exitâ�� ������ ��� �̺�Ʈ ó�� ���
			break;
		// ���콺 ��ư ���� �̺�Ʈ 
		if (Player::WhoisTurn().isWin() != WIN) { //���а� �����Ǹ� �̺�Ʈ�ߴ�
			if (ev.mouseButton.button == sf::Mouse::Left)
			{
				//���� �����̰� �ִ� ���¶�� �̺�Ʈ�߻�X
				if (playerBall->isIntersecting(mouseXY) && Player::WhoisTurn().isPhase() != MOVE) {
					if (playerBall->isFoul())
						playerBall->Select();
				}
				if(playerBall1->isIntersecting(mouseXY) && Player::WhoisTurn().isPhase() != MOVE) {
					if (playerBall1->isFoul())
						playerBall1->Select();
				}
				//�Ŀ��� ����� ��
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
			//������ ���� ���Ͽ� �ִ� ��� �׽�Ʈ�� 
			else if (ev.mouseButton.button == sf::Mouse::Right) {
				for (SampleBilliardObject* obj : gameObjects)
				{
					// SampleBilliardBall�� �ν��Ͻ��� �ƴ� ��� pass
					SampleBilliardBall* gameBall = dynamic_cast<SampleBilliardBall*>(obj);
					if (gameBall == nullptr)
					{
						continue;
					}
					// Ŀ���� ���� ���ΰ� �ƴ� ��� pass 
					if ((std::powf(mouseXY.x - gameBall->getPosition().x, 2.f)
						+ std::powf(mouseXY.y - gameBall->getPosition().y, 2.f))
	> gameBall->getRadius() * gameBall->getRadius())
					{
						continue;
					}
					//���� �� �ӽ� ���� 
					catchedBall = gameBall;
					isCatchingBall = true;
				}
			}
		}
		break;
	case sf::Event::MouseButtonReleased:
		if (ExitWindow->isOpen())
			break;
		// ���콺 ��ư �� �̺�Ʈ 
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
		if (ev.mouseButton.button == sf::Mouse::Right && isCatchingBall) { //�����ִٸ�
			isCatchingBall = false;
		}
		if (ev.mouseButton.button == sf::Mouse::Left && power->isDraggingPower()) { //�Ŀ� ���� ��
			power->setDragPower(false);
		}
		break;
	}
}

// ��� Ŭ������ �ݵ�� ���� ���� ���� �Լ� �����ؾ� �� 
void FourBallGame::update(void)
{
	//Ÿ�̸�
	sf::Time timeout = sf::seconds(TurnTime); //n�� ����
	sf::Time sec = playerClock.getElapsedTime(); //�÷��̾��� �ð��� �ҷ��ͼ�

	if (ExitWindow->isOpen()) {
		playerClock.restart();
	}
	//���� �����̰� ���� ���� ��� ���� �ð� restart
	else if (Players[0]->isPhase() == MOVE || Players[1]->isPhase() == MOVE) {
		isDraggedMouse = false; //���� ������ ���� false��
		if (StopTimer == 0)
			StopTimer = sec.asSeconds();
		playerClock.restart();
	}

	else if (Player::WhoisTurn().isWin() == WIN) { //�̰��� �� ����x
		if (StopTimer == 0)
			StopTimer = sec.asSeconds();
	}
	else if (timeout < sec) { //�÷��̾� �ð��� n�ʸ� �ѱ��
		Player::WhoisTurn().getNextP().setTurn(true); //���� ����� �� true��
		Player::WhoisTurn().getNextP().setTurn(false); //���� �� �ƴѻ�� false��
		//�Ŀ� �ʱ�ȭ
		power->InitDraggedDistance();
		power->InitHandlePosition();
		StopTimer = 0;
		playerClock.restart();
	}
	else //�������� ���� ��
		StopTimer = 0;

	//��� ���� �ӵ�
	int Velocity = 0;

	// ���� ������Ʈ ������Ʈ 
	for (SampleBilliardObject* obj : gameObjects)
	{
		obj->update(clock.getElapsedTime().asSeconds());

		//�� ������Ʈ�� �ӵ� ���ϱ�
		if (dynamic_cast<SampleBilliardBall*>(obj) != nullptr) {
			SampleBilliardBall& Ball = *dynamic_cast<SampleBilliardBall*>(obj);
			if (Ball.getVelocity() != sf::Vector2f(0.f, 0.f))
				Velocity = 1;
		}
	}
	//������Ʈ ������Ʈ �Ŀ� ���� ������ ����
	if (playerBall == nullptr) {
		std::cout << "Exit[-1]: Error" << std::endl;
		exit(-1);
	}

	// ���� ������Ʈ �浹 �˻�
	for (SampleBilliardObject* obj1 : gameObjects)
	{
		for (SampleBilliardObject* obj2 : gameObjects)
		{
			obj1->collide(*obj2);
		}
	}

	//�÷��̾� ������Ʈ, ������ ������Ʈ
	Player::WhoisTurn().FourBallupdate(*playerBall, *playerBall1,Velocity);

	/*  �Ŀ� ���� ���  */

	//�����̽��� �ڵ鸵
	if (power->isPressedSpace()) {
		//�ڵ� �ӵ� ����
		float HVf = 3.f;
		if (power->isChangeDir())
			power->setHandlePosition(power->getHandlePosition() + sf::Vector2f(-HVf, 0));
		else
			power->setHandlePosition(power->getHandlePosition() + sf::Vector2f(HVf, 0));
		//�ڵ� ���� ���� ����
		if (power->getHandlePosition().x > HDX2)
			power->setChangeDir(true);
		else if (power->getHandlePosition().x < HDX1)
			power->setChangeDir(false);
		power->FixRange();
	}

	//�ڵ鸵 ���� ��
	if (!(power->isPressedSpace() || power->isDraggingPower()) && power->getDraggedDistance() != 0) {
		//�ӵ� ���� �μ�
		float V = 4.f; //�ӵ� ��� ����
		if (!playerBall->isSelected() && Players[0]->isTurn() == true) {
			playerBall->setVelocity(V * power->getDraggedDistance() * Dirvec);
		}
		if (!playerBall1->isSelected() && Players[1]->isTurn() == true) {
			playerBall1->setVelocity(V * power->getDraggedDistance() * Dirvec);
		}
		//���� �Ÿ� �ʱ�ȭ
		power->InitDraggedDistance();
		power->InitHandlePosition();
	}

	// ���� ���� �ð��� ���� �ʱ�ȭ 
	clock.restart();
}

// ��� Ŭ������ �ݵ�� ��ü ������ �Լ� �����ؾ� �� 
void FourBallGame::render(sf::RenderTarget& target)
{
	// ȭ�� Ŭ���� 
	window->clear(sf::Color(75, 103, 163));

	// ��� �߰�
	target.draw(sBackGround);

	// �÷��̾� ���� ������
	for (Player* p : Players)
	{
		p->FourBallrender(target);
	}

	// ���� ������Ʈ ������ 
	for (SampleBilliardObject* obj : gameObjects)
	{
		obj->render(target);
	}

	// ���� UI ������


	if (Player::WhoisTurn().isPhase() != MOVE && Players[0]->isTurn() == true) {
		power->render(target);
		renderDirection1(target);
	}
	if (Player::WhoisTurn().isPhase() != MOVE && Players[1]->isTurn() == true) {
		power->render(target);
		renderDirection2(target);
	}
	

	

	//�÷��̾� �ð�
	PlayerTimerRender(target);

	if (ExitWindow->isOpen())
		ExitWindow->render(target);
}

void FourBallGame::renderDirection1(sf::RenderTarget& target) {
	if (!playerBall->isSelected()) {
		sf::Vector2f distance = (Posvec - playerBall->getPosition());
		float distanceBetween = sqrt(distance.x * distance.x + distance.y * distance.y); //�Ÿ��� ����
		float resize = 2000.f; //ũ�⸦ �ִ��� �ø�
		Dirvec = distance / distanceBetween; //���� ���⺤��
		sf::Vector2f toResizing(resize * Dirvec);
		Posvec = toResizing + playerBall->getPosition();
		sf::Color directionColor = sf::Color(255, 0, 0);

		Posvec = FixVec(Posvec); //�ش� ������ �浹 �� �浹�������� ����

		sf::Vertex start = sf::Vertex(playerBall->getPosition(), directionColor);
		sf::Vertex end = sf::Vertex(Posvec, directionColor);

		//ť���� ��ġ
		sf::Vector2f X(Posvec - playerBall->getPosition());
		sf::Vector2f rotationX(X.x * cos(M_PI) - X.y * sin(M_PI), X.x * sin(M_PI) + X.y * cos(M_PI));
		//������ ���ϱ� ���� �������� �̸� ����
		float InnerPd = rotationX.x * CueDirvec.x + rotationX.y * CueDirvec.y;
		resize = sqrt(rotationX.x * rotationX.x + rotationX.y * rotationX.y);
		//�Ŀ��� ���� ��ġ��  ~ 200.f���� ���ϵ���
		float MinPos = playerBall->getRadius();
		rotationX = (rotationX / resize) * (MinPos
			+ (power->getDraggedDistance() / (HDX2 - HDX1)) * (200 - MinPos));
		sf::Vector2f CuePosvec = playerBall->getPosition() + rotationX;
		CueSprite.setPosition(CuePosvec);
		//ť���� ȸ������ => cos���� +-�� ������ �� ����. 
		//���� rotationX�� y���� ����; ť���� ���⺤�ͺ��� ���� �ִ����� �Ǵ��ؼ� ������ -�ο�. 
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
		float distanceBetween = sqrt(distance.x * distance.x + distance.y * distance.y); //�Ÿ��� ����
		float resize = 2000.f; //ũ�⸦ �ִ��� �ø�
		Dirvec = distance / distanceBetween; //���� ���⺤��
		sf::Vector2f toResizing(resize * Dirvec);
		Posvec1 = toResizing + playerBall1->getPosition();
		sf::Color directionColor = sf::Color(255, 0, 0);

		Posvec1 = FixVec1(Posvec1); //�ش� ������ �浹 �� �浹�������� ����

		sf::Vertex start = sf::Vertex(playerBall1->getPosition(), directionColor);
		sf::Vertex end = sf::Vertex(Posvec1, directionColor);

		//ť���� ��ġ
		sf::Vector2f X(Posvec1 - playerBall1->getPosition());
		sf::Vector2f rotationX(X.x * cos(M_PI) - X.y * sin(M_PI), X.x * sin(M_PI) + X.y * cos(M_PI));
		//������ ���ϱ� ���� �������� �̸� ����
		float InnerPd = rotationX.x * CueDirvec.x + rotationX.y * CueDirvec.y;
		resize = sqrt(rotationX.x * rotationX.x + rotationX.y * rotationX.y);
		//�Ŀ��� ���� ��ġ��  ~ 200.f���� ���ϵ���
		float MinPos = playerBall1->getRadius();
		rotationX = (rotationX / resize) * (MinPos
			+ (power->getDraggedDistance() / (HDX2 - HDX1)) * (200 - MinPos));
		sf::Vector2f CuePosvec = playerBall1->getPosition() + rotationX;
		CueSprite.setPosition(CuePosvec);
		//ť���� ȸ������ => cos���� +-�� ������ �� ����. 
		//���� rotationX�� y���� ����; ť���� ���⺤�ͺ��� ���� �ִ����� �Ǵ��ؼ� ������ -�ο�. 
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

	if (StopTimer != 0) //�� �Ѱ��� �� ������ �ð�
		TimerBar.setSize(sf::Vector2f(300 - StopTimer * (300.f / TurnTime), 20));
	else
		TimerBar.setSize(sf::Vector2f(300 - playerClock.getElapsedTime().asSeconds() * (300.f / TurnTime), 20));
	target.draw(TimerBar);
}

sf::Vector2f FourBallGame::FixVec(sf::Vector2f vec) {
	sf::Vector2f result; //���� ����
	sf::Vector2f Start = playerBall->getPosition();
	sf::Vector2f End = vec;
	sf::Vector2f Direction = End - Start; //���⺤��
	float distance = sqrt(powf(Direction.x, 2.f) + powf(Direction.y, 2.f));
	Direction = Direction / distance;
	for (float i = 0; i <= distance; ++i) {
		//���������� ���������� ���� ������ ����
		result = i * Direction + Start;
		//���Ͽ� ���� �� �߿��� 
		for (SampleBilliardObject* obj : gameObjects) {
			if (nullptr != dynamic_cast<SampleBilliardGameBall*>(obj))
				continue;
			else if (nullptr == dynamic_cast<SampleBilliardBall*>(obj))
				continue;
			SampleBilliardBall& ball = *dynamic_cast<SampleBilliardBall*>(obj);
			//�ش� �� �ȿ� ���Ͱ� �����°��� �Ǵ�, �ش� ���� return 
			if ((std::powf(result.x - ball.getPosition().x, 2.f) + std::powf(result.y - ball.getPosition().y, 2.f))
				<= ball.getRadius() * ball.getRadius())
			{
				return result;
			}
		}
		//ó������ ��迡 ����� ������ ��ȯ
		if (!(result.x >= INLEFT && result.x <= INRIGHT &&
			result.y >= INTOP && result.y <= INBOTTOM)) {
			break;
		}
	}
	return result;
}

sf::Vector2f FourBallGame::FixVec1(sf::Vector2f vec) {
	sf::Vector2f result; //���� ����
	sf::Vector2f Start = playerBall1->getPosition();
	sf::Vector2f End = vec;
	sf::Vector2f Direction = End - Start; //���⺤��
	float distance = sqrt(powf(Direction.x, 2.f) + powf(Direction.y, 2.f));
	Direction = Direction / distance;
	for (float i = 0; i <= distance; ++i) {
		//���������� ���������� ���� ������ ����
		result = i * Direction + Start;
		//���Ͽ� ���� �� �߿��� 
		for (SampleBilliardObject* obj : gameObjects) {
			if (nullptr != dynamic_cast<SampleBilliardGameBall*>(obj))
				continue;
			else if (nullptr == dynamic_cast<SampleBilliardBall*>(obj))
				continue;
			SampleBilliardBall& ball = *dynamic_cast<SampleBilliardBall*>(obj);
			//�ش� �� �ȿ� ���Ͱ� �����°��� �Ǵ�, �ش� ���� return 
			if ((std::powf(result.x - ball.getPosition().x, 2.f) + std::powf(result.y - ball.getPosition().y, 2.f))
				<= ball.getRadius() * ball.getRadius())
			{
				return result;
			}
		}
		//ó������ ��迡 ����� ������ ��ȯ
		if (!(result.x >= INLEFT && result.x <= INRIGHT &&
			result.y >= INTOP && result.y <= INBOTTOM)) {
			break;
		}
	}
	return result;
}