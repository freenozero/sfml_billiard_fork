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
	//���� �α� ���⵵��
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

void SampleGame::makeBilliardObject()
{
	// �׽�Ʈ �ڵ� ���� �ʱ�ȭ
	//isCatchingBall = false;
	//catchedBall = nullptr;

	// SampleGame�� ���� �������̽� ���� �� ��� 

	// SampleGame�� ���� �籸�� ���� �� ��� 
	gameObjects.push_back(new SampleBilliardBoard()); //0��

	// ������ ���� �� ��� => ������Ʈ �������� �߰��� ������ �����ؾ� ��.
	gameObjects.push_back(new ScoreBoard());

	//���� 1~6��
	BilliardPocket* Pocket[6];
	//���� x,y ��ǥ �� ���� ���� �����ϵ��� �س����ϴ�.
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

	// SampleGame�� ���� �籸�� ���� �� ��� 
	
	//1���� ���� ��ǥ
	sf::Vector2f one = { 800,300 };
	//�ݰ�
	float R = 12;
	float Sqr3 = sqrt(3); //��Ʈ3

	//�÷��̾
	SampleBilliardGameBall* PlayerBall =
		new SampleBilliardGameBall(sf::Vector2f(one.x, one.y + 300), R, sf::Color::White);
	playerBall = PlayerBall;
	PlayerBall->setOwner("P");
	PlayerBall->setPlayable(true);
	PlayerBall->setFoul(false);
	PlayerBall->DeSelect();
	gameObjects.push_back(PlayerBall);

	//���Ӻ� 8~22������ 
	SampleBilliardBall* ball[15];
	typedef sf::Color C;

	//�� ����
	//1~7�� ���� �ܻ��� = �� 7��
	//9��~15�� ���� �ٹ��� �� = �� 7��
	//���, ����, ����, ����(��ũ), ��Ȳ, �ʷ�, ����  = �� 7��
	C color[15] = {
	C::Yellow, C::Color(0,0,128), C::Red,
	C::Color(77,55,123), C::Color(255,165,0), C::Green,
	C::Color(111, 79, 48),C::Black,C::Yellow,
	C::Color(0,0,128), C::Red,C::Color(77,55,123),
	C::Color(255,165,0), C::Green,  C::Color(111, 79, 48),
	};

	//�� ��ǥ ����
	//1���� �� �տ�, 8���� ���
	//�������� ��ġǥ�� �°� ������ ���� 

	std::vector<int> sol = { 1,2,3,4,5,6 };
	std::vector<int> str = { 8,9,10,11,12,13,14 };

	sf::Vector2f Cord[15] =
	{ one,{one.x - R, one.y - Sqr3 * R},{one.x + R, one.y - Sqr3 * R},
	{one.x - 2 * R,one.y - 2 * Sqr3 * R },{one.x + 2 * R,one.y - 2 * Sqr3 * R},{one.x - 3 * R, one.y - 3 * Sqr3 * R},
	{one.x - R,one.y - 3 * Sqr3 * R},{one.x,one.y - 2 * Sqr3 * R},{one.x + R,one.y - 3 * Sqr3 * R },
	{one.x + 3 * R,one.y - 3 * Sqr3 * R},{one.x - 4 * R,one.y - 4 * Sqr3 * R},{one.x - 2 * R, one.y - 4 * Sqr3 * R},
	{one.x, one.y - 4 * Sqr3 * R},{one.x + 2 * R, one.y - 4 * Sqr3 * R},{one.x + 4 * R, one.y - 4 * Sqr3 * R} };

	//��ġǥ
	int table[15] = { SOLIDS,SOLIDS,STRIPES,
		SOLIDS,STRIPES,SOLIDS,STRIPES,
		UNKNOWN,SOLIDS,STRIPES,STRIPES,
		SOLIDS,STRIPES,STRIPES,SOLIDS };

	//�迭 �ʱ�ȭ
	for (int i = 0; i < 15; ++i) {
		int k = -1;
		if (i == 0 || i == 7) //����Ÿ�� ��Ƽ��
			k = i;
		else {
			if (table[i] == STRIPES) { //��Ʈ������
				int val = rand() % str.size(); //���� ����
				k = str[val];
				str.erase(remove(str.begin(), str.end(), str[val]), str.end()); //�ش� ��� ����
			}
			else { //�ָ���
				int val = rand() % sol.size();
				k = sol[val];
				sol.erase(remove(sol.begin(), sol.end(), sol[val]), sol.end());
			}
		}
		ball[i] = new SampleBilliardBall(Cord[i], R, color[i]);
		ball[i]->setOwner(std::to_string(k + 1));
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
}

SampleGame::~SampleGame(void)
{	
	// UI �ν��Ͻ� ����  
	// ���� ������Ʈ�� ����

	// static����� �Ҹ��� �߻��ص� ��������.
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

// ��� Ŭ������ �ݵ�� �̺�Ʈ �ڵ鸵 �Լ� �����ؾ� �� 
void SampleGame::handle(sf::Event& ev)
{
	// SampleGame�� ���� �Է� �̺�Ʈ ó�� 
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
		// ���콺 ������ �̺�Ʈ 
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
			if (power->isDraggingPower()) { //�Ŀ��� ���ȴٸ�
				power->setHandlePosition(mouseXY); //������ ����
				power->FixRange(); //���� ����
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
				//�Ŀ��� ����� ��
				if (power->inHandle(mouseXY) && Player::WhoisTurn().isPhase() != MOVE) {
					power->setDragPower(true);
					isDraggedMouse = false;
				}
				else if (!playerBall->isSelected()){
					isDraggedMouse = true;
				}
			}
			//������ ���� ���Ͽ� �ִ� ��� �׽�Ʈ�� 
			/*
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
			*/
		}
		break;
	case sf::Event::MouseButtonReleased:
		if (ExitWindow->isOpen())
			break;
		// ���콺 ��ư �� �̺�Ʈ 
		if (playerBall->isFoul())
			playerBall->DeSelect();
		if (isDraggedMouse == true)
			isDraggedMouse = false;
		/*
		if (ev.mouseButton.button == sf::Mouse::Right && isCatchingBall) { //�����ִٸ�
			isCatchingBall = false;
		}
		*/
		if (ev.mouseButton.button == sf::Mouse::Left && power->isDraggingPower()){ //�Ŀ� ���� ��
			power->setDragPower(false); 
		}
		break;
	}
}

