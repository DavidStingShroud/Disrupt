#include "Global.h"
#include <math.h>

struct PLAYER P;

float PlayerX1;
float PlayerY1;
float PlayerX2;
float PlayerY2;
float GetPlayerPosX() { return P.x; }
float GetPlayerPosY() { return P.y; }

static void PLAYER_BLOCKCHECKX() {
	//	右
	float headY = P.y - 4.0f;//2.0f
	float rightX = P.x + 12.0f;//6.0f
	float bodyY = P.y - 36.0f;//18.0f
	float footY = P.y - 4.0f;//2.0f

	bool hit_head = STAGE_HitCheck(rightX, headY);
	bool hit_body = STAGE_HitCheck(rightX, bodyY);
	bool hit_foot = STAGE_HitCheck(rightX, footY);
	if (hit_head || hit_body || hit_foot) {
		//	キャラをブロックの左に
		P.x = STAGE_GetBlockX(rightX) - 14.0f;//7.0f
	}
	//	左
	float leftX = P.x - 12.0f;//12.0f
	bool hit_headL = STAGE_HitCheck(leftX, headY);
	bool hit_bodyL = STAGE_HitCheck(leftX, bodyY);
	bool hit_footL = STAGE_HitCheck(leftX, footY);
	if (hit_headL || hit_bodyL || hit_footL) {
		//	キャラをブロックの右に
		P.x = STAGE_GetBlockX(leftX) + BLOCK_SIZE + 12.0f;//6.0f
	}
}
static void PLAYER_BLOCKCHECKY() {

	float left = P.x - 12.0f;//6.0f
	float right = P.x + 12.0f;
	//頭打つ（上昇中のみ）
	if (P.vel_y < 0) {
		bool Hitleft = STAGE_HitCheck(left, P.y - BLOCK_SIZE);
		bool Hitright = STAGE_HitCheck(right, P.y - BLOCK_SIZE);
		if (Hitleft || Hitright) {

			P.vel_y = 0;
			P.y = (float)STAGE_GetBlockY(P.y - BLOCK_SIZE);
			//P.y += 24.0f;
			P.y += 32.0f;//16

		}
	}

	if (P.vel_y >= 0) {
		bool Hitleft = STAGE_HitCheck(left, P.y);
		bool Hitright = STAGE_HitCheck(right, P.y);

		if (Hitleft || Hitright) {
			P.vel_y = 0;

			P.y = (float)STAGE_GetBlockY(P.y);

			P.isGrounded = true;

			P.vel_x -= P.vel_x * 0.1f;

			if (fabsf(P.vel_x) < 10) {
				P.vel_x = 0;
			}
		}
	}
}

