#include "BaseGame.h"
#include "SampleGame.h"
#include "StartGame.h"

int main(void)
{
	//�ʱ� ���� ȭ�� ����
	BaseGame&& start_game = StartGame(500, 500, 60);
	start_game.run();

	return 0;
}
