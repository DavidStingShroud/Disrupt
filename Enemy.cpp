#include "Global.h"
#include <math.h>
struct ENEMY enemy01[ENEMY_MAX];

static void Enemy_BlockCheck_X(ENEMY_KIND kind, int no) {

	if (kind == ENEMY_CREEP) {
		float headY = enemy01[no].y - 4.0f;
		float bodyY = enemy01[no].y - 36.0f;
		float footY = enemy01[no].y - 4.0f;
		float rightX = enemy01[no].x + 12.0f;
		float leftX = enemy01[no].x - 12.0f;

		//	âE
		bool Hit_HeadR = STAGE_HitCheck(rightX, headY);
		bool Hit_BodyR = STAGE_HitCheck(rightX, bodyY);
		bool Hit_FootR = STAGE_HitCheck(rightX, footY);
		if (Hit_HeadR || Hit_BodyR || Hit_FootR) { enemy01[no].x = STAGE_GetBlockX(rightX) - 14.0f; }
		//	ç∂
		bool Hit_headL = STAGE_HitCheck(leftX, headY);
		bool Hit_bodyL = STAGE_HitCheck(leftX, bodyY);
		bool Hit_footL = STAGE_HitCheck(leftX, footY);
		if (Hit_headL || Hit_bodyL || Hit_footL) { enemy01[no].x = STAGE_GetBlockX(leftX) + BLOCK_SIZE + 12.0f; }
	}

	else if (kind == ENEMY_TFM) {

	}
	else if (kind == ENEMY_BLOODSOAK) {

	}
	else if (kind == ENEMY_HUMAN) {

	}
}

static void Enemy_BlockCheck_Y(ENEMY_KIND kind, int no) {

	if (kind == ENEMY_CREEP) {
		float left = enemy01[no].x - 12.0f;
		float right = enemy01[no].x + 12.0f;
		//ì™ë≈Ç¬Åiè„è∏íÜÇÃÇ›Åj
		if (enemy01[no].vel_y < 0) {
			bool Hitleft = STAGE_HitCheck(left, enemy01[no].y - BLOCK_SIZE);
			bool Hitright = STAGE_HitCheck(right, enemy01[no].y - BLOCK_SIZE);
			if (Hitleft || Hitright) {

				enemy01[no].vel_y = 0;
				enemy01[no].y = (float)STAGE_GetBlockY(enemy01[no].y - BLOCK_SIZE);
				enemy01[no].y += 32.0f;

			}
		}

		if (enemy01[no].vel_y >= 0) {
			bool Hitleft = STAGE_HitCheck(left, enemy01[no].y);
			bool Hitright = STAGE_HitCheck(right, enemy01[no].y);

			if (Hitleft || Hitright) {
				enemy01[no].vel_y = 0;

				enemy01[no].y = (float)STAGE_GetBlockY(enemy01[no].y);

				enemy01[no].isGrounded = true;

				enemy01[no].vel_x -= enemy01[no].vel_x * 0.1f;

				if (fabsf(enemy01[no].vel_x) < 10) {
					enemy01[no].vel_x = 0;
				}
			}
		}
	}

	else if (kind == ENEMY_TFM) {

	}
	else if (kind == ENEMY_BLOODSOAK) {

	}
	else if (kind == ENEMY_HUMAN) {

	}
}

void Enemy_Init()
{
	enemy01[0].isDead = false;

	enemy01[0].x = 300;

	enemy01[0].y = 330;

	for (int i = 0; i < ENEMY_MAX; i++) {
		if (enemy01[i].isDead != true) {
			enemy01[i].kind = (ENEMY_KIND)ENEMY_CREEP;
			enemy01[i].hp = ENEMY01_LIFE;
			enemy01[i].atk01 = ENEMY01_ATTACK01;
		}
	}

}

void Enemy_Release()
{
}

float Enemy01_X1[ENEMY_MAX];
float Enemy01_Y1[ENEMY_MAX];
float Enemy01_X2[ENEMY_MAX];
float Enemy01_Y2[ENEMY_MAX];