static void Move(float time) {

	//----デバッグ用違法クイックチェンジ--------
#ifdef DEBUG
	if (Key.input[KEY_INPUT_LEFT] == 1) {

		Wapon.W_type += 3;
		//enumを使いたければ　STATE(ここに演算式)　のように書くこと。

		if (Wapon.W_type > 20) {
			Wapon.W_type = 0;
		}
	}
#endif
	//----デバッグ用違法クイックチェンジ・終了--
	if (Key.input[KEY_INPUT_1] == 1 && Wapon.have[WAPON_PISTOL]) {
		Wapon.W_type = 0;//ピストル
	}
	if (Key.input[KEY_INPUT_2] == 1 && Wapon.have[WAPON_SHOTGUN]) {
		Wapon.W_type = 9;//ショットガン
	}
	if (Key.input[KEY_INPUT_3] == 1 && Wapon.have[WAPON_SNIPER]) {
		Wapon.W_type = 12;//スナイパーライフル
	}
	if (Key.input[KEY_INPUT_4] == 1 && Wapon.have[WAPON_MINIGUN]) {
		Wapon.W_type = 18;//ミニガン
	}


	//----人外ジャンプ--------------------------
	if (P.isGrounded == true) {
		if (Key.input[KEY_INPUT_SPACE] != 0) {
			if ((Wapon.Wapon_kind == WAPON_ROCKET)
				|| (Wapon.Wapon_kind == WAPON_MINIGUN)) {
				P.vel_y = -380;//-260
			}
			else {
				P.vel_y = -580;//-460
			}
			P.state = STATE_JUMP;
			return;
		}
	}
	//----人外ジャンプ・終了--------------------


	//----歩行・アニメ、重火器の移動制限--------
	P.AnimNo %= 4;
	P.Anicount++;

	if (Key.input[KEY_INPUT_A] > 0) {
		if ((Wapon.Wapon_kind == WAPON_ROCKET)
			|| (Wapon.Wapon_kind == WAPON_MINIGUN)) {
			P.vel_x += -32;//-16
		}
		else {
			P.vel_x += -50;//-25
		}
		P.turn = 1;


		if (P.Anicount > 8) {
			P.AnimNo++;
			P.Anicount = 0;
		}
		if (P.AnimNo > 3) {
			P.AnimNo = 0;
		}
	}
	if (Key.input[KEY_INPUT_D] > 0) {
		if ((Wapon.Wapon_kind == WAPON_ROCKET)
			|| (Wapon.Wapon_kind == WAPON_MINIGUN)) {
			P.vel_x += 32;//16
		}
		else {
			P.vel_x += 50;//25
		}
		P.turn = 0;

		if (P.Anicount > 8) {
			P.AnimNo++;
			P.Anicount = 0;
		}
		if (P.AnimNo > 3) {
			P.AnimNo = 0;
		}

	}
	//----歩行・アニメ、重火器の移動制限・終了--


	//----移動を監視----------------------------
	if (Key.input[KEY_INPUT_A] <= 0 && Key.input[KEY_INPUT_D] <= 0) {
		P.AnimNo = 0;
		P.Anicount = 0;
	}

	//if (Key.input[KEY_INPUT_W] >= 1) {
	//	P.AnimNo += 4;
	//	P.lookup = 1;
	//}
	//if (Key.input[KEY_INPUT_W] == 0 && P.lookup >= true) {
	//	P.AnimNo = 0;
	//	P.Anicount = 0;
	//	Wapon.W_type -= 1;
	//	P.lookup = 0;
	//}

	//if (Key.input[KEY_INPUT_S] >= 1) {
	//	P.AnimNo += 8;
	//	P.lookdown = 1;
	//}

	//if (Key.input[KEY_INPUT_S] == 0 && P.lookdown >= true) {
	//	P.AnimNo = 0;
	//	P.Anicount = 0;
	//	Wapon.W_type -= 2;
	//	P.lookdown = 0;
	//}
	//----移動を監視・終了----------------------


	//----上下構え------------------------------
	//if (Key.input[KEY_INPUT_W] == 1) {
	//	Wapon.W_type += 1;
	//}
	//if (Key.input[KEY_INPUT_S] == 1) {
	//	Wapon.W_type += 2;
	//}
	//----上下構え・終了------------------------


	//----各種バレット起動フェイズ--------------

	for (int i = 0; i < ALL_BULLET_MAX; i++) {
		if (Key.input[KEY_INPUT_RCONTROL] == 1 && Wapon.Wapon_kind != WAPON_MINIGUN && Ammo.NowAmmo != 0) {
			Wapon.isFlash[i] = true;
		}
		else if (Key.input[KEY_INPUT_RCONTROL] >= 1 && Wapon.Wapon_kind == WAPON_MINIGUN && Ammo.NowAmmo != 0) {
			Wapon.isFlash[i] = true;
		}
		if (Key.input[KEY_INPUT_RCONTROL] == 1 && !(Wapon.Wapon_kind == WAPON_MINIGUN)
			&& !(Wapon.Wapon_kind == WAPON_SHOTGUN) && !(Wapon.Wapon_kind == WAPON_DOUBLES)
			&& bul[i].isAlive==false) {
			if (Wapon.Wapon_kind == WAPON_PISTOL && Ammo.Bullet>0) {
				Ammo.Bullet--;
				NozzleputterDefault(i);
				break;
			}
			else if (Wapon.Wapon_kind == WAPON_HUMAN && Ammo.Oxigen>0) {
				Ammo.Oxigen--;
				NozzleputterDefault(i);
				break;
			}
			else if (Wapon.Wapon_kind == WAPON_ROCKET && Ammo.Rocket>0) {
				Ammo.Rocket--;
				NozzleputterDefault(i);
				break;
			}
			else if (Wapon.Wapon_kind == WAPON_SNIPER && Ammo.Pow_Bullet>0) {
				Ammo.Pow_Bullet--;
				NozzleputterDefault(i);
				break;
			}
		}

	}


	if (((Wapon.Wapon_kind == WAPON_SHOTGUN) || (Wapon.Wapon_kind == WAPON_DOUBLES))
		&& Key.input[KEY_INPUT_RCONTROL] == 1 && (Ammo.Shell>0)) {
		Ammo.Shell--;
	}
	for (int i = 0; i < SHOTGUN_BULMAX; i++) {

	if ((Wapon.Wapon_kind == WAPON_SHOTGUN)
		&& Key.input[KEY_INPUT_RCONTROL] == 1
		&& Ammo.Shell>0) {

		for (int j = 0; j < SHOTGUN_BULMAX; j++) {
			ShotGunSetter(j);
		}
		if (bul[i].isAlive == false) {
			NozzleputterDefault(i);
			}	
		}
	}
	for (int i = 0; i < DOUBLES_BULMAX; i++) {

		if ((Wapon.Wapon_kind == WAPON_DOUBLES)
			&& Key.input[KEY_INPUT_RCONTROL] == 1
			&& (Ammo.Shell>0)) {
			for (int j = 0; j < DOUBLES_BULMAX; j++) {
				ShotGunSetter(j);
			}
			if (bul[i].isAlive == false) {
				NozzleputterDefault(i);
			}
		}
	}

	for (int i = 0; i < MINIGUN_BULMAX; i++) {
		if ((Wapon.Wapon_kind == WAPON_MINIGUN)
			&& Key.input[KEY_INPUT_RCONTROL] >= 1
			&& (Ammo.Bullet>0)) {
			Shotdelay %= 3;
			Shotdelaycount++;
			if (Shotdelaycount > 2) {
				Shotdelay++;
				Shotdelaycount = 0;
			}
			if ((Minigun.ShotF[i] == false)&&(Shotdelay > 2)) {
				Ammo.Bullet--;
				Nozzleputter(i);
				ChainBulletSetter(i);
				Shotdelay = 0;
				break;
			}
		}
	}
	if (Key.input[KEY_INPUT_RCONTROL] >= 1) {
		Wapon.isFired = true;
	}
	else {
		Wapon.isFired = false;
	}
	//----各種バレット起動フェイズ・終了--------


	//----各種バレットを更新--------------------
	for (int i = 0; i < ALL_BULLET_MAX; i++) {
		if (bul[i].isAlive == true && !(Wapon.Wapon_kind == WAPON_SHOTGUN)) {
			if(Wapon.isFlash[i])Wapon.firecount[i]++;
			Fire(i);
		}
	}

	for (int i = 0; i <= MINIGUN_BULMAX; i++) {
		if (Minigun.ShotF[i] == true && (Wapon.Wapon_kind == WAPON_MINIGUN)) {
			if (Wapon.isFlash[i])Wapon.firecount[i]++;
			ChainBulletUpdater(i);
		}
	}

	for (int i = 0; i < SHOTGUN_BULMAX; i++) {
		if (bul[i].isAlive == true && (Wapon.Wapon_kind == WAPON_SHOTGUN)) {
			if (Wapon.isFlash[i])Wapon.firecount[i]++;
			ShotGunUpdater(i);
		}
	}

	for (int i = 0; i < DOUBLES_BULMAX; i++) {
		if (bul[i].isAlive == true && (Wapon.Wapon_kind == WAPON_DOUBLES)) {
			if (Wapon.isFlash[i])Wapon.firecount[i]++;
			ShotGunUpdater(i);
		}
	}
	//----各種バレットを更新・終了--------------


	//----マズルフラッシュアニメ----------------
	for (int i = 0; i < ALL_BULLET_MAX; i++) {
		if (Wapon.firecount[i] > 4) {
			Wapon.fireAni[i]++;
			Wapon.firecount[i] = 0;
		}
		if (Wapon.fireAni[i] > 4) {
			Wapon.isFlash[i] = false;
			Wapon.fireAni[i] = 0;
			Wapon.firecount[i] = 0;
		}

		Wapon.fireAni[i] %= 5;
	}

	//----マズルフラッシュアニメ・終了----------


	//----ダメージ判定-----------------------
	if (P.isDamaged) {
		P.Damagecount++;
		if (P.Damagecount > 60) { 
			P.Damagecount = 0;  P.state = STATE_NORMAL;
			P.isDamaged = false;
		}
	}
	//----ダメージ判定・終了-----------------


	//----加速度適用、ブロック、接地判定--------
	P.vel_y += 9.8f * (100) * time;
	P.vel_x -= P.vel_x * 0.04f;

	P.x += P.vel_x * time;
	PLAYER_BLOCKCHECKX();
	P.y += P.vel_y * time;

	P.isGrounded = false;
	PLAYER_BLOCKCHECKY();
	if (P.isGrounded == false) {
		P.state = STATE_JUMP;
	}
	//----加速度適用、ブロック、接地判定・終了--

}

