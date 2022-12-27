#include "GameExit.h"

GameExit::GameExit(float WinSizeWidth, float WinSizeHeight) 
	: ExitOpen(false),SelectYes(false), SelectNo(false) {
	Texture.loadFromFile("Exit.png");
	Sprite.setTexture(Texture);
	//윈도우 정중앙으로 위치
	Sprite.setPosition((WinSizeWidth - 805) / 2.f, (WinSizeHeight - 507) / 2.f);
	float StartPosX = Sprite.getPosition().x + 150;
	float StartPosY = Sprite.getPosition().y + 330;
	ExitYes = new GameButton("yes.png",StartPosX, StartPosY, 241, 43);
	ExitYes_Sel = new GameButton("yes_sel.png", StartPosX, StartPosY, 241, 43);
	ExitNo = new GameButton("no.png", StartPosX+270, StartPosY, 241, 43);
	ExitNo_Sel = new GameButton("no_sel.png", StartPosX+270, StartPosY, 241, 43);
}

void GameExit::render(sf::RenderTarget& target) {
	target.draw(Sprite);
	ExitYes->render(target);
	ExitYes->setVisible(true);
	ExitYes_Sel->render(target);
	ExitNo->render(target);
	ExitNo->setVisible(true);
	ExitNo_Sel->render(target);
}

GameExit::~GameExit() {
	delete ExitYes;
	delete ExitYes_Sel;
	delete ExitNo;
	delete ExitNo_Sel;
}

void GameExit::Open() {
	ExitOpen = true;
}
void GameExit::Close() {
	ExitOpen = false;
}
bool GameExit::isOpen() {
	return ExitOpen;
}

void GameExit::update(sf::Vector2f mouse) {
	if (ExitYes->inButton(mouse)) {
		SelectYes = true;
		ExitYes->setVisible(false);
		ExitYes_Sel->setVisible(true);
	}
	else {
		SelectYes = false;
		ExitYes->setVisible(true);
		ExitYes_Sel->setVisible(false);
	}
	if (ExitNo->inButton(mouse)) {
		SelectNo = true;
		ExitNo->setVisible(false);
		ExitNo_Sel->setVisible(true);
	}
	else {
		SelectNo = false;
		ExitNo->setVisible(true);
		ExitNo_Sel->setVisible(false);
	}
}

int GameExit::SelectedButton() {
	if (SelectYes)
		return YES;
	else if (SelectNo)
		return NO;
	else
		return NOTHING;
}