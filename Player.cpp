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
void Player::update(float timeElapsed) {} //일정 시간지나면 자동으로 턴넘기기 할 때 쓰면?
void Player::collide(SampleBilliardObject& other) {} //충돌 이벤트 x

//업데이트
//플레이어 볼과 8번공,속도를 받음
void Player::update(SampleBilliardGameBall& playerBall,SampleBilliardObject& eightBall,int V){ 	
	if (win == true) return; //이겼다면 실행x
	//플레이어 턴일 때
	if (turn) {  
		//Phase : 공의 상태
		switch (Phase) {
		case BASIC: 
		{
			if (playerBall.getVelocity() == sf::Vector2f(0, 0)) {}
			else //플레이어 볼의 속도가 변했을 때
				setPhase(MOVE); //공 친 상태로 변경
		}
		break;
		case MOVE: 
		{
			if (V == 0) //속도가 모두 0이라면
				setPhase(STOP); //친 후의 상태로 변경
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
						yourLose(true); //무조건 패배
					}
					else {
						if (ballType <= 0) { //BallType이 미정이라면 패배
							yourLose(true);
						}
						else if (BilliardPocket::InPocket(eightBall) ==
							BilliardPocket::getSizePocket() - 1)//8번공이 마지막 요소일 때 
						{
							//플레이어의 모든 목적볼을 넣은 경우
							if (ballType == SOLIDS && BilliardPocket::getCntSolids() == 7)
								yourWin(true); //승리
							else if (ballType == STRIPES && BilliardPocket::getCntStripes() == 7)
								yourWin(true); //승리
							else //목적공을 못넣은 경우
								yourLose(true);
						}
						else //8번공이 마지막 요소가 아닌 경우 패배
							yourLose(true);
					}
				}
				else if (BilliardPocket::InPocket(playerBall) >= 0) { //8번공 제외 플레이어 볼이 들어간 경우
					Phase = BASIC;
					turn = false; //턴종료
					getNextP().setTurn(true); //다음 플레이어 턴 
					playerBall.setPosition(800, 500); //플레이어볼의 포지션 정하기
					BilliardPocket::outBall(BilliardPocket::InPocket(playerBall)); //흰공을 포켓에서 꺼냄
					Player::setSizePocket(BilliardPocket::getSizePocket()); //포켓 크기 갱신

					if (ballType == BREAKSHOT) { //브레이크 샷 인경우 
						setBallType(UNKNOWN); //미정 상태로 변경
						getNextP().setBallType(UNKNOWN);
					}
					else if (ballType == UNKNOWN && DiffSize >= 2) { //미정 상태인 경우,플레이어볼 포함 2개이상
						//공이 몇 개이고 어떤 종류가 들어갔는지 확인
						int str = 0; //스트라이프
						int sol = 0; //솔리드
						for (int i = BilliardPocket::getSizePocket() - 1; i >= DiffSize - 2; --i) {
							if (stoi(BilliardPocket::BallRef(i).getOwner()) > 8)
								str++;
							else
								sol++;
						}
						if (str == sol) {} //두 종류의 숫자가 같은 경우
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
						//공 타입이 정해진 경우
						setScore(stoi(getScore()) + (DiffSize - 1)); //흰공 뺀 숫자만큼 득점.
					}
				}
				else { //8과 플레이어 볼을 제외한 목적구가 들어갔다면
					setPhase(BASIC);
					setScore(stoi(getScore()) + DiffSize); //들어간 수만큼 득점
					Player::setSizePocket(BilliardPocket::getSizePocket()); //포켓 공의 개수 갱신

					if (ballType == BREAKSHOT) { //브레이크 샷 인경우 
						setBallType(UNKNOWN); //미정 상태로 변경
						getNextP().setBallType(UNKNOWN);
					}
					else if (ballType == UNKNOWN) { //미정 상태인 경우
						//공이 몇 개이고 어떤 종류가 들어갔는지 확인
						int str = 0;
						int sol = 0;
						for (int i = BilliardPocket::getSizePocket() - 1; i >= DiffSize - 1; --i) {
							if (stoi(BilliardPocket::BallRef(i).getOwner()) > 8)
								str++;
							else
								sol++;
						}
						if (str == sol) {} //두 숫자가 같은 경우
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
					else{ //공이 정해진 경우
						//어떤 공인지 확인하고 자신의 공이 들어갔으면 턴유지
						//그게 아니면 턴 넘김
					}
					//턴 유지
				}
			}
			else { //변화가 없다면
				if (ballType == BREAKSHOT) { 
					setBallType(UNKNOWN); 
					getNextP().setBallType(UNKNOWN);
				}
				setPhase(BASIC); //전 상태로 돌림
				setTurn(false); //턴 종료
				getNextP().setTurn(true); //다음 플레이어에게 턴넘김
			}
		}
		break;
		}
	}
	else
		return;
}

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
	getNextP().yourWin(true); //해당 플레이어 패배 == 상대 플레이어 승리
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