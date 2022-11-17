#include "BaseGame.h"
#include "SampleGame.h"
#include "StartGame.h"

int main(void)
{
	//초기 게임 화면 생성
	BaseGame&& start_game = StartGame(500, 500, 60);
	start_game.run();

	return 0;
}
