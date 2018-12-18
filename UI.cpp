#include "Global.h"

int WaponWindowIm;
int ScoreWindowIm;
int LifeWindowIm;
int LifeBarIm;
int LifeBarDeadIm;
int ExpWindowIm;
int ExpLifeIm;
int ExpLifeDeadIm;
int WaponIconIm[64];
int Unable_EquipIm;

//基本的にスコア読み込みの際は配列０番が最大の桁を読み込みます。表示する要素ごとに適した桁数の表示用配列を作成してください。
void UI_ReadAmmoNo(int num,int nummax) {
	Ammo_No[0] = (num / 100);//3桁目
	Ammo_No[1] = (num / 10 - (Ammo_No[0] * 10));//2桁目
	Ammo_No[2]= (num - (Ammo_No[0] * 100 + Ammo_No[1] * 10));//1桁目

	Ammo_NoMax[0] = (nummax / 100);
	Ammo_NoMax[1] = (nummax / 10 - (Ammo_NoMax[0] * 10));
	Ammo_NoMax[2] = (nummax - (Ammo_NoMax[0] * 100 + Ammo_NoMax[1] * 10));
}

static void UI_ReadScore(int num) {
	Score_No[0] = (num / 10000000);
	Score_No[1] = (num / 1000000 - (Score_No[0] * 10));
	Score_No[2] = (num / 100000 - (Score_No[0] * 100 + Score_No[1] * 10));
	Score_No[3] = (num / 10000 - (Score_No[0] * 1000 + Score_No[1] * 100 + Score_No[2] * 10));
	Score_No[4] = (num / 1000 - (Score_No[0] * 10000 + Score_No[1] * 1000 + Score_No[2] * 100 + Score_No[3] * 10));
	Score_No[5] = (num / 100 - (Score_No[0] * 100000 + Score_No[1] * 10000 + Score_No[2] * 1000 + Score_No[3] * 100 + Score_No[4] * 10));
	Score_No[6] = (num / 10 - (Score_No[0] * 1000000 + Score_No[1] * 100000 + Score_No[2] * 10000 + Score_No[3] * 1000 + Score_No[4] * 100 + Score_No[5] * 10));
	Score_No[7] = (num - (Score_No[0] * 10000000 + Score_No[1] * 1000000 + Score_No[2] * 100000 + Score_No[3] * 10000 + Score_No[4] * 1000 + Score_No[5] * 100 + Score_No[6] * 10));
}


float LifeRate;

void UI_LifeSetter(int life,int lifemax) {
	LifeRate = (float)life / lifemax;
	if (LifeRate > 1.0f)LifeRate = 1.0f;
	if (LifeRate < 0.0f)LifeRate = 0.0f;
}

void UI_Init()
{
	Score = 0;
	LifeRate = 0;
}

void UI_Release()
{

}

void UI_Update()
{
	if (Score > 99999999) {
		Score =  99999999;
	}
	UI_ReadScore(Score);
}

#define NUMBER_X 14
#define AMMO_NO_POS_X 98
#define AMMO_NO_POS_Y 54
#define AMMO_NOMAX_POS_X 154
#define SCOREPOS_X 448
#define SCOREPOS_Y 54

#define LIFEBAR_X 242
#define LIFEBAR_Y 34
#define LIFEBAR_WIDTH 156
#define LIFEBAR_HEIGHT 20

