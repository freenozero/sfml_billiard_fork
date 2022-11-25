#include "SampleGame.h"
#include "SampleBilliardBoard.h"
#include "SampleBilliardGameBall.h"
#include "BilliardPocket.h"
#include "SampleBilliardObject.h"
#include "ScoreBoard.h"
#include "StartGame.h"

SampleGame::SampleGame(int width, int height, int fpsLimit,int option)
	: BaseGame(width, height, fpsLimit,option), isDraggingBall(false), draggedBall(nullptr)
{
	//���� �α� ���⵵��
	system("cls");
	std::cout << "[Log]" << std::endl;
	std::cout << "Start Game" << std::endl;
	srand(time(NULL));

	//���ε�
	tBackGround.loadFromFile("���.png");
	sBackGround.setTexture(tBackGround);

	// �׽�Ʈ �ڵ� ���� �ʱ�ȭ
	isCatchingBall = false;
	catchedBall = nullptr;

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
	PlayerBall->setOwner("P");
	PlayerBall->setPlayable(true);
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
				sol.erase(remove(sol.begin(), sol.end(), sol[val]), sol.end()); //�ش� ��� ����
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
}

SampleGame::~SampleGame(void)
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
		// Ű���� �̺�Ʈ 
		if (ev.key.code == sf::Keyboard::Escape)
		{
			// TODO: game paused 
			// TODO: call sample GUI 
		}
		break;
	case sf::Event::MouseMoved:
		// ���콺 ������ �̺�Ʈ 
		mouseXY.x = (float)ev.mouseMove.x;
		mouseXY.y = (float)ev.mouseMove.y;
		break;
	case sf::Event::MouseButtonPressed:
		// ���콺 ��ư ���� �̺�Ʈ 
		if (ev.mouseButton.button == sf::Mouse::Left)
		{
			for (SampleBilliardObject* obj : gameObjects)
			{
				//���� �����̰� �ִ� ���¶�� ��ư ���� �̺�Ʈ�߻�X
				if (Players[0]->isPhase() == MOVE || Players[1]->isPhase() == MOVE)
					continue;

				// SampleBilliardBall�� �ν��Ͻ��� �ƴ� ��� pass
				SampleBilliardGameBall* gameBall = dynamic_cast<SampleBilliardGameBall*>(obj);
				if (gameBall == nullptr)
				{
					continue;
				}

				// Ŀ���� ���� ���ΰ� �ƴ� ��� pass 
				if (!gameBall->isIntersecting(mouseXY))
				{
					continue;
				}

				// ���� Playable�� �ƴ� ��� pass 
				if (!gameBall->isPlayable()) {
					continue;
				}

				// �巡�� ������ �� �ӽ� ���� 
				draggedBall = gameBall;
				isDraggingBall = true;
			}
		}
		//������ ���� ���Ͽ� �ִ� ��� �׽�Ʈ�� 
		else if(ev.mouseButton.button == sf::Mouse::Right) {
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
		break;
	case sf::Event::MouseButtonReleased:
		// ���콺 ��ư �� �̺�Ʈ 

		if (ev.mouseButton.button == sf::Mouse::Left && isDraggingBall)
		{

			isDraggingBall = false;
		}
		//�׽�Ʈ �ڵ�
		else if (ev.mouseButton.button == sf::Mouse::Right && isCatchingBall) { //�����ִٸ�
			isCatchingBall = false;
		}
		break;
	}
}

// ��� Ŭ������ �ݵ�� ���� ���� ���� �Լ� �����ؾ� �� 
void SampleGame::update(void)
{
	// �÷��̾� ������ �ּ� �߰�
	if (&(Players[0]->getNextP()) == nullptr) Players[0]->setNextP(*Players[1]);
	if (&(Players[1]->getNextP()) == nullptr) Players[1]->setNextP(*Players[0]);

	//Ÿ�̸�
	sf::Time timeout = sf::seconds(5); //n�� ����
	sf::Time sec = playerClock.getElapsedTime(); //�÷��̾��� �ð��� �ҷ��ͼ�

	//���� �����̰� ���� ���� ��� ���� �ð� restart
	if (Players[0]->isPhase() == MOVE || Players[1]->isPhase() == MOVE) { 
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
		StopTimer= 0;
		playerClock.restart();
	}
	else //�������� ���� ��
		StopTimer = 0;
	
	//�ʿ��� ��
	SampleBilliardGameBall* playerBall = nullptr;
	SampleBilliardObject* eightBall = nullptr;
	
	//��� ���� �ӵ�
	int Velocity = 0;
	
	// ���� ������Ʈ ������Ʈ 
	for (SampleBilliardObject* obj : gameObjects)
	{
		obj->update(clock.getElapsedTime().asSeconds());

		//�÷��̾ ����
		if (playerBall==nullptr)
			playerBall = dynamic_cast<SampleBilliardGameBall*>(obj);
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
	

			
	// �����ٰ� ���� ���� �ӵ��� �����ϰ� ǥ�� ����
	if (!isDraggingBall && draggedBall != nullptr)
	{
		draggedBall->setVelocity(draggedBall->getPosition().x - mouseXY.x, draggedBall->getPosition().y - mouseXY.y);
		draggedBall = nullptr;
	}

	//�÷��̾� ���� �ӵ��� ���� ���� ������ġ�� �־����.
	if (!isCatchingBall && catchedBall != nullptr) {
		playerBall->setVelocity(2, 2);
		catchedBall->setPosition(595, 447);
		catchedBall = nullptr;
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

	// ���� �巡�� �ϸ� ���� ǥ�� (���� �� ��)
	renderDragpower(target);

	// ���� UI ������

	//�÷��̾� �ð�
	PlayerTimerRender(target);
}

void SampleGame::renderDragpower(sf::RenderTarget& target)
{
	if (isDraggingBall)
	{

		sf::Vector2f distance = (mouseXY - draggedBall->getPosition());
		float distanceBetween = sqrt(distance.x * distance.x + distance.y * distance.y);
		sf::Vector2f invert = distance / distanceBetween; //��������?
		sf::Color directionColor = sf::Color(255, (255 - ((int)distanceBetween / 2) % 255), 0);
		if (distanceBetween > 510)
		{
			directionColor = sf::Color::Red;
		}

		sf::Vector2f toSubtract(distanceBetween * invert.x, distanceBetween * invert.y); //�ݴ����
		sf::Vertex start = sf::Vertex(draggedBall->getPosition(), directionColor);
		sf::Vertex end = sf::Vertex(draggedBall->getPosition() - toSubtract, directionColor);

		sf::VertexArray points;
		points.setPrimitiveType(sf::LineStrip);
		points.append(start);
		points.append(end);
		target.draw(points);
	}
}

void SampleGame::PlayerTimerRender(sf::RenderTarget& target) {

	sf::RectangleShape TimerBar;
	TimerBar.setFillColor(sf::Color::Green);
	TimerBar.setOutlineColor(sf::Color::Black);
	TimerBar.setOutlineThickness(1);
	TimerBar.setPosition(30.f, 50.f);

	if(StopTimer!=0) //�� �Ѱ��� �� ������ �ð�
		TimerBar.setSize(sf::Vector2f(300- StopTimer*60, 20));
	else
		TimerBar.setSize(sf::Vector2f(300 - playerClock.getElapsedTime().asSeconds()* 60, 20));
	target.draw(TimerBar);
}