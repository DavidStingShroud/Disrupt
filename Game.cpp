#include "Global.h"

float deltatime = 1.0f / 60;

int Number[10], Ammo_No[3], Ammo_NoMax[3];
int Score, Score_No[8];

void Game_Load() {
	//ファイル名、画像総数、画像数X、画像数Y、画像サイズX、画像サイズY、格納する場所
	LoadDivGraph("Materials/Bullet32x32.png", 4, 4, 1, BLOCK_SIZE, BLOCK_SIZE, bulgra);
	LoadDivGraph("Materials/P32x64.png", 12, 4, 3, BLOCK_SIZE, BLOCK_SIZE * 2, P.gra);
	LoadDivGraph("Materials/Chipset16x16.bmp", 64, 8, 8, BLOCK_SIZE, BLOCK_SIZE, tile.gra);
	WaponWindowIm = LoadGraph("Materials/Window_Wapon.png");
	ScoreWindowIm = LoadGraph("Materials/Window_Score.png");
	LifeWindowIm = LoadGraph("Materials/Window_Life.png");
	LifeBarIm = LoadGraph("Materials/Window_Life_Bar.png");
	LifeBarDeadIm = LoadGraph("Materials/Window_Life_Bar_dead.png");
	ExpWindowIm = LoadGraph("Materials/Window_EXP.png");
	ExpLifeIm = LoadGraph("Materials/Window_EXP_Bar.png");//EXPバーの増減部分は武器・プレイヤー共に共通のものを使用します。
	ExpLifeDeadIm = LoadGraph("Materials/Window_EXP_Bar_dead.png");
	Unable_EquipIm = LoadGraph("Materials/Window_UnableEquip.png");
	LoadDivGraph("Materials/WaponIcon32x32.png", 64, 8, 8, 32, 32, WaponIconIm);
	LoadDivGraph("Materials/Wapon_64x64.png", 24, 3, 8, BLOCK_SIZE * 2, BLOCK_SIZE * 2, Wapon.gra);
	LoadDivGraph("Materials/Flash34x34.png", 10, 5, 2, 34, 34, Wapon.Fire_gra);
	LoadDivGraph("Materials/Number18x27.png", 10, 10, 1, 18, 27, Number);
	LoadDivGraph("Materials/Creep24x64.png", 6, 6, 1, 24, 64, enemy01->gra);//敵
	tile.StageBG[0] = LoadGraph("Materials/StageBack01.png");
}

void Game_Init()
{
	Player_Init();
	Wapon_Init();
	Bullet_Init();
	Enemy_Init();
	Stage_Init();
	UI_Init();
}

void Game_Release()
{
	/*
	Player_Release();
	Wapon_Release();
	Bullet_Release();
	Enemy_Release();
	Stage_Release();
	UI_Release();
	*/
}

void Game_Update()
{
	Player_Update();
	Wapon_Update();
	Bullet_Update();
	Enemy_Update();
	Stage_Update();
	UI_Update();
	Bulletgetter();
}

void Game_Rend()
{
	Stage_RendBG();
	Player_Rend();
	Wapon_Rend();
	Bullet_Rend();
	Enemy_Render();
	Stage_Rend();
	UI_Rend();
}
