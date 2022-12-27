#include "Power.h"

Power::Power() : SpaceKey(false),ChangeDir(false),statePower(false),DraggedDistance(0) {
	//틀 초기 값
	Frame.loadFromFile("powerframe.png", sf::IntRect(0, 0, 484, 57));
	Fsprite.setTexture(Frame);
	Fsprite.setPosition(HDX1-35,HDY-10);
	
	//핸들 초기 값
	HandlePos.x = HDX1;
	HandlePos.y = HDY;
	Hwidth = 21;
	Hheight = 33;
	PHandle.loadFromFile("powerhandle.png", sf::IntRect(0, 0, Hwidth, Hheight));
	PHsprite.setTexture(PHandle);
	PHsprite.setPosition(HandlePos);
}

void Power::render(sf::RenderTarget& target) {
	sf::RectangleShape PowerBar;
	PowerBar.setOutlineColor(sf::Color::Black);
	PowerBar.setOutlineThickness(1);
	PowerBar.setPosition(HDX1+10,HDY+15);
	PowerBar.setSize(sf::Vector2f(DraggedDistance, 5));
	PowerBar.setFillColor(sf::Color(255,255 - 255*DraggedDistance/HDX2, 0));

	PHsprite.setPosition(HandlePos);
	target.draw(Fsprite);
	target.draw(PowerBar);
	target.draw(PHsprite);
}

float Power::getHwidth() {
	return Hwidth;
}
float Power::getHheight() {
	return Hheight;
}
sf::Vector2f Power::getHandlePosition() const{
	return HandlePos;
}

void Power::setHandlePosition(sf::Vector2f pos){
	HandlePos = pos;
}

void Power::InitHandlePosition() {
	HandlePos.x = HDX1;
	HandlePos.y = HDY;
}

void Power::FixRange() { //범위에 있는지 확인 후 없으면 교정
	if (HandlePos.x < HDX1)
		HandlePos.x = HDX1;
	else if (HandlePos.x > HDX2)
		HandlePos.x = HDX2;
	HandlePos.y = HDY;
	DraggedDistance = HandlePos.x - HDX1;
}

bool Power::inHandle(sf::Vector2f mouse) {
	//잡혔는지에 대한 검사
	if ((mouse.x <= HandlePos.x + Hwidth && mouse.x >= HandlePos.x) 
		&& (mouse.y <= HandlePos.y + Hheight && mouse.y >= HandlePos.y))
		return true;
	return false;
}

void Power::setDragPower(bool state) {
	statePower = state;
}
bool Power::isDraggingPower() const {
	return statePower;
}

float Power::getDraggedDistance() {
	return DraggedDistance;
}

void Power::InitDraggedDistance() {
	DraggedDistance = 0;
}

void Power::setPressedSpace(bool v) {
	SpaceKey = v;
}
bool Power::isPressedSpace() {
	return SpaceKey;
}

void Power::setChangeDir(bool v) {
	ChangeDir = v;
}
bool Power::isChangeDir() {
	return ChangeDir;
}