#include "SampleBilliardBall.h"
#include "SampleBilliardBoard.h"
#include "BilliardPocket.h"
#include "SampleGame.h"
#include "Player.h"
#include "ScoreBoard.h"

//정적 멤버
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
	//NextP는 SampleGame에서 소멸됨
}

int Player::PocketSize = 0;

void Player::setSizePocket(int size) {
	PocketSize = size;
}
int Player::getSizePocket() {
	return PocketSize;
}

//가상함수
void Player::update(float timeElapsed) {}
void Player::collide(SampleBilliardObject& other) {} //충돌 이벤트 x


//8ball업데이트 처리
//플레이어 볼과 8번공,속도를 받음
void Player::EightBallupdate(SampleBilliardGameBall& playerBall, SampleBilliardObject& eightBall, int V) {
	if (win == WIN) {
		PocketSize = 0;
		return; //이겼다면 실행x
	}
	//플레이어 턴일 때
	if (turn) {
		switch (Phase) {  //Phase : 공의 상태
		case BASIC:
		{
			if (playerBall.getVelocity() == sf::Vector2f(0, 0)) {}
			else { //플레이어 볼의 속도가 변했을 때
				playerBall.setFoul(false);
				setPhase(MOVE); //공 친 상태로 변경
			}
		}
		break;
		case MOVE:
		{
			if (V == 0) //공의 속도가 모두 0이라면
				setPhase(STOP); //정지 상태로 변경
		}
		break;
		case STOP:
		{
			//포켓 크기의 변화가 있다면
			if (BilliardPocket::getSizePocket() != Player::getSizePocket()) {
				//이번 턴에 들어간 공의 수를 구함.
				int DiffSize = BilliardPocket::getSizePocket() - Player::getSizePocket();

				if (BilliardPocket::InPocket(eightBall) >= 0) { //8번공이 들어간 경우
					if (BilliardPocket::InPocket(playerBall) >= 0) { //플레이어 볼이 들어간 경우
						yourLose(); //무조건 패배
					}
					else { //8번 공이 아닌 경우
						if (ballType <= UNKNOWN) //BallType이 미정이라면 패배
							yourLose();
						else if (isWin() == LAST) //모든 목적볼을 넣었다면
							yourWin(); //승리
						else //목적볼을 안넣었는데 8번볼 들어갔다면
							yourLose();
					}
				}
				else if (BilliardPocket::InPocket(playerBall) >= 0) { //8번공 제외 플레이어 볼이 들어간 경우
					//흰 공이 들어간 경우이므로 무조건 턴 변경
					playerBall.setPosition(800, 500); //플레이어볼의 포지션 정하기
					BilliardPocket::outBall(BilliardPocket::InPocket(playerBall)); //흰공을 포켓에서 꺼냄
					playerBall.setFoul(true);
					DiffSize = BilliardPocket::getSizePocket() - Player::getSizePocket(); //크기차이 재갱신
					IfTypeSet(DiffSize); //타입 판단 후 결정
					setTurn(false); //턴종료
					getNextP().setTurn(true); //다음 플레이어 턴 
				}
				else //8과 플레이어 볼을 제외한 목적구가 들어갔다면
					IfTypeSet(DiffSize);
			}
			//변화가 없다면
			else {
				if (ballType == BREAKSHOT) {
					setBallType(UNKNOWN);
					getNextP().setBallType(UNKNOWN);
				}
				setPhase(BASIC); //전 상태로 돌림
				setTurn(false); //턴 종료
				getNextP().setTurn(true); //다음 플레이어에게 턴넘김
			}

			//공이 당구대 밖으로 나갔는지에 대한 판별
			if (!SampleBilliardBoard::inBoard(playerBall.getPosition())) {
				playerBall.setFoul(true);
				playerBall.setPosition(800, 500);
			}
		}
		break;
		}
	}
}