static void Jump(float time) {
	if (Key.input[KEY_INPUT_1] == 1 && Wapon.have[WAPON_PISTOL]) {
		Wapon.W_type = 0;//ピストル
	}
	if (Key.input[KEY_INPUT_2] == 1 && Wapon.have[WAPON_SHOTGUN]) {
		Wapon.W_type = 9;//ショットガン
	}
	if (Key.input[KEY_INPUT_3] == 1 && Wapon.have[WAPON_SNIPER]) {
		Wapon.W_type = 12;//スナイパーライフル
	}
	if (Key.input[KEY_INPUT_4] == 1 && Wapon.have[WAPON_MINIGUN]) {
		Wapon.W_type = 18;//ミニガン
	}


	//----空中移動------------------------------
	if (Key.input[KEY_INPUT_A] > 0) {
		P.vel_x += -10;//-5
		P.turn = 1;
	}
	if (Key.input[KEY_INPUT_D] > 0)
	{
		P.vel_x += 10;//5
		P.turn = 0;
	}
	//----空中移動・終了------------------------


	//----加速度適用、ブロック、接地判定-------

		P.vel_x -= P.vel_x * 0.02f;
		P.vel_y += 9.8f * (100) * time;
		P.x += P.vel_x * time;

	PLAYER_BLOCKCHECKX();


		P.y += P.vel_y * time + 0.89f;

	P.isGrounded = false;

	PLAYER_BLOCKCHECKY();
	//----加速度適用、ブロック、接地判定・終了--


	//----上下構え------------------------------
	//if (Key.input[KEY_INPUT_W] == 1) {
	//	P.lookup = 1;
	//	Wapon.W_type++;
	//	P.AnimNo += 4;
	//}
	//if (Key.input[KEY_INPUT_W] == 0 && P.lookup >= true) {
	//	P.AnimNo = 0;
	//	P.Anicount = 0;
	//	P.lookup = 0;
	//	Wapon.W_type--;
	//}

	//if (Key.input[KEY_INPUT_S] == 1) {
	//	P.lookdown = 1;
	//	Wapon.W_type += 2;
	//	P.AnimNo += 8;
	//}
	//if (Key.input[KEY_INPUT_S] == 0 && P.lookdown >= true) {
	//	P.AnimNo = 0;
	//	P.Anicount = 0;
	//	P.lookdown = 0;
	//	Wapon.W_type -= 2;
	//}
	//----上下構え・終了------------------------


	//----各種バレット起動フェイズ--------------

	for (int i = 0; i < ALL_BULLET_MAX; i++) {
		if (Key.input[KEY_INPUT_RCONTROL] == 1 && Wapon.Wapon_kind != WAPON_MINIGUN && Ammo.NowAmmo != 0) {
			Wapon.isFlash[i] = true;
		}
		else if (Key.input[KEY_INPUT_RCONTROL] >= 1 && Wapon.Wapon_kind == WAPON_MINIGUN && Ammo.NowAmmo != 0) {
			Wapon.isFlash[i] = true;
		}

		if (Key.input[KEY_INPUT_RCONTROL] == 1 && !(Wapon.Wapon_kind == WAPON_MINIGUN)
			&& !(Wapon.Wapon_kind == WAPON_SHOTGUN) && !(Wapon.Wapon_kind == WAPON_DOUBLES)
			&& bul[i].isAlive == false) {
			if (Wapon.Wapon_kind == WAPON_PISTOL && Ammo.Bullet>0) {
				Ammo.Bullet--;
				NozzleputterDefault(i);
				break;
			}
			else if (Wapon.Wapon_kind == WAPON_HUMAN && Ammo.Oxigen>0) {
				Ammo.Oxigen--;
				NozzleputterDefault(i);
				break;
			}
			else if (Wapon.Wapon_kind == WAPON_ROCKET && Ammo.Rocket>0) {
				Ammo.Rocket--;
				NozzleputterDefault(i);
				break;
			}
			else if (Wapon.Wapon_kind == WAPON_SNIPER && Ammo.Pow_Bullet>0) {
				Ammo.Pow_Bullet--;
				NozzleputterDefault(i);
				break;
			}
		}

	}


	if (((Wapon.Wapon_kind == WAPON_SHOTGUN) || (Wapon.Wapon_kind == WAPON_DOUBLES))
		&& Key.input[KEY_INPUT_RCONTROL] == 1 && (Ammo.Shell>0)) {
		Ammo.Shell--;
	}
	for (int i = 0; i < SHOTGUN_BULMAX; i++) {

		if ((Wapon.Wapon_kind == WAPON_SHOTGUN)
			&& Key.input[KEY_INPUT_RCONTROL] == 1
			&& Ammo.Shell>0) {

			for (int j = 0; j < SHOTGUN_BULMAX; j++) {
				ShotGunSetter(j);
			}
			if (bul[i].isAlive == false) {
				NozzleputterDefault(i);
			}
		}
	}
	for (int i = 0; i < DOUBLES_BULMAX; i++) {

		if ((Wapon.Wapon_kind == WAPON_DOUBLES)
			&& Key.input[KEY_INPUT_RCONTROL] == 1
			&& (Ammo.Shell>0)) {
			for (int j = 0; j < DOUBLES_BULMAX; j++) {
				ShotGunSetter(j);
			}
			if (bul[i].isAlive == false) {
				NozzleputterDefault(i);
			}
		}
	}

	for (int i = 0; i < MINIGUN_BULMAX; i++) {
		if ((Wapon.Wapon_kind == WAPON_MINIGUN)
			&& Key.input[KEY_INPUT_RCONTROL] >= 1
			&& (Ammo.Bullet>0)) {
			Shotdelay %= 3;
			Shotdelaycount++;
			if (Shotdelaycount > 2) {
				Shotdelay++;
				Shotdelaycount = 0;
			}
			if ((Minigun.ShotF[i] == false) && (Shotdelay > 2)) {
				Ammo.Bullet--;
				Nozzleputter(i);
				ChainBulletSetter(i);
				Shotdelay = 0;
				break;
			}
		}
	}
	if (Key.input[KEY_INPUT_RCONTROL] >= 1) {
		Wapon.isFired = true;
	}
	else {
		Wapon.isFired = false;
	}
	//----各種バレット起動フェイズ・終了--------


	//----各種バレットを更新--------------------
	for (int i = 0; i < ALL_BULLET_MAX; i++) {
		if (bul[i].isAlive == true && !(Wapon.Wapon_kind == WAPON_SHOTGUN)) {
			if(Wapon.isFlash[i])Wapon.firecount[i]++;
			Fire(i);
		}
	}

	for (int i = 0; i < MINIGUN_BULMAX; i++) {
		if (Minigun.ShotF[i] == true && (Wapon.Wapon_kind == WAPON_MINIGUN)) {
			if (Wapon.isFlash[i])Wapon.firecount[i]++;
			ChainBulletUpdater(i);
		}
	}

	for (int i = 0; i < SHOTGUN_BULMAX; i++) {
		if (bul[i].isAlive == true && (Wapon.Wapon_kind == WAPON_SHOTGUN)) {
			if (Wapon.isFlash[i])Wapon.firecount[i]++;
			ShotGunUpdater(i);
		}
	}

	for (int i = 0; i < DOUBLES_BULMAX; i++) {
		if (bul[i].isAlive == true && (Wapon.Wapon_kind == WAPON_DOUBLES)) {
			if (Wapon.isFlash[i])Wapon.firecount[i]++;
			ShotGunUpdater(i);
		}
	}
	//----各種バレットを更新・終了--------------


	//----マズルフラッシュアニメ----------------
	for (int i = 0; i < ALL_BULLET_MAX; i++) {
		if (Wapon.firecount[i] > 4) {
			Wapon.fireAni[i]++;
			Wapon.firecount[i] = 0;
		}
		if (Wapon.fireAni[i] > 4) {
			Wapon.isFlash[i] = false;
			Wapon.fireAni[i] = 0;
			Wapon.firecount[i] = 0;
		}

		Wapon.fireAni[i] %= 5;
	}

	//----マズルフラッシュアニメ・終了----------


	//----ダメージ判定-----------------------
	if (P.isDamaged) {
		P.Damagecount++;
		if (P.Damagecount > 60) {
			P.Damagecount = 0;  P.state = STATE_NORMAL;
			P.isDamaged = false;
		}
	}
	//----ダメージ判定・終了-----------------


	//----接地判定、ジャンプ調整----------------
	if (P.isGrounded) {
		P.state = STATE_NORMAL;
	}
	if (Key.input[KEY_INPUT_SPACE] <= 0 && P.isDamaged != true) {
		P.vel_y *= 0.96f;
	}
	//----接地判定、ジャンプ調整----------------

}

