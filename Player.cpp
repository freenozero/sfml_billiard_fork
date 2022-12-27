#include "SampleBilliardBall.h"
#include "SampleBilliardBoard.h"
#include "BilliardPocket.h"
#include "SampleGame.h"
#include "Player.h"
#include "ScoreBoard.h"

//���� ���
Player* Player::TurnPlayer = nullptr;

Player::Player() : Player(0, false) {}

Player::Player(int num, bool turn) : Phase(BASIC)
, score(0), win(DEFAULT), ballType(BREAKSHOT), PlayerNum(num) {
	setTurn(turn);
	setPutBallCnt(0);
	NextP = nullptr;
	whether = DEFAULT;
}

Player::~Player(void) {
	NextP = nullptr;
	TurnPlayer = nullptr;
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
void Player::update(float timeElapsed) {}
void Player::collide(SampleBilliardObject& other) {} //�浹 �̺�Ʈ x


//8ball������Ʈ ó��
//�÷��̾� ���� 8����,�ӵ��� ����
void Player::EightBallupdate(SampleBilliardGameBall& playerBall, SampleBilliardObject& eightBall, int V) {
	if (win == WIN) {
		PocketSize = 0;
		return; //�̰�ٸ� ����x
	}
	//�÷��̾� ���� ��
	if (turn) {
		switch (Phase) {  //Phase : ���� ����
		case BASIC:
		{
			if (playerBall.getVelocity() == sf::Vector2f(0, 0)) {}
			else { //�÷��̾� ���� �ӵ��� ������ ��
				playerBall.setFoul(false);
				setPhase(MOVE); //�� ģ ���·� ����
			}
		}
		break;
		case MOVE:
		{
			if (V == 0) //���� �ӵ��� ��� 0�̶��
				setPhase(STOP); //���� ���·� ����
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
						yourLose(); //������ �й�
					}
					else { //8�� ���� �ƴ� ���
						if (ballType <= UNKNOWN) //BallType�� �����̶�� �й�
							yourLose();
						else if (isWin() == LAST) //��� �������� �־��ٸ�
							yourWin(); //�¸�
						else //�������� �ȳ־��µ� 8���� ���ٸ�
							yourLose();
					}
				}
				else if (BilliardPocket::InPocket(playerBall) >= 0) { //8���� ���� �÷��̾� ���� �� ���
					//�� ���� �� ����̹Ƿ� ������ �� ����
					playerBall.setPosition(800, 500); //�÷��̾�� ������ ���ϱ�
					BilliardPocket::outBall(BilliardPocket::InPocket(playerBall)); //����� ���Ͽ��� ����
					playerBall.setFoul(true);
					DiffSize = BilliardPocket::getSizePocket() - Player::getSizePocket(); //ũ������ �簻��
					IfTypeSet(DiffSize); //Ÿ�� �Ǵ� �� ����
					setTurn(false); //������
					getNextP().setTurn(true); //���� �÷��̾� �� 
				}
				else //8�� �÷��̾� ���� ������ �������� ���ٸ�
					IfTypeSet(DiffSize);
			}
			//��ȭ�� ���ٸ�
			else {
				if (ballType == BREAKSHOT) {
					setBallType(UNKNOWN);
					getNextP().setBallType(UNKNOWN);
				}
				setPhase(BASIC); //�� ���·� ����
				setTurn(false); //�� ����
				getNextP().setTurn(true); //���� �÷��̾�� �ϳѱ�
			}

			//���� �籸�� ������ ���������� ���� �Ǻ�
			if (!SampleBilliardBoard::inBoard(playerBall.getPosition())) {
				playerBall.setFoul(true);
				playerBall.setPosition(800, 500);
			}
		}
		break;
		}
	}
}