void Enemy_Update()
{
	for (int i = 0; i < ENEMY_MAX; i++) {

		if (enemy01[i].isDead != true) {

		//-----Animation----------
			if (enemy01[i].isNear != true) {
				enemy01[i].Anicount++;
				enemy01[i].AnimNo %= 5;
				if (enemy01[i].Anicount > 4) {
					enemy01[i].AnimNo++;
					enemy01[i].Anicount = 0;
				}
				if (enemy01[i].AnimNo > 3) {
					enemy01[i].AnimNo = 0;
					enemy01[i].Anicount = 0;
				}
			}
			else {
				enemy01[i].Anicount++;
				if (enemy01[i].Anicount > 4) {
					enemy01[i].AnimNo = 4;
				}
				if (enemy01[i].Anicount > 8) {
					enemy01[i].AnimNo = 5;
					enemy01[i].Anicount = 0;
				}
			}
			//-----Animation----------


			//-----Gravity----------
			if (enemy01[i].isGrounded != true) {
				enemy01[i].vel_y += 9.8f * (100) * deltatime;
				enemy01[i].y += enemy01[i].vel_y * deltatime + 0.89f;
				enemy01[i].isGrounded = false;
				Enemy_BlockCheck_Y(enemy01[i].kind, i);
			}
			//-----Gravity----------


			//-----Looking----------
			if (enemy01[i].x < GetPlayerPosX())enemy01[i].turn = 0;
			else if (enemy01[i].x > GetPlayerPosX())enemy01[i].turn = 1;
			//-----Looking----------


			//-----Move-------------
			if (enemy01[i].x - (BLOCK_SIZE * 5) < GetPlayerPosX() && enemy01[i].x + (BLOCK_SIZE * 5) > GetPlayerPosX()) {
				if (enemy01[i].x < GetPlayerPosX())enemy01[i].vel_x += 0.1;
				else if (enemy01[i].x > GetPlayerPosX())enemy01[i].vel_x -= 0.1;

				if (enemy01[i].vel_x > 10)enemy01[i].vel_x = 10;
				if (enemy01[i].vel_x < -10)enemy01[i].vel_x = -10;
				enemy01[i].isNear = true;
			}
			else {
				enemy01[i].vel_x = 0;
				enemy01[i].isNear = false;
			}

			enemy01[i].x += enemy01[i].vel_x;
			//-----Move-------------


			//-----Hit--------------
			Enemy01_X1[i] = enemy01[i].x - (BLOCK_SIZE / 2 - 8);
			Enemy01_Y1[i] = enemy01[i].y - (BLOCK_SIZE * 2 - 8);
			Enemy01_X2[i] = enemy01[i].x + (BLOCK_SIZE / 2 - 8);
			Enemy01_Y2[i] = enemy01[i].y;
			if (HitChecker(PlayerX1, PlayerY1, PlayerX2, PlayerY2,
				Enemy01_X1[i], Enemy01_Y1[i], Enemy01_X2[i], Enemy01_Y2[i])) {
				P.hp -= enemy01[i].atk01;
			}
			//-----Hit--------------


			//-----Die--------------
			if (enemy01[i].hp <= 0) {
				enemy01[i].isDead = true;
			}

			Enemy_BlockCheck_X(enemy01[i].kind, i);

		}
	}
}

void Enemy_Render()
{
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (enemy01[i].isDead != true) {
			if (enemy01[i].turn == 0) {
				DrawGraphF(enemy01[i].x - (BLOCK_SIZE / 2), enemy01[i].y - (BLOCK_SIZE * 2), enemy01[i].gra[enemy01[i].AnimNo], TRUE);
			}
			else {
				DrawTurnGraphF(enemy01[i].x - (BLOCK_SIZE / 2), enemy01[i].y - (BLOCK_SIZE * 2), enemy01[i].gra[enemy01[i].AnimNo], TRUE);
			}

#ifdef DEBUG
			DrawBox(Enemy01_X1[i], Enemy01_Y1[i], Enemy01_X2[i], Enemy01_Y2[i], 0x00FF00, FALSE);
#endif // DEBUG
		}
	}
}
