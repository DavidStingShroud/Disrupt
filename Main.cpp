#include "Global.h"

struct TILE tile;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetMainWindowText("DELENTLESS  the First Destruction");
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);
	
	Game_Load();
	Game_Init();
	srand(time(NULL));

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && gpUpdateKey() == 0) {
		Game_Update();

		Game_Rend();
	}

	DxLib_End();

	return 0;
}