void Player_Init() {
	P.x = 200;
	P.y = 380;
	P.vel_x = 0, P.vel_y = 0;
	P.isGrounded = false;
	P.state = STATE_NORMAL;
	P.Jcount = 20;
	P.turn = 0;
	P.lookdown = 0, P.lookup = 0;
	P.hp_max = PLAYER_LIFE_MAX_LV1;
	P.hp = P.hp_max;

}

void Player_Release() {

}

void Player_Update() {
	switch (P.state) {
	case STATE_NORMAL:		Move(deltatime);	break;
	case STATE_JUMP:		Jump(deltatime);	break;
	}
	if (P.hp >= P.hp_max) P.hp = P.hp_max;
	if (P.hp <= 0) P.hp = 0;
	PlayerX1 = P.x - (BLOCK_SIZE / 2 - 8);
	PlayerY1 = P.y - (BLOCK_SIZE * 2 - 16);
	PlayerX2 = P.x + (BLOCK_SIZE / 2 - 8);
	PlayerY2 = P.y;

	UI_LifeSetter(P.hp, P.hp_max);

}

void Player_Rend() {
	if (P.turn == 0) {
		DrawGraphF(P.x - (BLOCK_SIZE / 2), P.y - (BLOCK_SIZE * 2), P.gra[P.AnimNo], TRUE);
	}
	else {
		DrawTurnGraphF(P.x - (BLOCK_SIZE / 2), P.y - (BLOCK_SIZE * 2), P.gra[P.AnimNo], TRUE);
	}
#ifdef DEBUG
	DrawBox(PlayerX1, PlayerY1, PlayerX2, PlayerY2, 0xFF0000, FALSE);
#endif
}