//8ball전용: ball타입에 따른 설정
void Player::IfTypeSet(int DiffSize) {
	if (isLast())
		setWin(LAST);
	setPhase(BASIC);
	Player::setSizePocket(BilliardPocket::getSizePocket()); //포켓 크기 갱신
	if (DiffSize > 0)
		setScore(stoi(getScore()) + (DiffSize));
	int str = 0; //스트라이프
	int sol = 0; //솔리드
	if (DiffSize != 0) {
		for (int i = BilliardPocket::getSizePocket() - 1; i >= BilliardPocket::getSizePocket() - DiffSize; --i) {
			if (stoi(BilliardPocket::BallRef(i).getOwner()) > 8)
				str++;
			else
				sol++;
		}
	}
	if (ballType == BREAKSHOT) { //브레이크 샷 인경우 
		setBallType(UNKNOWN); //미정 상태로 변경
		getNextP().setBallType(UNKNOWN);
	}
	else if (ballType == UNKNOWN) { //미정 상태인 경우
		//들어간 공이 확실히 구분 될 때 정함
		if (str > sol) {
			setBallType(STRIPES);
			getNextP().setBallType(SOLIDS);
		}
		else if (str < sol) {
			setBallType(SOLIDS);
			getNextP().setBallType(STRIPES);
		}
	}
	else { //공이 정해진 경우
		if ((ballType == STRIPES && str == 0) || (ballType == SOLIDS && sol == 0)) {
			setTurn(false); //턴 종료
			getNextP().setTurn(true); //다음 플레이어에게 턴넘김
		}
		//턴 유지
	}
}