//8ball����: ballŸ�Կ� ���� ����
void Player::IfTypeSet(int DiffSize) {
	if (isLast())
		setWin(LAST);
	setPhase(BASIC);
	Player::setSizePocket(BilliardPocket::getSizePocket()); //���� ũ�� ����
	if (DiffSize > 0)
		setScore(stoi(getScore()) + (DiffSize));
	int str = 0; //��Ʈ������
	int sol = 0; //�ָ���
	if (DiffSize != 0) {
		for (int i = BilliardPocket::getSizePocket() - 1; i >= BilliardPocket::getSizePocket() - DiffSize; --i) {
			if (stoi(BilliardPocket::BallRef(i).getOwner()) > 8)
				str++;
			else
				sol++;
		}
	}
	if (ballType == BREAKSHOT) { //�극��ũ �� �ΰ�� 
		setBallType(UNKNOWN); //���� ���·� ����
		getNextP().setBallType(UNKNOWN);
	}
	else if (ballType == UNKNOWN) { //���� ������ ���
		//�� ���� Ȯ���� ���� �� �� ����
		if (str > sol) {
			setBallType(STRIPES);
			getNextP().setBallType(SOLIDS);
		}
		else if (str < sol) {
			setBallType(SOLIDS);
			getNextP().setBallType(STRIPES);
		}
	}
	else { //���� ������ ���
		if ((ballType == STRIPES && str == 0) || (ballType == SOLIDS && sol == 0)) {
			setTurn(false); //�� ����
			getNextP().setTurn(true); //���� �÷��̾�� �ϳѱ�
		}
		//�� ����
	}
}

//3ball ������Ʈ ó��
void Player::ThreeBallupdate(SampleBilliardGameBall& playerBall, int V) {
	if (win == WIN) {
		return; //�̰�ٸ� ����x
	}
	//�÷��̾� ���� ��
	if (turn) {
		switch (Phase) {  //Phase : ���� ����
		case BASIC:
		{
			if (playerBall.getVelocity() == sf::Vector2f(0, 0)) {}
			else { //�÷��̾� ���� �ӵ��� ������ ��
				playerBall.setFoul(false);
				setPhase(MOVE); //�� ģ ���·� ����
			}
		}
		break;
		case MOVE:
		{
			if (whether == DEFAULT) {
				if (playerBall.isNewCollideBall() && playerBall.getCollideBoardCnt() >= 3) {
					whether = SUCCESS;
				}
				else if (playerBall.isNewCollideBall() && playerBall.getCollideBoardCnt() < 3) {
					whether = FAILURE;
				}
			}
			if (V == 0) //�ӵ��� ��� 0�̶��
				setPhase(STOP); //ģ ���� ���·� ����
		}
		break;
		case STOP:
		{
			//��� �� �ʱ�ȭ
			playerBall.InitNewCollideBall();
			playerBall.setCollideBoardCnt(0);
			if (whether == SUCCESS) {
				setScore(score + 1); //1���� ������
				if (score >= 20)
					yourWin();
			}
			else {
				setTurn(false);
				getNextP().setTurn(true);
			}
			whether = DEFAULT;
			setPhase(BASIC);
			//���� �籸�� ������ ���������� ���� �Ǻ�
			if (!SampleBilliardBoard::inBoard(playerBall.getPosition())) {
				playerBall.setPosition(800, 500);
			}
		}
		break;
		}
	}
}
void Player::FourBallupdate(SampleBilliardGameBall& playerBall, SampleBilliardGameBall& playerBall1, int V) {

	if (win == WIN) {
		return; //�̰�ٸ� ����x
	}
	//�÷��̾� ���� ��
	if (turn) {
		switch (Phase) {  //Phase : ���� ����
		case BASIC:
		{
			if (playerBall.getVelocity() == sf::Vector2f(0, 0)) {}
			else { //�÷��̾� ���� �ӵ��� ������ ��
				playerBall.setFoul(false);
				setPhase(MOVE); //�� ģ ���·� ����
			}
			if (playerBall1.getVelocity() == sf::Vector2f(0, 0)) {}
			else {
				playerBall1.setFoul(false);
				setPhase(MOVE); //�� ģ ���·� ����
			}
		}
		break;
		case MOVE:
		{
			int i = 0;
			int ii = 0;



			// ����� ��Ģ
			if ((playerBall1.isNewCollideBall() != true) && (playerBall.isNewCollideBall() == true) && (playerBall.collideWithFourBall(playerBall1) == true) || (playerBall1.collideWithFourBall(playerBall) == true)) {
				i++;
			}
			if (whether == DEFAULT) {
				if (i >= 1) {
					whether = SUCCESS;
				}
				else if (i < 0) {
					whether = FAILURE;
				}
			}
			// ������� ��Ģ
			if ((playerBall1.isNewCollideBall() == true) && (playerBall.isNewCollideBall() != true) && (playerBall.collideWithFourBall(playerBall1) == true) || (playerBall1.collideWithFourBall(playerBall) == true)) {
				ii++;
			}
			if (whether == DEFAULT) {
				if (ii >= 1) {
					whether = SUCCESS;
				}
				else if (ii < 0) {
					whether = FAILURE;
				}
			}

			if ((playerBall.collideWithFourBall(playerBall1) == false) || (playerBall1.collideWithFourBall(playerBall) == false)) {
				i = -10;
				ii = -10;
				whether = FAILURE;
			}


			if (V == 0) { //�ӵ��� ��� 0�̶��

				setPhase(STOP); //ģ ���� ���·� ����
			}
		}
		break;
		case STOP:
		{
			//��� �� �ʱ�ȭ
			playerBall.InitNewCollideBall();
			playerBall.setCollideBoardCnt(0);
			playerBall1.InitNewCollideBall();
			playerBall1.setCollideBoardCnt(0);
			if (whether == SUCCESS) {
				setScore(score + 1); //1���� ������
				if (score >= 20)
					yourWin();
			}
			else {
				setTurn(false);
				getNextP().setTurn(true);
			}
			whether = DEFAULT;
			setPhase(BASIC);
		}
		break;
		}
	}

}
//8ball ���� ������
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

	if (ballType == BREAKSHOT)
		BallText.setString("BreakShot");
	else if (ballType == UNKNOWN)
		BallText.setString("Unknown");
	else if (ballType == SOLIDS)
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
	BallText.setPosition(60.f + (PlayerNum - 1) * 190.f, 150.f);
	if (ballType > 0)
		BallText.setPosition(80.f + (PlayerNum - 1) * 190.f, 150.f);

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

