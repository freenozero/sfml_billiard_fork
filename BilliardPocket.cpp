#include "BilliardPocket.h"
#include "SampleGame.h"

//��������
vector<SampleBilliardObject*> BilliardPocket::Pocket;

//�Ҹ���.
BilliardPocket::~BilliardPocket() {
	//���� Ŭ����
	initPocket();
}

void BilliardPocket::initPocket() {
	Pocket.clear();
}

//������Ʈ �浹 ������
void BilliardPocket::collide(SampleBilliardObject& other)
{
	// ���� �浹�� ��� 
	if (dynamic_cast<SampleBilliardBall*>(&other) != nullptr)
	{
		SampleBilliardBall& ball = *dynamic_cast<SampleBilliardBall*>(&other);
		collideWithBall(ball);
	}
	//�籸�� �浹�� ��� ��.
}

//���ϰ� �� �浹
void BilliardPocket::collideWithBall(SampleBilliardBall& other)
{
	// ������ �� �񱳽� ���� 
	if (this == &other)
	{
		return;
	}
	// �Ÿ� ��� 
	sf::Vector2f distance = getPosition() - other.getPosition();
	float distanceBetween = (sqrtf((distance.x * distance.x) + (distance.y * distance.y)));

	// �� ���� ��ġ���� �˻� 
	//������ �ٷ� �־����� �ʰ� Ư�� ���� �Ѿ�� ���Ͽ� �ִ� ������ ����
	if (distanceBetween < getRadius()/1.5)  
	{
		// ��ġ�� ���� ��� 
		float overlap = (distanceBetween - getRadius() - other.getRadius()) / 2.f;
		float moveX = (overlap * (getPosition().x - other.getPosition().x) / distanceBetween);
		float moveY = (overlap * (getPosition().y - other.getPosition().y) / distanceBetween);

		// �浹 �� �ӵ� ����Ͽ� ����
		sf::Vector2f normal(distance.x / distanceBetween, distance.y / distanceBetween);
		sf::Vector2f tangential(-normal.y, normal.x);

		float dotProductTangential1 = getVelocity().x * tangential.x + getVelocity().y * tangential.y;
		float dotProductTangential2 = other.getVelocity().x * tangential.x + other.getVelocity().y * tangential.y;

		float dotProductNormal1 = getVelocity().x * normal.x + getVelocity().y * normal.y;
		float dotProductNormal2 = other.getVelocity().x * normal.x + other.getVelocity().y * normal.y;

		float m1 = (2.f * other.getMass() * dotProductNormal2 + dotProductNormal1 * (getMass() - other.getMass())) / (getMass() + other.getMass());
		float m2 = (2.f * getMass() * dotProductNormal1 + dotProductNormal2 * (other.getMass() - getMass())) / (getMass() + other.getMass());
		

		putBall(other); //���Ͽ� ���ֱ�			
		SampleBilliardBall* Ball = dynamic_cast<SampleBilliardBall*>(&other);
		int Cnt;
		//� �÷��̾ �־����� Ȯ��
		if(Player::WhoisTurn().getPlayerNum()==1) { //Player1
			Cnt = Player::WhoisTurn().getPutBallCnt(); //�÷��̾ ���� ���� ������ ������
			other.setPosition(50 + (2 * Cnt * Ball->getRadius()), 500); //�浹�� ���� ��ġ ����
			if (typeid(other) != typeid(SampleBilliardGameBall)) //����� �ƴϸ�
				Player::WhoisTurn().setPutBallCnt(Cnt+1); //���� ������ �߰�
		}
		else { //Player2
			Cnt = Player::WhoisTurn().getPutBallCnt(); //�÷��̾ ���� ���� ������ ������
			other.setPosition(50 + (2 * Cnt * Ball->getRadius()), 600); //�浹�� ���� ��ġ ����
			if (typeid(other) != typeid(SampleBilliardGameBall)) //����� �ƴϸ�
				Player::WhoisTurn().setPutBallCnt(Cnt + 1); //���� ������ �߰�
		}
		//other.setVelocity(dotProductTangential2 * tangential + m2 * normal);
		other.setVelocity(0, 0);
		setVelocity(0, 0); //���� �ӵ�0
	}
}

//������ ������
void BilliardPocket::render(sf::RenderTarget& target) {
	//printBall(target); //�׽�Ʈ�ڵ�
	target.draw(getVertices());
}

//���Ͽ� ���� �� ���
void BilliardPocket::printBall(sf::RenderTarget& target) {
	for (int i = 0; i < Pocket.size(); ++i) {
		//BilliardGameBall�� ����ĳ����
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

//���Ͽ� ���ֱ�
void BilliardPocket::putBall(SampleBilliardObject& Ball) {
	Pocket.push_back(&Ball);
}

//���Ͽ��� �� ������
SampleBilliardObject& BilliardPocket::outBall(int index){
	SampleBilliardObject* Ball = Pocket[index];
	//���� ���ۿ��� �� �������� �ش� ���� ���� ������Ʈ�� ����
	Pocket.erase(remove(Pocket.begin(), Pocket.end(), Pocket[index]), Pocket.end());
	return *Ball;
}

SampleBilliardBall& BilliardPocket::BallRef(int index) {
	int i=Pocket.size()-1; //���������
	if (0 <= index && index < Pocket.size()) //���� �������
		i=index;
	SampleBilliardBall& Ball = *dynamic_cast<SampleBilliardBall*>(Pocket[i]);
	return Ball;
}


//���Ͼȿ� �ش� ���� �ִ��� �˻�
int BilliardPocket::InPocket(SampleBilliardObject& other){ 
	int index = -1;
	SampleBilliardBall& oB = *dynamic_cast<SampleBilliardBall*>(&other);
	if (&oB == nullptr) return index;
	for (int i = 0; i < Pocket.size(); ++i) {
		SampleBilliardBall& pB = *dynamic_cast<SampleBilliardBall*>(Pocket[i]);
		if (&pB == &oB) { //���Ͼȿ� �ִٸ�
			index = i;
			break;
		}
	}
	return index;
}

//stripes�� ���� ��ȯ
int BilliardPocket::getCntStripes() {
	int cnt=0;
	for (int i = 0; i < Pocket.size(); ++i) {
		if (nullptr != dynamic_cast<SampleBilliardBall*>(Pocket[i])) {
			SampleBilliardBall* Ball = dynamic_cast<SampleBilliardBall*>(Pocket[i]);
			if (Ball->getOwner().compare("P")) { //�÷��̾� ���� �ƴ϶��
				if (stoi(Ball->getOwner()) > 8) //���� 8���� ū ���� ������ ��
					cnt++;
			}
		}
	}
	return cnt;
}

//solids�� ���� ��ȯ
int BilliardPocket::getCntSolids() {
	int cnt = 0;
	for (int i = 0; i < Pocket.size(); ++i) {
		if (nullptr != dynamic_cast<SampleBilliardBall*>(Pocket[i])) {
			SampleBilliardBall* Ball = dynamic_cast<SampleBilliardBall*>(Pocket[i]);
			if (Ball->getOwner().compare("P")) { //�÷��̾� ���� �ƴ϶��
				if (stoi(Ball->getOwner()) < 8) //���� 8���� ���� ���� ������ ��
					cnt++;
			}
		}
	}
	return cnt;
}

//���� ũ�� ��ȯ
int BilliardPocket::getSizePocket() {
	return Pocket.size();
}