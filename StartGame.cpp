#include "StartGame.h"


StartGame::StartGame(int width, int height, int fpsLimit) 
	: Width(width), Height(height), FpsLimit(fpsLimit), option(0), isChangeBK(false), interval(0), time(0)
{	
	std::cout << "GameWindow Loading..." << std::endl;

	//���Ǽ���
	if (!Startbuffer.loadFromFile("�κ�����.wav"))
		std::cout << "������ ����� �� �����ϴ�." << std::endl;
	Startsound.setBuffer(Startbuffer);
	Startsound.play();
	Startsound.setLoop(true);

	//������ �ʱⰪ ����
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

	//��ư �߰� >> ���ڷ� �̹����̸�,x��ǥ,y��ǥ,�ʺ�,���̸� ����
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
	Startsound.stop(); //��������
	for (GameButton* g : GBBW) {
		delete g;
	}
	for (GameButton* g : GB) {
		delete g;
	}
	delete window;
	//â �ݰ� ���� �ҷ���
	if(option>0){
		BaseGame&& game = SampleGame(Width, Height, FpsLimit,option);
		game.run();
		option = 0; //�⺻
	}
	std::cout << "Exit" << std::endl;
}

void StartGame::handle(sf::Event& ev)
{
	switch (ev.type) {
	case sf::Event::Closed:
		Close = true; //x��ư���� ���� ���� ��������
		window->close();
		break;
	case sf::Event::MouseMoved:
		// ���콺 ������ �̺�Ʈ 
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
			if(!isChangeBK){ //������
				//���⿡�� ���� ����
				sprite.setTexture(BackGround);
				sprite.setPosition(0, 0);
				isChangeBK=true; //ȭ�� ����
			}
			else { //����Ǿ��� ��
				//��ư1 �ȿ��� ��Ŭ�� �߻� ��
				if (GB[0]->isIntersecting(mouseXY)){
					option = EIGHTBALL; //EIGHTBALL���� ����
					window->close();
				}
				else {
					//��ư 2, 3 �� ���� �ɼ�	
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
				isChangeBK = false; //ȭ�� ����
			}
		}
	}
}

void StartGame::Info(void)
{
	if (window == nullptr)
	{
		//������ ��������
		return;
	}

	// ���� ���� ���� 
	while (window->isOpen())
	{
		sf::Event ev;
		while (window->pollEvent(ev))
		{
			// �Է� �̺�Ʈ ó�� (�ڽ� Ŭ���� ����) 
			handle(ev);
		}

		// ������ (�ڽ� Ŭ���� ����) 
		render(*window);

		// ������ ��� ǥ��
		window->display();
	}
}

void StartGame::render(sf::RenderTarget& target)
{	
	float xPosition = 130;
	float yPosition = 450;
	//��� ���� ��
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
		clock.restart(); //�����
	}
	else { //���� ���� ȭ��
		target.draw(sprite);
		for (GameButton* g : GB)
			g->render(target);
		for (GameButton* g : GBBW)
			g->render(target);
	}
}