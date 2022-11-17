#include "SampleGame.h"
#include "SampleBilliardBoard.h"
#include "SampleBilliardGameBall.h"
#include "BilliardPocket.h"
#include "SampleBilliardObject.h"
#include "ScoreBoard.h"

SampleGame::SampleGame(int width, int height, int fpsLimit)
	:BaseGame(width, height, fpsLimit), isDraggingBall(false), draggedBall(nullptr)
{
	// SampleGame�� ���� �������̽� ���� �� ��� 

	// SampleGame�� ���� �籸�� ���� �� ��� 
	gameObjects.push_back(new SampleBilliardBoard()); //0��

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
	
	//�÷��̾
	SampleBilliardGameBall* PlayerBall =
		new SampleBilliardGameBall(sf::Vector2f(800, 500), 10, sf::Color::White);
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
    //2���� 3���� ���� ����
    //1�� ���� �迭�� ���-���
    //�߰��迭�� ����-��� �����ư��鼭
	sf::Vector2f Cord[15] =
	{ {800, 300},{840, 260},{760, 260},
	{780, 280},{810, 270},{770, 270},
	{800, 260},{800, 280},{830, 270},
	{790, 290},{780, 260},{790, 270},
	{820, 260},{810, 290},{820, 280} };

	//�迭 �ʱ�ȭ
	for (int i = 0; i < 15; ++i) {
		ball[i] = new SampleBilliardBall(Cord[i], 10, color[i]);
		ball[i]->setOwner(std::to_string(i+1)); 
		gameObjects.push_back(ball[i]);
	}

	//�÷��̾� ����
	int PlayerCnt = 2; //�÷��̾� ��
	srand(time(NULL));
	Player* p;
	//ù��° �� ����
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


	// ������ ���� �� ���
	gameObjects.push_back(new ScoreBoard());
	
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
		font->loadFromFile("Arial.ttf");
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
		break;
	case sf::Event::MouseButtonReleased:
		// ���콺 ��ư �� �̺�Ʈ 
		if (ev.mouseButton.button == sf::Mouse::Left && isDraggingBall)
		{
			isDraggingBall = false;
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
	if (playerBall == nullptr|| eightBall == nullptr) exit(-1);  

	//�÷��̾� ������Ʈ, ������ ������Ʈ
	for (Player* p : Players)
	{
		p->update(*playerBall, *eightBall, Velocity);
	}
	
	// ���� ������Ʈ �浹 �˻�
	for (SampleBilliardObject* obj1 : gameObjects)
	{
		for (SampleBilliardObject* obj2 : gameObjects)
		{
			obj1->collide(*obj2);
		}
	}

	// �����ٰ� ���� ���� �ӵ��� �����ϰ� ǥ�� ����
	if (!isDraggingBall && draggedBall != nullptr)
	{
		draggedBall->setVelocity(draggedBall->getPosition().x - mouseXY.x, draggedBall->getPosition().y - mouseXY.y);
		draggedBall = nullptr;
	}



	// ���� ���� �ð��� ���� �ʱ�ȭ 
	clock.restart();
}


void SampleGame::run(void)
{
	if (window == nullptr)
	{
		// ���� �����찡 �����Ǿ� �־�� �� 
		return;
	}

	// ���� ���� ���� 
	while (window->isOpen())
	{
		sf::Time timeout = sf::seconds(10); //10�ʸ� ���� �÷��̾�� �� �ѱ�
		sf::Time sec = playerClock.getElapsedTime();

		if (timeout < sec) {
			
			if (Players[0]->isTurn()) {
				Players[0]->setTurn(false); //�� ����
				Players[0]->getNextP().setTurn(true); //���� �÷��̾�� �ϳѱ�
			}
			else {
				Players[1]->setTurn(false); //�� ����
				Players[1]->getNextP().setTurn(true); //���� �÷��̾�� �ϳѱ�
			}
			playerClock.restart();
		}

		if (Players[0]->isPhase() == MOVE || Players[1]->isPhase() == MOVE) //���� �����̰� ���� ���� ��� ���� �ð� restart
			playerClock.restart();

		sf::Event ev;
		while (window->pollEvent(ev))
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


// ��� Ŭ������ �ݵ�� ��ü ������ �Լ� �����ؾ� �� 
void SampleGame::render(sf::RenderTarget& target)
{
	// ȭ�� Ŭ���� 
	window->clear(sf::Color(75, 103, 163));

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

	// Ÿ�̸� ǥ��
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