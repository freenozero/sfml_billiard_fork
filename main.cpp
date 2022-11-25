#include "BaseGame.h"
#include "SampleGame.h"
#include "StartGame.h"

int main(void)
{
	while(!StartGame::isClose()){
		StartGame Game(1028, 960, 60);
		Game.Info();
	}
	return 0;
}