//3ball 업데이트 처리
void Player::ThreeBallupdate(SampleBilliardGameBall& playerBall, int V) {
	if (win == WIN) {
		return; //이겼다면 실행x
	}
	//플레이어 턴일 때
	if (turn) {
		switch (Phase) {  //Phase : 공의 상태
		case BASIC:
		{
			if (playerBall.getVelocity() == sf::Vector2f(0, 0)) {}
			else { //플레이어 볼의 속도가 변했을 때
				playerBall.setFoul(false);
				setPhase(MOVE); //공 친 상태로 변경
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
			if (V == 0) //속도가 모두 0이라면
				setPhase(STOP); //친 후의 상태로 변경
		}
		break;
		case STOP:
		{
			//모든 값 초기화
			playerBall.InitNewCollideBall();
			playerBall.setCollideBoardCnt(0);
			if (whether == SUCCESS) {
				setScore(score + 1); //1득점 턴유지
				if (score >= 20)
					yourWin();
			}
			else {
				setTurn(false);
				getNextP().setTurn(true);
			}
			whether = DEFAULT;
			setPhase(BASIC);
			//공이 당구대 밖으로 나갔는지에 대한 판별
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
		return; //이겼다면 실행x
	}
	//플레이어 턴일 때
	if (turn) {
		switch (Phase) {  //Phase : 공의 상태
		case BASIC:
		{
			if (playerBall.getVelocity() == sf::Vector2f(0, 0)) {}
			else { //플레이어 볼의 속도가 변했을 때
				playerBall.setFoul(false);
				setPhase(MOVE); //공 친 상태로 변경
			}
			if (playerBall1.getVelocity() == sf::Vector2f(0, 0)) {}
			else {
				playerBall1.setFoul(false);
				setPhase(MOVE); //공 친 상태로 변경
			}
		}
		break;
		case MOVE:
		{
			int i = 0;
			int ii = 0;



			// 흰공의 규칙
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
			// 노란공의 규칙
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


			if (V == 0) { //속도가 모두 0이라면

				setPhase(STOP); //친 후의 상태로 변경
			}
		}
		break;
		case STOP:
		{
			//모든 값 초기화
			playerBall.InitNewCollideBall();
			playerBall.setCollideBoardCnt(0);
			playerBall1.InitNewCollideBall();
			playerBall1.setCollideBoardCnt(0);
			if (whether == SUCCESS) {
				setScore(score + 1); //1득점 턴유지
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
//8ball 점수 랜더링
void Player::render(sf::RenderTarget& target) {
	// 점수판
	sf::Text P_score;
	sf::Text ScoreText;
	sf::Text TurnText;
	sf::Text WinText;
	sf::Text BallText;

	//폰트 설정
	P_score.setFont(SampleGame::getFont());
	ScoreText.setFont(SampleGame::getFont());
	TurnText.setFont(SampleGame::getFont());
	WinText.setFont(SampleGame::getFont());
	BallText.setFont(SampleGame::getFont());

	//색
	P_score.setFillColor(sf::Color::White);
	ScoreText.setFillColor(sf::Color::Cyan);
	TurnText.setFillColor(sf::Color::Cyan);
	WinText.setFillColor(sf::Color::Cyan);
	BallText.setFillColor(sf::Color::Cyan);

	//크기
	P_score.setCharacterSize(80);
	ScoreText.setCharacterSize(50);
	TurnText.setCharacterSize(40);
	WinText.setCharacterSize(40);
	BallText.setCharacterSize(40);

	//점수 가져옴
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

//3ball 점수 렌더링
void Player::ThreeBallrender(sf::RenderTarget& target) {
	// 점수판
	sf::Text P_score;
	sf::Text ScoreText;
	sf::Text TurnText;
	sf::Text WinText;

	//폰트 설정
	P_score.setFont(SampleGame::getFont());
	ScoreText.setFont(SampleGame::getFont());
	TurnText.setFont(SampleGame::getFont());
	WinText.setFont(SampleGame::getFont());

	//색
	P_score.setFillColor(sf::Color::White);
	ScoreText.setFillColor(sf::Color::Cyan);
	TurnText.setFillColor(sf::Color::Cyan);
	WinText.setFillColor(sf::Color::Cyan);

	//크기
	P_score.setCharacterSize(80);
	ScoreText.setCharacterSize(50);
	TurnText.setCharacterSize(40);
	WinText.setCharacterSize(40);

	//점수 가져옴
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
	// 점수판
	sf::Text P_score;
	sf::Text ScoreText;
	sf::Text TurnText;
	sf::Text WinText;
	sf::Text BallText;
	sf::Text BallText1;

	//폰트 설정
	P_score.setFont(SampleGame::getFont());
	ScoreText.setFont(SampleGame::getFont());
	TurnText.setFont(SampleGame::getFont());
	WinText.setFont(SampleGame::getFont());
	BallText.setFont(SampleGame::getFont());
	BallText1.setFont(SampleGame::getFont());
	//색
	P_score.setFillColor(sf::Color::White);
	ScoreText.setFillColor(sf::Color::Cyan);
	TurnText.setFillColor(sf::Color::Cyan);
	WinText.setFillColor(sf::Color::Cyan);
	BallText.setFillColor(sf::Color::Cyan);
	BallText1.setFillColor(sf::Color::Cyan);
	//크기
	P_score.setCharacterSize(80);
	ScoreText.setCharacterSize(50);
	TurnText.setCharacterSize(40);
	WinText.setCharacterSize(40);
	BallText.setCharacterSize(40);
	BallText1.setCharacterSize(40);
	//점수 가져옴
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

	// 어느 색깔이 본인 공인지 구분
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

//턴인 플레이어 반환
Player& Player::WhoisTurn() {
	return *TurnPlayer;
}

void Player::setTurn(bool turn) {
	this->turn = turn;
	if (turn) //턴설정
		TurnPlayer = this; //해당 플레이어를 턴인 플레이어로 설정
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
	//해당 플레이어 패배 == 상대 플레이어 승리
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
	//플레이어의 모든 목적볼을 넣은 경우
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

//생성 초기로 되돌림 
//게임 재시작 시 함수 사용 후 => 여기에서 턴만 다시 설정해주면 됨.
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