// ��� Ŭ������ �ݵ�� ���� ���� ���� �Լ� �����ؾ� �� 
void SampleGame::update(void)
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
		if(StopTimer==0)
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
		StopTimer= 0;
		playerClock.restart();
	}
	else //�������� ���� ��
		StopTimer = 0;
	
	//�ʿ��� ��
	SampleBilliardObject* eightBall = nullptr;

	//��� ���� �ӵ�
	int Velocity = 0;
	
	// ���� ������Ʈ ������Ʈ 
	for (SampleBilliardObject* obj : gameObjects)
	{
		obj->update(clock.getElapsedTime().asSeconds());

		//8���� ����
		if (eightBall == nullptr) {
			SampleBilliardBall* eight = dynamic_cast<SampleBilliardBall*>(obj);
			if (eight!=nullptr&&eight->getOwner().compare("8") == 0)
				eightBall = obj;
		}
		//�� ������Ʈ�� �ӵ� ���ϱ�
		if (dynamic_cast<SampleBilliardBall*>(obj) != nullptr
			&& dynamic_cast<BilliardPocket*>(obj) == nullptr) {
			SampleBilliardBall& Ball = *dynamic_cast<SampleBilliardBall*>(obj);
			if (Ball.getVelocity() != sf::Vector2f(0.f, 0.f))
				Velocity = 1;
		}
	}
	//������Ʈ ������Ʈ �Ŀ� ���� ������ ����
	if (playerBall == nullptr || eightBall == nullptr) {
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
	Player::WhoisTurn().EightBallupdate(*playerBall, *eightBall, Velocity);


	//�÷��̾� ���� �ӵ��� ���� ���� ������ġ�� �־����.
	//�׽�Ʈ�ڵ�
	/*
	if (!isCatchingBall && catchedBall != nullptr) {
		playerBall->setVelocity(2, 2);
		catchedBall->setPosition(595, 447);
		catchedBall = nullptr;
	}
	*/

	/*  �Ŀ� ���� ���  */

	//�����̽��� �ڵ鸵
	if (power->isPressedSpace()){
		//�ڵ� �ӵ� ����
		float HVf = 4.f;
		if(power->isChangeDir())
			power->setHandlePosition(power->getHandlePosition() + sf::Vector2f(-HVf, 0));
		else 
			power->setHandlePosition(power->getHandlePosition() + sf::Vector2f(HVf, 0));
		//�ڵ� ���� ���� ����
		if (power->getHandlePosition().x > HDX2)
			power->setChangeDir(true);
		else if(power->getHandlePosition().x < HDX1)
			power->setChangeDir(false);
		power->FixRange();
	}

	//�ڵ鸵 ���� ��
	if (!(power->isPressedSpace() || power->isDraggingPower()) && power->getDraggedDistance() != 0) {
		//�ӵ� ���� �μ�
		float V = 3.2f; //�ӵ� ��� ����
		playerBall->setVelocity(V * power->getDraggedDistance() * Dirvec);
		//���� �Ÿ� �ʱ�ȭ
		power->InitDraggedDistance();
		power->InitHandlePosition();
	}

	// ���� ���� �ð��� ���� �ʱ�ȭ 
	clock.restart();
}