//3ball ���� ������
void Player::ThreeBallrender(sf::RenderTarget& target) {
	// ������
	sf::Text P_score;
	sf::Text ScoreText;
	sf::Text TurnText;
	sf::Text WinText;

	//��Ʈ ����
	P_score.setFont(SampleGame::getFont());
	ScoreText.setFont(SampleGame::getFont());
	TurnText.setFont(SampleGame::getFont());
	WinText.setFont(SampleGame::getFont());

	//��
	P_score.setFillColor(sf::Color::White);
	ScoreText.setFillColor(sf::Color::Cyan);
	TurnText.setFillColor(sf::Color::Cyan);
	WinText.setFillColor(sf::Color::Cyan);

	//ũ��
	P_score.setCharacterSize(80);
	ScoreText.setCharacterSize(50);
	TurnText.setCharacterSize(40);
	WinText.setCharacterSize(40);

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

	P_score.setPosition(120.f + (PlayerNum - 1) * 180.f, 250.f);
	ScoreText.setPosition(70.f + (PlayerNum - 1) * 180.f, 200.f);
	if (turn == true)
		TurnText.setPosition(100.f + (PlayerNum - 1) * 180.f, 350.f);
	if (win == true)
		WinText.setPosition(100.f + (PlayerNum - 1) * 180.f, 350.f);

	target.draw(P_score);
	target.draw(ScoreText);
	if (turn == true) {
		if (win == true)
			target.draw(WinText);
		else
			target.draw(TurnText);
	}
}
void Player::FourBallrender(sf::RenderTarget& target) {
	// ������
	sf::Text P_score;
	sf::Text ScoreText;
	sf::Text TurnText;
	sf::Text WinText;
	sf::Text BallText;
	sf::Text BallText1;

	//��Ʈ ����
	P_score.setFont(SampleGame::getFont());
	ScoreText.setFont(SampleGame::getFont());
	TurnText.setFont(SampleGame::getFont());
	WinText.setFont(SampleGame::getFont());
	BallText.setFont(SampleGame::getFont());
	BallText1.setFont(SampleGame::getFont());
	//��
	P_score.setFillColor(sf::Color::White);
	ScoreText.setFillColor(sf::Color::Cyan);
	TurnText.setFillColor(sf::Color::Cyan);
	WinText.setFillColor(sf::Color::Cyan);
	BallText.setFillColor(sf::Color::Cyan);
	BallText1.setFillColor(sf::Color::Cyan);
	//ũ��
	P_score.setCharacterSize(80);
	ScoreText.setCharacterSize(50);
	TurnText.setCharacterSize(40);
	WinText.setCharacterSize(40);
	BallText.setCharacterSize(40);
	BallText1.setCharacterSize(40);
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

	// ��� ������ ���� ������ ����
	//BallText.setString("White");
	//BallText1.setString("Yellow");


	P_score.setPosition(120.f + (PlayerNum - 1) * 180.f, 250.f);
	ScoreText.setPosition(70.f + (PlayerNum - 1) * 180.f, 200.f);
	if (turn == true)
		TurnText.setPosition(100.f + (PlayerNum - 1) * 180.f, 350.f);
	if (win == true)
		WinText.setPosition(100.f + (PlayerNum - 1) * 180.f, 350.f);
	int a = 0;
	if (a == 0) {
		BallText.setPosition(60.f + (PlayerNum - 1) * 190.f, 150.f);
		a++;
	}

	BallText1.setPosition(80.f + (PlayerNum - 1) * 190.f, 150.f);

	target.draw(P_score);
	target.draw(ScoreText);
	if (turn == true) {
		if (win == true)
			target.draw(WinText);
		else
			target.draw(TurnText);
	}
	target.draw(BallText);
	target.draw(BallText1);

	P_score.setPosition(120.f + (PlayerNum - 1) * 180.f, 250.f);
	ScoreText.setPosition(70.f + (PlayerNum - 1) * 180.f, 200.f);
	if (turn == true)
		TurnText.setPosition(100.f + (PlayerNum - 1) * 180.f, 350.f);
	if (win == true)
		WinText.setPosition(100.f + (PlayerNum - 1) * 180.f, 350.f);

	target.draw(P_score);
	target.draw(ScoreText);
	if (turn == true) {
		if (win == true)
			target.draw(WinText);
		else
			target.draw(TurnText);
	}
}
int Player::getPlayerNum() const {
	return PlayerNum;
}

