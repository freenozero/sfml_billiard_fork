#include "SampleBilliardBall.h"
#include "SampleBilliardBoard.h"
#include "BilliardPocket.h"
#include "SampleGame.h"
#include "Player.h"
#include "ScoreBoard.h"

Player::Player() : Player(0,false){}

Player::Player(int num,bool turn) : turn(turn), Phase(BASIC)
,score(0), win(false), ballType(BREAKSHOT),PlayerNum(num) {
	NextP = nullptr;
}

Player::~Player(void) {
	NextP = nullptr;
	//NextP�� SampleGame���� �Ҹ��
}

int Player::PocketSize = 0;

void Player::setSizePocket(int size) {
	PocketSize = size;
}
int Player::getSizePocket() {
	return PocketSize;
}

//�����Լ�
void Player::update(float timeElapsed) {} //���� �ð������� �ڵ����� �ϳѱ�� �� �� ����?
void Player::collide(SampleBilliardObject& other) {} //�浹 �̺�Ʈ x

//������Ʈ
//�÷��̾� ���� 8����,�ӵ��� ����
void Player::update(SampleBilliardGameBall& playerBall,SampleBilliardObject& eightBall,int V){ 	
	if (win == true) return; //�̰�ٸ� ����x
	//�÷��̾� ���� ��
	if (turn) {  
		//Phase : ���� ����
		switch (Phase) {
		case BASIC: 
		{
			if (playerBall.getVelocity() == sf::Vector2f(0, 0)) {}
			else //�÷��̾� ���� �ӵ��� ������ ��
				setPhase(MOVE); //�� ģ ���·� ����
		}
		break;
		case MOVE: 
		{
			if (V == 0) //�ӵ��� ��� 0�̶��
				setPhase(STOP); //ģ ���� ���·� ����
		}
		break;
		case STOP: 
		{
			//���� ũ���� ��ȭ�� �ִٸ�
			if (BilliardPocket::getSizePocket() != Player::getSizePocket()) {
				//�̹� �Ͽ� �� ���� ���� ����.
				int DiffSize = BilliardPocket::getSizePocket() - Player::getSizePocket();

				if (BilliardPocket::InPocket(eightBall) >= 0) { //8������ �� ���
					if (BilliardPocket::InPocket(playerBall) >= 0) { //�÷��̾� ���� �� ���
						yourLose(true); //������ �й�
					}
					else {
						if (ballType <= 0) { //BallType�� �����̶�� �й�
							yourLose(true);
						}
						else if (BilliardPocket::InPocket(eightBall) ==
							BilliardPocket::getSizePocket() - 1)//8������ ������ ����� �� 
						{
							//�÷��̾��� ��� �������� ���� ���
							if (ballType == SOLIDS && BilliardPocket::getCntSolids() == 7)
								yourWin(true); //�¸�
							else if (ballType == STRIPES && BilliardPocket::getCntStripes() == 7)
								yourWin(true); //�¸�
							else //�������� ������ ���
								yourLose(true);
						}
						else //8������ ������ ��Ұ� �ƴ� ��� �й�
							yourLose(true);
					}
				}
				else if (BilliardPocket::InPocket(playerBall) >= 0) { //8���� ���� �÷��̾� ���� �� ���
					Phase = BASIC;
					turn = false; //������
					getNextP().setTurn(true); //���� �÷��̾� �� 
					playerBall.setPosition(800, 500); //�÷��̾�� ������ ���ϱ�
					BilliardPocket::outBall(BilliardPocket::InPocket(playerBall)); //����� ���Ͽ��� ����
					Player::setSizePocket(BilliardPocket::getSizePocket()); //���� ũ�� ����

					if (ballType == BREAKSHOT) { //�극��ũ �� �ΰ�� 
						setBallType(UNKNOWN); //���� ���·� ����
						getNextP().setBallType(UNKNOWN);
					}
					else if (ballType == UNKNOWN && DiffSize >= 2) { //���� ������ ���,�÷��̾ ���� 2���̻�
						//���� �� ���̰� � ������ ������ Ȯ��
						int str = 0; //��Ʈ������
						int sol = 0; //�ָ���
						for (int i = BilliardPocket::getSizePocket() - 1; i >= DiffSize - 2; --i) {
							if (stoi(BilliardPocket::BallRef(i).getOwner()) > 8)
								str++;
							else
								sol++;
						}
						if (str == sol) {} //�� ������ ���ڰ� ���� ���
						else { 
							if (stoi(BilliardPocket::BallRef(BilliardPocket::getSizePocket() - 1).getOwner()) > 8) {
								setBallType(STRIPES);
								getNextP().setBallType(SOLIDS);
							}
							else {
								setBallType(SOLIDS);
								getNextP().setBallType(STRIPES);
							}
						}
					}
					else if (ballType != UNKNOWN) {
						//�� Ÿ���� ������ ���
						setScore(stoi(getScore()) + (DiffSize - 1)); //��� �� ���ڸ�ŭ ����.
					}
				}
				else { //8�� �÷��̾� ���� ������ �������� ���ٸ�
					setPhase(BASIC);
					setScore(stoi(getScore()) + DiffSize); //�� ����ŭ ����
					Player::setSizePocket(BilliardPocket::getSizePocket()); //���� ���� ���� ����

					if (ballType == BREAKSHOT) { //�극��ũ �� �ΰ�� 
						setBallType(UNKNOWN); //���� ���·� ����
						getNextP().setBallType(UNKNOWN);
					}
					else if (ballType == UNKNOWN) { //���� ������ ���
						//���� �� ���̰� � ������ ������ Ȯ��
						int str = 0;
						int sol = 0;
						for (int i = BilliardPocket::getSizePocket() - 1; i >= DiffSize - 1; --i) {
							if (stoi(BilliardPocket::BallRef(i).getOwner()) > 8)
								str++;
							else
								sol++;
						}
						if (str == sol) {} //�� ���ڰ� ���� ���
						else {
							if (stoi(BilliardPocket::BallRef(BilliardPocket::getSizePocket() - 1).getOwner()) > 8) {
								setBallType(STRIPES);
								getNextP().setBallType(SOLIDS);
							}
							else {
								setBallType(SOLIDS);
								getNextP().setBallType(STRIPES);
							}
						}
					}
					else{ //���� ������ ���
						//� ������ Ȯ���ϰ� �ڽ��� ���� ������ ������
						//�װ� �ƴϸ� �� �ѱ�
					}
					//�� ����
				}
			}
			else { //��ȭ�� ���ٸ�
				if (ballType == BREAKSHOT) { 
					setBallType(UNKNOWN); 
					getNextP().setBallType(UNKNOWN);
				}
				setPhase(BASIC); //�� ���·� ����
				setTurn(false); //�� ����
				getNextP().setTurn(true); //���� �÷��̾�� �ϳѱ�
			}
		}
		break;
		}
	}
	else
		return;
}

