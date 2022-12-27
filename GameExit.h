#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "GameButton.h"

enum {
	NO=-1,NOTHING,YES
};

class GameExit {
private:
	sf::Texture Texture;
	sf::Sprite Sprite;

	GameButton* ExitYes;
	GameButton* ExitYes_Sel;
	GameButton* ExitNo;
	GameButton* ExitNo_Sel;

	bool ExitOpen;

	bool SelectYes;
	bool SelectNo;
public:
	//Exit»≠∏È
	GameExit(float WinSizeWidth, float WinSizeHeight);
	~GameExit();
	void render(sf::RenderTarget& target);
	void Open();
	void Close();
	bool isOpen();
	void update(sf::Vector2f mouse);
	int SelectedButton();
};