#include "Global.h"

struct WAPON Wapon;

void Wapon_Init()
{
	Wapon.W_type = 0 * 3;//0=pistol, 3=human, 6=Rocket, 9=Shotgun, 12=sniper, 15=D_Shotgun, 18=Minigun, 21=PYLO.
	Wapon.sprash = 6;//6が割と強い。ダブルバレルタイプは4とかなり拡散するものの、弾数が多いため近接でのDPSではノーマルタイプを大きく上回る。
	for (int i = 0; i < ALL_BULLET_MAX; i++) {Wapon.fireAni[i] = 0;}
	for (int i = 0; i < WAPON_MAX; i++) { Wapon.have[i] = false; }
	Wapon.have[0] = true;
	Wapon.have[3] = true;
	Wapon.have[4] = true;
	Wapon.have[6] = true;
}

void Wapon_Release()
{

}

void Wapon_Update()
{
	if (P.turn == 0) {
		Wapon.x = P.x - 16.0f;
	}
	else {
		Wapon.x = P.x - 48.0f;
	}
	Wapon.y = P.y - 64.0f;

	if (Wapon.isFired == true && (Wapon.W_type >= 18 && Wapon.W_type <= 20)) {
		if (P.turn == 0) {
			Wapon.x = RandAtoZ((P.x - (16.0f)) - 2, (P.x - (16.0f)) + 2);
		}
		else {
			Wapon.x = RandAtoZ((P.x - (48.0f)) - 2, (P.x - (48.0f)) + 2);
		}
		Wapon.y = RandAtoZ((P.y - (64.0f)) - 2, (P.y - (64.0f)) + 2);
	}

	if ((Wapon.W_type >= 0 && Wapon.W_type <= 2))Wapon.Wapon_kind = WAPON_PISTOL;
	if ((Wapon.W_type >= 3 && Wapon.W_type <= 5))Wapon.Wapon_kind = WAPON_HUMAN;
	if ((Wapon.W_type >= 6 && Wapon.W_type <= 8))Wapon.Wapon_kind = WAPON_ROCKET;
	if ((Wapon.W_type >= 9 && Wapon.W_type <= 11))Wapon.Wapon_kind = WAPON_SHOTGUN;
	if ((Wapon.W_type >= 12 && Wapon.W_type <= 14))Wapon.Wapon_kind = WAPON_SNIPER;
	if ((Wapon.W_type >= 15 && Wapon.W_type <= 17))Wapon.Wapon_kind = WAPON_DOUBLES;
	if ((Wapon.W_type >= 18 && Wapon.W_type <= 20))Wapon.Wapon_kind = WAPON_MINIGUN;
	if ((Wapon.W_type >= 21 && Wapon.W_type <= 23))Wapon.Wapon_kind = WAPON_PYLO;
}

void Wapon_Rend()
{
	if (P.turn == 0) {//右

		DrawGraphF(Wapon.x, Wapon.y, Wapon.gra[Wapon.W_type], TRUE);

		for (int i = 0; i < ALL_BULLET_MAX; i++) {
			if (Wapon.isFlash[i]) {
				if ((Wapon.W_type >= 18 && Wapon.W_type <= 20)) {
					DrawGraphF(P.x + 12.0f, P.y - 28.0f, Wapon.Fire_gra[Wapon.fireAni[i]], TRUE);
				}
				else {
					DrawGraphF(P.x + 12.0f, P.y - 46.0f, Wapon.Fire_gra[Wapon.fireAni[i]], TRUE);
				}
			}
		}
	}
	if (P.turn == 1) {//左


		DrawTurnGraphF(Wapon.x, Wapon.y, Wapon.gra[Wapon.W_type], TRUE);

		for (int i = 0; i < ALL_BULLET_MAX; i++) {
			if (Wapon.isFlash[i]) {
				if ((Wapon.W_type >= 18 && Wapon.W_type <= 20)) {
					DrawGraphF(P.x - 46.0f, P.y - 28.0f, Wapon.Fire_gra[Wapon.fireAni[i]], TRUE);
				}
				else {
					DrawGraphF(P.x - 46.0f, P.y - 46.0f, Wapon.Fire_gra[Wapon.fireAni[i]], TRUE);
				}
			}
		}
	}
}