void Player::render(sf::RenderTarget& target) {
	// ������
	sf::Text P_score;
	sf::Text ScoreText;
	sf::Text TurnText;
	sf::Text WinText;
	sf::Text BallText;

	//��Ʈ ����
	P_score.setFont(SampleGame::getFont());
	ScoreText.setFont(SampleGame::getFont());
	TurnText.setFont(SampleGame::getFont());
	WinText.setFont(SampleGame::getFont());
	BallText.setFont(SampleGame::getFont());

	//��
	P_score.setFillColor(sf::Color::White);
	ScoreText.setFillColor(sf::Color::Cyan);
	TurnText.setFillColor(sf::Color::Cyan);
	WinText.setFillColor(sf::Color::Cyan);
	BallText.setFillColor(sf::Color::Cyan);

	//ũ��
	P_score.setCharacterSize(80);
	ScoreText.setCharacterSize(50);
	TurnText.setCharacterSize(40);
	WinText.setCharacterSize(40);
	BallText.setCharacterSize(40);

	//���� ������
	P_score.setString(getScore());
	P_score.setOutlineColor(sf::Color::Cyan);
	P_score.setOutlineThickness(1.f);

	std::string str1 = "player";
	std::string str2 = std::to_string(PlayerNum);
	ScoreText.setString(str1 + str2);
	ScoreText.setOutlineColor(sf::Color::White);
	ScoreText.setOutlineThickness(1.f);

	TurnText.setString("Turn");
	TurnText.setOutlineColor(sf::Color::Red);
	TurnText.setOutlineThickness(1.f);

	WinText.setString("Win");
	WinText.setOutlineColor(sf::Color::Yellow);
	WinText.setOutlineThickness(1.f);

	if (ballType <= 0)
		BallText.setString("Unknown");
	else if (ballType == 1)
		BallText.setString("Solids");
	else
		BallText.setString("Stripes");
	BallText.setOutlineColor(sf::Color::Yellow);
	BallText.setOutlineThickness(1.f);

	P_score.setPosition(120.f + (PlayerNum - 1) * 180.f, 250.f);
	ScoreText.setPosition(70.f + (PlayerNum - 1) * 180.f, 200.f);
	if (turn == true)
		TurnText.setPosition(100.f + (PlayerNum - 1) * 180.f, 350.f);
	if (win == true)
		WinText.setPosition(100.f + (PlayerNum - 1) * 180.f, 350.f);
	BallText.setPosition(50.f + (PlayerNum - 1) * 190.f, 150.f);

	target.draw(P_score);
	target.draw(ScoreText);
	if (turn == true) {
		if (win == true)
			target.draw(WinText);
		else
			target.draw(TurnText);
	}
	target.draw(BallText);

}


void Player::setTurn(bool turn) {
	this->turn = turn;
}
bool Player::isTurn() const {
	return turn;
}

void Player::setPhase(int Phase) {
	this->Phase = Phase;
}
int Player::isPhase() const {
	return Phase;
}

std::string Player::getScore() const {
	return std::to_string(score);
}

void Player::setScore(int param) {
	score = param;
}

void Player::setScore(std::string param) {
	score = stoi(getScore()) + stoi(param);
}

void Player::yourWin(bool win) {
	Phase = 0;
	turn = true;
	this->win = win;
}

void Player::yourLose(bool lose) {
	Phase = 0;
	turn = false;
	getNextP().yourWin(true); //�ش� �÷��̾� �й� == ��� �÷��̾� �¸�
}
bool Player::isWin() const {
	return win;
}

void Player::setBallType(int type) {
	if (0 < type && type < 8) {
		ballType = SOLIDS; 
	}
	else if (type > 8) {
		ballType = STRIPES;
	}
	else if (type == 0)
		ballType = UNKNOWN;
	else
		ballType = BREAKSHOT;
}
int Player::getBallType() const {
	return ballType;
}


void Player::setNextP(Player& p) {
	NextP = &p;
}

Player& Player::getNextP()const{
	return *NextP;
}