void UI_Rend()
{
	//----左サイドウィンドウ------------
	DrawGraph(0, 0, WaponWindowIm, TRUE);
	//今回経験値の表示は無し
	//DrawGraph(86, 74, ExpLifeIm, TRUE);
	//DrawGraph(86, 74, ExpLifeDeadIm, TRUE);
	//DrawTurnGraph(82, 72, ExpWindowIm, TRUE);//こっちのEXPバーを反転表示させてます
	DrawGraph(36, 27, WaponIconIm[Wapon.Wapon_kind], TRUE);
	//----左サイドウィンドウ・終了------

	//----右サイドウィンドウ------------
	DrawGraph(434, 0, ScoreWindowIm, TRUE);
	//DrawGraph(447, 74, ExpLifeIm, TRUE);
	//DrawGraph(447, 74, ExpLifeDeadIm, TRUE);
	//DrawGraph(443, 72, ExpWindowIm, TRUE);//こっちはそのまま
	DrawGraph(570, 27, WaponIconIm[63], TRUE);
	//----右サイドウィンドウ・終了------

	//----禁止パネル--------------------
	if (P.QC_Enabled[0] == false)DrawGraph(26,  76, Unable_EquipIm, TRUE);
	if (P.QC_Enabled[1] == false)DrawGraph(26, 126, Unable_EquipIm, TRUE);
	if (P.QC_Enabled[2] == false)DrawGraph(26, 176, Unable_EquipIm, TRUE);
	//----禁止パネル・終了--------------

	//----体力ゲージ--------------------
	DrawGraph(LIFEBAR_X, LIFEBAR_Y, LifeBarIm, TRUE);
	DrawModiGraph(LIFEBAR_X + (LIFEBAR_WIDTH * LifeRate), LIFEBAR_Y,                 //左上
				  LIFEBAR_X + LIFEBAR_WIDTH, LIFEBAR_Y, //右上
				  LIFEBAR_X + LIFEBAR_WIDTH, LIFEBAR_Y + LIFEBAR_HEIGHT,   //右下
				  LIFEBAR_X + (LIFEBAR_WIDTH * LifeRate), LIFEBAR_Y + LIFEBAR_HEIGHT,//左下
				  LifeBarDeadIm, TRUE);
	DrawGraph(LIFEBAR_X - 12, LIFEBAR_Y - 34, LifeWindowIm, TRUE);
	//----体力ゲージ・終了--------------

	DrawRotaGraph(AMMO_NO_POS_X                 , AMMO_NO_POS_Y, 0.6, 0,Number[Ammo_No[0]], TRUE);
	DrawRotaGraph(AMMO_NO_POS_X +  NUMBER_X     , AMMO_NO_POS_Y, 0.6, 0, Number[Ammo_No[1]], TRUE);
	DrawRotaGraph(AMMO_NO_POS_X + (NUMBER_X * 2), AMMO_NO_POS_Y, 0.6, 0, Number[Ammo_No[2]], TRUE);

	DrawRotaGraph(AMMO_NOMAX_POS_X                 , AMMO_NO_POS_Y, 0.6, 0, Number[Ammo_NoMax[0]], TRUE);
	DrawRotaGraph(AMMO_NOMAX_POS_X +  NUMBER_X     , AMMO_NO_POS_Y, 0.6, 0, Number[Ammo_NoMax[1]], TRUE);
	DrawRotaGraph(AMMO_NOMAX_POS_X + (NUMBER_X * 2), AMMO_NO_POS_Y, 0.6, 0, Number[Ammo_NoMax[2]], TRUE);

	DrawRotaGraph(SCOREPOS_X                 , SCOREPOS_Y, 0.6, 0, Number[Score_No[0]], TRUE);
	DrawRotaGraph(SCOREPOS_X +  NUMBER_X     , SCOREPOS_Y, 0.6, 0, Number[Score_No[1]], TRUE);
	DrawRotaGraph(SCOREPOS_X + (NUMBER_X * 2), SCOREPOS_Y, 0.6, 0, Number[Score_No[2]], TRUE);
	DrawRotaGraph(SCOREPOS_X + (NUMBER_X * 3), SCOREPOS_Y, 0.6, 0, Number[Score_No[3]], TRUE);
	DrawRotaGraph(SCOREPOS_X + (NUMBER_X * 4), SCOREPOS_Y, 0.6, 0, Number[Score_No[4]], TRUE);
	DrawRotaGraph(SCOREPOS_X + (NUMBER_X * 5), SCOREPOS_Y, 0.6, 0, Number[Score_No[5]], TRUE);
	DrawRotaGraph(SCOREPOS_X + (NUMBER_X * 6), SCOREPOS_Y, 0.6, 0, Number[Score_No[6]], TRUE);
	DrawRotaGraph(SCOREPOS_X + (NUMBER_X * 7), SCOREPOS_Y, 0.6, 0, Number[Score_No[7]], TRUE);
}
