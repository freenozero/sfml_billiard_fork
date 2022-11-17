#include "StartGame.h"
#include "SampleGame.h"
StartGame::StartGame(int width, int height, int fpsLimit) :
	BaseGame(width, height, fpsLimit)
{
	//���� ȭ�� �̹��� �ε� 
	texture.loadFromFile("logo.png", sf::IntRect(0, 0, 500, 500));

	sprite.setTexture(texture);
	sprite.setPosition(0, 0);
}


StartGame::~StartGame(void)
{
	delete window;
}

sf::Font* StartGame::font = nullptr;
const sf::Font& StartGame::getFont(void)
{
	if (font == nullptr) {
		font = new sf::Font;
		font->loadFromFile("Arial.ttf");
	}
	return *font;
}

void StartGame::handle(sf::Event& ev)
{
	switch (ev.type) {
	case sf::Event::Closed:
		window->close();
		break;
	case sf::Event::MouseButtonPressed:
		if (ev.mouseButton.button == sf::Mouse::Left) {
			// ���� ����
			SampleGame&& game = SampleGame(1024, 900, 60);

			// ���� ����
			game.run();
		}
	}
}

void StartGame::update(void)
{
}

void StartGame::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}