//���� �÷��̾� ��ȯ
Player& Player::WhoisTurn() {
	return *TurnPlayer;
}

void Player::setTurn(bool turn) {
	this->turn = turn;
	if (turn) //�ϼ���
		TurnPlayer = this; //�ش� �÷��̾ ���� �÷��̾�� ����
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

void Player::yourWin() {
	setPhase(BASIC);
	setTurn(true);
	this->win = WIN;
}

void Player::yourLose() {
	setPhase(BASIC);
	setTurn(false);
	getNextP().yourWin();
	//�ش� �÷��̾� �й� == ��� �÷��̾� �¸�
}

void Player::setWin(int n) {
	win = n;
}
int Player::isWin() const {
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

void Player::setPutBallCnt(int num) {
	PutBallCnt = num;
}
int Player::getPutBallCnt() const {
	return PutBallCnt;
}

bool Player::isLast() {
	//�÷��̾��� ��� �������� ���� ���
	if ((ballType == SOLIDS && BilliardPocket::getCntSolids() == 7)
		|| (ballType == STRIPES && BilliardPocket::getCntStripes() == 7))
		return true;
	else
		return false;
}

void Player::setNextP(Player& p) {
	NextP = &p;
}
Player& Player::getNextP()const {
	return *NextP;
}

//���� �ʱ�� �ǵ��� 
//���� ����� �� �Լ� ��� �� => ���⿡�� �ϸ� �ٽ� �������ָ� ��.
void Player::Init() {
	TurnPlayer = nullptr;
	PocketSize = 0;
	turn = false;
	Phase = BASIC;
	whether = DEFAULT;
	win = DEFAULT;
	score = 0;
	ballType = BREAKSHOT;
	PutBallCnt = 0;
	NextP = nullptr;
}