// ��� Ŭ������ �ݵ�� ��ü ������ �Լ� �����ؾ� �� 
void SampleGame::render(sf::RenderTarget& target)
{
	// ȭ�� Ŭ���� 
	window->clear(sf::Color(75, 103, 163));

	// ��� �߰�
	target.draw(sBackGround);

	// �÷��̾� ���� ������
	for (Player* p : Players)
	{
		p->render(target);
	}

	// ���� ������Ʈ ������ 
	for (SampleBilliardObject* obj : gameObjects)
	{
		obj->render(target);
	}

	// ���� UI ������
	//���� ǥ��
	if (Player::WhoisTurn().isPhase() != MOVE) {
		power->render(target);
		renderDirection(target);
	}

	//�÷��̾� �ð�
	PlayerTimerRender(target);

	if(ExitWindow->isOpen())
		ExitWindow->render(target);
}

void SampleGame::renderDirection(sf::RenderTarget& target) {
	if (!playerBall->isSelected()) {
		sf::Vector2f distance = (Posvec - playerBall->getPosition());
		float distanceBetween = sqrt(distance.x * distance.x + distance.y * distance.y); //�Ÿ��� ����
		float resize = 2000.f; //ũ�⸦ �ִ��� �ø�
		Dirvec = distance / distanceBetween; //���⺤��
		sf::Vector2f toResizing(resize *Dirvec);
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
			+ (power->getDraggedDistance() / (HDX2 - HDX1)) * (200-MinPos));
		sf::Vector2f CuePosvec = playerBall->getPosition() + rotationX;
		CueSprite.setPosition(CuePosvec);
		//ť���� ȸ������ => cos���� +-�� ������ �� ����. 
		//���� rotationX�� y���� ����; ť���� ���⺤�ͺ��� ���� �ִ����� �Ǵ��ؼ� ������ -�ο�. 
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

	if(StopTimer!=0) //�� �Ѱ��� �� ������ �ð�
		TimerBar.setSize(sf::Vector2f(300- StopTimer*(300.f/TurnTime), 20));
	else
		TimerBar.setSize(sf::Vector2f(300 - playerClock.getElapsedTime().asSeconds()*(300.f / TurnTime), 20));
	target.draw(TimerBar);
}

sf::Vector2f SampleGame::FixVec(sf::Vector2f vec) {
	sf::Vector2f result; //���� ����
	sf::Vector2f Start = playerBall->getPosition();
	sf::Vector2f End = vec;
	sf::Vector2f Direction = End - Start; //���⺤��
	float distance = sqrt(powf(Direction.x, 2.f) + powf(Direction.y, 2.f));
	Direction = Direction / distance; 
	for (float i = 0; i <= distance; ++i) {
		//���������� ���������� ���� ������ ����
		result = i*Direction + Start;
		//���Ͽ� ���� �� �߿��� 
		for (SampleBilliardObject* obj : gameObjects) {
			if (nullptr != dynamic_cast<BilliardPocket*>(obj))
				continue;
			else if (nullptr != dynamic_cast<SampleBilliardGameBall*>(obj))
				continue;
			else if (nullptr == dynamic_cast<SampleBilliardBall*>(obj))
				continue;
			SampleBilliardBall& ball = *dynamic_cast<SampleBilliardBall*>(obj);
			//�ش� ���� ���� ���Ͽ� ����ִ� ������ Ȯ��
			if (BilliardPocket::InPocket(ball) != -1) 
				continue;
			//�ش� �� �ȿ� ���Ͱ� �����°��� �Ǵ�, �ش� ���� return 
			if ((std::powf(result.x - ball.getPosition().x, 2.f) + std::powf(result.y - ball.getPosition().y, 2.f))
				<= ball.getRadius() * ball.getRadius())
			{
				return result;
			}
		}
		//ó������ ��迡 ����� ������ ��ȯ
		if (!(result.x >= INLEFT && result.x <= INRIGHT &&
			result.y >= INTOP && result.y <= INBOTTOM)){
			break;
		}
	}
	return result;
}