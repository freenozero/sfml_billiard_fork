#include "BilliardPocket.h"
#include "SampleGame.h"

//정적변수
vector<SampleBilliardObject*> BilliardPocket::Pocket;

//소멸자.
BilliardPocket::~BilliardPocket() {
	//포켓 클리어
	initPocket();
}

void BilliardPocket::initPocket() {
	Pocket.clear();
}

//오브젝트 충돌 재정의
void BilliardPocket::collide(SampleBilliardObject& other)
{
	// 공과 충돌할 경우 
	if (dynamic_cast<SampleBilliardBall*>(&other) != nullptr)
	{
		SampleBilliardBall& ball = *dynamic_cast<SampleBilliardBall*>(&other);
		collideWithBall(ball);
	}
	//당구대 충돌은 없어도 됨.
}

//포켓과 공 충돌
void BilliardPocket::collideWithBall(SampleBilliardBall& other)
{
	// 동일한 공 비교시 종료 
	if (this == &other)
	{
		return;
	}
	// 거리 계산 
	sf::Vector2f distance = getPosition() - other.getPosition();
	float distanceBetween = (sqrtf((distance.x * distance.x) + (distance.y * distance.y)));

	// 두 공이 겹치는지 검사 
	//닿으면 바로 넣어지지 않고 특정 비율 넘어가면 포켓에 넣는 것으로 판정
	if (distanceBetween < getRadius()/1.5)  
	{
		// 겹치는 정도 계산 
		float overlap = (distanceBetween - getRadius() - other.getRadius()) / 2.f;
		float moveX = (overlap * (getPosition().x - other.getPosition().x) / distanceBetween);
		float moveY = (overlap * (getPosition().y - other.getPosition().y) / distanceBetween);

		// 충돌 후 속도 계산하여 적용
		sf::Vector2f normal(distance.x / distanceBetween, distance.y / distanceBetween);
		sf::Vector2f tangential(-normal.y, normal.x);

		float dotProductTangential1 = getVelocity().x * tangential.x + getVelocity().y * tangential.y;
		float dotProductTangential2 = other.getVelocity().x * tangential.x + other.getVelocity().y * tangential.y;

		float dotProductNormal1 = getVelocity().x * normal.x + getVelocity().y * normal.y;
		float dotProductNormal2 = other.getVelocity().x * normal.x + other.getVelocity().y * normal.y;

		float m1 = (2.f * other.getMass() * dotProductNormal2 + dotProductNormal1 * (getMass() - other.getMass())) / (getMass() + other.getMass());
		float m2 = (2.f * getMass() * dotProductNormal1 + dotProductNormal2 * (other.getMass() - getMass())) / (getMass() + other.getMass());
		

		putBall(other); //포켓에 공넣기			
		SampleBilliardBall* Ball = dynamic_cast<SampleBilliardBall*>(&other);
		int Cnt;
		//어떤 플레이어가 넣었는지 확인
		if(Player::WhoisTurn().getPlayerNum()==1) { //Player1
			Cnt = Player::WhoisTurn().getPutBallCnt(); //플레이어가 넣은 공의 개수를 가져옴
			other.setPosition(50 + (2 * Cnt * Ball->getRadius()), 500); //충돌한 공의 위치 지정
			if (typeid(other) != typeid(SampleBilliardGameBall)) //흰공이 아니면
				Player::WhoisTurn().setPutBallCnt(Cnt+1); //넣은 공으로 추가
		}
		else { //Player2
			Cnt = Player::WhoisTurn().getPutBallCnt(); //플레이어가 넣은 공의 개수를 가져옴
			other.setPosition(50 + (2 * Cnt * Ball->getRadius()), 600); //충돌한 공의 위치 지정
			if (typeid(other) != typeid(SampleBilliardGameBall)) //흰공이 아니면
				Player::WhoisTurn().setPutBallCnt(Cnt + 1); //넣은 공으로 추가
		}
		//other.setVelocity(dotProductTangential2 * tangential + m2 * normal);
		other.setVelocity(0, 0);
		setVelocity(0, 0); //포켓 속도0
	}
}

//렌더링 재정의
void BilliardPocket::render(sf::RenderTarget& target) {
	//printBall(target); //테스트코드
	target.draw(getVertices());
}

//포켓에 넣은 공 출력
void BilliardPocket::printBall(sf::RenderTarget& target) {
	for (int i = 0; i < Pocket.size(); ++i) {
		//BilliardGameBall로 동적캐스팅
		SampleBilliardBall& ball = *dynamic_cast<SampleBilliardBall*>(Pocket[i]);
		sf::Text BallName;
		BallName.setFont(SampleGame::getFont());
		BallName.setFillColor(sf::Color::Black);
		BallName.setString(ball.getOwner());
		BallName.setCharacterSize(20);
		BallName.setPosition(450,i*50+300);
		target.draw(BallName);
	}
}

//포켓에 공넣기
void BilliardPocket::putBall(SampleBilliardObject& Ball) {
	Pocket.push_back(&Ball);
}

//포켓에서 공 빼오기
SampleBilliardObject& BilliardPocket::outBall(int index){
	SampleBilliardObject* Ball = Pocket[index];
	//포켓 시작에서 끝 범위에서 해당 값을 가진 오브젝트를 제거
	Pocket.erase(remove(Pocket.begin(), Pocket.end(), Pocket[index]), Pocket.end());
	return *Ball;
}

SampleBilliardBall& BilliardPocket::BallRef(int index) {
	int i=Pocket.size()-1; //마지막요소
	if (0 <= index && index < Pocket.size()) //정상 범위라면
		i=index;
	SampleBilliardBall& Ball = *dynamic_cast<SampleBilliardBall*>(Pocket[i]);
	return Ball;
}


//포켓안에 해당 공이 있는지 검사
int BilliardPocket::InPocket(SampleBilliardObject& other){ 
	int index = -1;
	SampleBilliardBall& oB = *dynamic_cast<SampleBilliardBall*>(&other);
	if (&oB == nullptr) return index;
	for (int i = 0; i < Pocket.size(); ++i) {
		SampleBilliardBall& pB = *dynamic_cast<SampleBilliardBall*>(Pocket[i]);
		if (&pB == &oB) { //포켓안에 있다면
			index = i;
			break;
		}
	}
	return index;
}

//stripes의 개수 반환
int BilliardPocket::getCntStripes() {
	int cnt=0;
	for (int i = 0; i < Pocket.size(); ++i) {
		if (nullptr != dynamic_cast<SampleBilliardBall*>(Pocket[i])) {
			SampleBilliardBall* Ball = dynamic_cast<SampleBilliardBall*>(Pocket[i]);
			if (Ball->getOwner().compare("P")) { //플레이어 볼이 아니라면
				if (stoi(Ball->getOwner()) > 8) //값이 8보다 큰 것의 개수를 셈
					cnt++;
			}
		}
	}
	return cnt;
}

//solids의 개수 반환
int BilliardPocket::getCntSolids() {
	int cnt = 0;
	for (int i = 0; i < Pocket.size(); ++i) {
		if (nullptr != dynamic_cast<SampleBilliardBall*>(Pocket[i])) {
			SampleBilliardBall* Ball = dynamic_cast<SampleBilliardBall*>(Pocket[i]);
			if (Ball->getOwner().compare("P")) { //플레이어 볼이 아니라면
				if (stoi(Ball->getOwner()) < 8) //값이 8보다 작은 것의 개수를 셈
					cnt++;
			}
		}
	}
	return cnt;
}

//포켓 크기 반환
int BilliardPocket::getSizePocket() {
	return Pocket.size();
}