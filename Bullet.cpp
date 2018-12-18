#include "Global.h"
#include "time.h"
#include"math.h"

struct AMMO Ammo;
int bulgra[4];
BULLET bul[ALL_BULLET_MAX];
struct MINIGUN_PARAM Minigun;
int rocket_velocity[ALL_BULLET_MAX];
int Shotdelay;
int Shotdelaycount;

float Bullet_X1[ALL_ENEMY_MAX];
float Bullet_Y1[ALL_ENEMY_MAX];
float Bullet_X2[ALL_ENEMY_MAX];
float Bullet_Y2[ALL_ENEMY_MAX];

float MiniBul_X1[MINIGUN_BULMAX];
float MiniBul_Y1[MINIGUN_BULMAX];
float MiniBul_X2[MINIGUN_BULMAX];
float MiniBul_Y2[MINIGUN_BULMAX];

static void Kill(WAPON_KIND kind, int no) {

	if (kind == WAPON_MINIGUN) {
		for (int i = 0; i < ALL_BULLET_MAX; i++) { Wapon.isFlash[i] = false; }
		Wapon.fireAni[no] = 0;
		Wapon.firecount[no] = 0;
		Minigun.ShotT[no] = 0;
		Minigun.ShotC[no] = 0;
		Minigun.ShotF[no] = false;
	}
	else {
		P.fire = 0;
		bul[no].time = 0;
		for (int i = 0; i < ALL_BULLET_MAX; i++) { Wapon.isFlash[i] = false; }
		Wapon.fireAni[no] = 0;
		Wapon.firecount[no] = 0;
		bul[no].cool = 0;
		rocket_velocity[no] = 0;
		bul[no].alive = false;
	}
}

static void Bullet_HitCheck(WAPON_KIND kind, int no, float pos_x, float pos_y) {

	float head = pos_y + BLOCK_SIZE / 2;
	float right = pos_x + BLOCK_SIZE - (BLOCK_SIZE / 2);
	float left = pos_x + (BLOCK_SIZE / 2);

	bool HitTile_L = STAGE_HitCheck(left, head);
	bool HitTile_R = STAGE_HitCheck(right, head);//Enemyとかの衝突判定を適宜追加していけば弾の削除は全部やってくれるようになる。
	bool HitEnemy01;
	for (int i = 0; i < ENEMY_MAX; i++) {HitEnemy01 = HitChecker(left, head, right, head, Enemy01_X1[i], Enemy01_Y1[i], Enemy01_X2[i], Enemy01_Y2[i]);}//うまく機能してない？？
	if (HitTile_R || HitTile_L || HitEnemy01) {
		if (kind == WAPON_MINIGUN)	Kill(WAPON_MINIGUN, no);
		else						Kill(WAPON_BASE , no);
	}
}

void Bulletgetter() {

	for (int i = 0; i < ALL_BULLET_MAX; i++) {
		for (int j = 0; j < MINIGUN_BULMAX; j++) {
			if ((bul[i].x < 0 - 32 || bul[i].x>640)
				|| (Minigun.ShotX[j] < 0 - 32 || Minigun.ShotX[j]>640)) {
			P.fire = 0;
			Minigun.ShotF[j] = false;
			}
		}
		if ((Wapon.Wapon_kind == WAPON_PISTOL)
) {
			bul[i].kind = 0;
		}
		if (Wapon.Wapon_kind == WAPON_HUMAN) {
			bul[i].kind = 1;
		}
		if ((Wapon.Wapon_kind == WAPON_SNIPER)
			|| (Wapon.Wapon_kind == WAPON_MINIGUN)
			|| (Wapon.Wapon_kind == WAPON_SHOTGUN)
			|| (Wapon.Wapon_kind == WAPON_DOUBLES)) {
			bul[i].kind = 2;
		}
		if (Wapon.Wapon_kind == WAPON_ROCKET) {
			bul[i].kind = 3;
		}
	}
}

void Nozzleputter(int i) {

	Minigun.ShotR[i] = P.turn;
	if (Wapon.Wapon_kind == WAPON_MINIGUN) {
		Minigun.ShotY[i] = P.y - 28;//14
	}
}

void NozzleputterDefault(int i) {
	bul[i].turn = P.turn;
		if (P.turn == 0) {
			bul[i].x = P.x + 12;//6
		}
		else if (P.turn == 1) {
			bul[i].x = P.x - 46;//23
		}

			bul[i].y = P.y - 46;//23

		bul[i].alive = true;
}

void ChainBulletSetter(int no) {
			if (Minigun.ShotR[no] == true) { Minigun.ShotX[no] = P.x - 56.0f; }//28.0f
			else if (Minigun.ShotR[no] == false) { Minigun.ShotX[no] = P.x + 24.0f; }//12.0f
			Minigun.ShotY[no] = P.y - 22.0f/*11.0f*/ - (rand() % 10/*5*/ + rand() % 10/*5*/);
			Minigun.ShotF[no] = true;
}

void ChainBulletUpdater(int no) {
		if (Minigun.ShotR[no] == true) Minigun.ShotX[no] -= MINIGUN_SPEED;
		if (Minigun.ShotR[no] == false) Minigun.ShotX[no] += MINIGUN_SPEED;

		if (!(Minigun.ShotT[no] < MINIGUN_RANGE)) {
			Minigun.ShotT[no]++;
		}

		if ((Minigun.ShotT[no] >= MINIGUN_RANGE)) {

			Minigun.ShotT[no] = MINIGUN_RANGE;
			Minigun.ShotC[no]++;
		}

		if ((Minigun.ShotC[no] > MINIGUN_DERAY)
			|| Minigun.ShotX[no] < (GetPlayerPosX() - SCREEN_WIDTH / 2)
			|| Minigun.ShotX[no] > (GetPlayerPosX() + SCREEN_WIDTH / 2)) {
			Kill(WAPON_MINIGUN, no);
		}
		Bullet_HitCheck(WAPON_MINIGUN, no, Minigun.ShotX[no], Minigun.ShotY[no]);
}

float RSM;
//PI/6は30度にあたる。この範囲で乱数を発生させ、シェルの弾を拡散させる。
//因みにPI/4が45度のため、分母を減らす度に集弾性能は下がる。
//PI/2は垂直(90度)。PI=180
//左を向くとまた角度が複雑になる。sprashをfloatにしているのはこのため。
void ShotGunSetter(int no) {
	if (Wapon.Wapon_kind == WAPON_SHOTGUN) {
		Wapon.sprash = 7;
	}
	else if (Wapon.Wapon_kind == WAPON_DOUBLES) {
		Wapon.sprash = 2;
	}
		RSM = PI / Wapon.sprash;

		bul[no].angle = RandAtoZ(-RSM, RSM);
}

void ShotGunUpdater(int no) {
	if (Wapon.Wapon_kind == WAPON_SHOTGUN) {
		if (bul[no].turn == 0) {
			bul[no].x += cos(bul[no].angle) * SHOTGUN_SPEED;
		}
		else {
			bul[no].x -= cos(bul[no].angle) * SHOTGUN_SPEED;
		}

		bul[no].y += sin(bul[no].angle) * SHOTGUN_SPEED;
		bul[no].time++;
		if ((bul[no].time > SHOTGUN_RANGE)) {
			bul[no].time = 0;
			bul[no].cool++;
			if ((bul[no].cool > SHOTGUN_DERAY)) {
				Kill(WAPON_SHOTGUN, no);
			}
		}
	}
	else if (Wapon.Wapon_kind == WAPON_DOUBLES) {
			if (bul[no].turn == 0) {
				bul[no].x += cos(bul[no].angle) * DOUBLES_SPEED;
			}
			else {
				bul[no].x -= cos(bul[no].angle) * DOUBLES_SPEED;
			}

			bul[no].y += sin(bul[no].angle) * DOUBLES_SPEED;
			bul[no].time++;
			if ((bul[no].time > DOUBLES_RANGE)) {
				bul[no].time = 0;
				bul[no].cool++;
				if ((bul[no].cool > DOUBLES_DERAY)) {
					Kill(WAPON_DOUBLES, no);
				}
			}
		}
	Bullet_HitCheck(WAPON_BASE, no, bul[no].x, bul[no].y);
}

void Fire(int i)
{
	if (bul[i].turn == 0) {
		if (Wapon.Wapon_kind == WAPON_PISTOL) { bul[i].x += PISTOL_SPEED; }
		if (Wapon.Wapon_kind == WAPON_HUMAN) { bul[i].x += HUMAN_SPEED; }
		if (Wapon.Wapon_kind == WAPON_ROCKET) { bul[i].x += ROCKET_SPEED + rocket_velocity[i] * 0.5f; rocket_velocity[i]++; }
		if (Wapon.Wapon_kind == WAPON_SNIPER) { bul[i].x += SNIPER_SPEED; }
		if (Wapon.W_type >= 21 && Wapon.W_type <= 23) { bul[i].x += PYLO_SPEED; }
		bul[i].time++;
	}

		else if (bul[i].turn == 1) {
			if (Wapon.Wapon_kind == WAPON_PISTOL) { bul[i].x -= PISTOL_SPEED; }
			if (Wapon.Wapon_kind == WAPON_HUMAN) { bul[i].x -= HUMAN_SPEED; }
			if (Wapon.Wapon_kind == WAPON_ROCKET) { bul[i].x -= ROCKET_SPEED + rocket_velocity[i] * 0.5f; rocket_velocity[i]++; }
			if (Wapon.Wapon_kind == WAPON_SNIPER) { bul[i].x -= SNIPER_SPEED; }
			if (Wapon.W_type >= 21 && Wapon.W_type <= 23) { bul[i].x -= PYLO_SPEED; }
			bul[i].time++;
		}

		if (((Wapon.Wapon_kind == WAPON_PISTOL) && (bul[i].time > PISTOL_RANGE))
			|| ((Wapon.Wapon_kind == WAPON_HUMAN) && (bul[i].time > HUMAN_RANGE))
			|| ((Wapon.Wapon_kind == WAPON_ROCKET) && (bul[i].time > ROCKET_RANGE))
			|| ((Wapon.Wapon_kind == WAPON_SNIPER) && (bul[i].time > SNIPER_RANGE))
			|| ((Wapon.W_type >= 21 && Wapon.W_type <= 23) && (bul[i].time > PYLO_RANGE))) {
			bul[i].time = 0;
			bul[i].cool++;

			if (((Wapon.Wapon_kind == WAPON_PISTOL) && (bul[i].cool > PISTOL_DERAY))
				|| ((Wapon.Wapon_kind == WAPON_HUMAN) && (bul[i].cool > HUMAN_DERAY))
				|| ((Wapon.Wapon_kind == WAPON_ROCKET) && (bul[i].cool > ROCKET_DERAY))
				|| ((Wapon.Wapon_kind == WAPON_SNIPER) && (bul[i].cool > SNIPER_DERAY))
				|| ((Wapon.W_type >= 21 && Wapon.W_type <= 23) && (bul[i].cool > PYLO_DERAY))
				|| bul[i].x < (GetPlayerPosX() - SCREEN_WIDTH / 2)
				|| bul[i].x > (GetPlayerPosX() + SCREEN_WIDTH / 2)) {
				Kill(WAPON_PISTOL, i);
			}
		}
		Bullet_HitCheck(WAPON_BASE, i, bul[i].x, bul[i].y);
}


void Bullet_Init() {

	for (int i = 0; i < ALL_BULLET_MAX; i++) {
		bul[i].kind = 0;
		bul[i].hit = false;
		bul[i].alive = false;
		bul[i].angle = 0;
	}
	for (int j = 0; j < MINIGUN_BULMAX; j++) {
		Minigun.ShotX[j] = P.x;
		Minigun.ShotY[j] = P.y - 14;
		Minigun.ShotF[j] = false;
	}

	Ammo.Bullet = BULLET_MAX;
	Ammo.Oxigen = OXIGEN_MAX;
	Ammo.Rocket = ROCKET_MAX;
	Ammo.Shell = SHELL_MAX;
	Ammo.Pow_Bullet = POWBULLET_MAX;
	Ammo.Gasoline = GASOLINE_MAX;
}

void Bullet_Release() {

}

void Bullet_Update() {
	if (Wapon.Wapon_kind == WAPON_PISTOL || Wapon.Wapon_kind == WAPON_MINIGUN) {
		Ammo.MaxAmmo = BULLET_MAX;
		Ammo.NowAmmo = Ammo.Bullet;
	}
	else if (Wapon.Wapon_kind == WAPON_HUMAN) {
		Ammo.MaxAmmo = OXIGEN_MAX;
		Ammo.NowAmmo = Ammo.Oxigen;
	}
	else if (Wapon.Wapon_kind == WAPON_ROCKET) {
		Ammo.MaxAmmo = ROCKET_MAX;
		Ammo.NowAmmo = Ammo.Rocket;
	}
	else if (Wapon.Wapon_kind == WAPON_SNIPER) {
		Ammo.MaxAmmo = POWBULLET_MAX;
		Ammo.NowAmmo = Ammo.Pow_Bullet;
	}
	else if (Wapon.Wapon_kind == WAPON_SHOTGUN || Wapon.Wapon_kind == WAPON_DOUBLES) {
		Ammo.MaxAmmo = SHELL_MAX;
		Ammo.NowAmmo = Ammo.Shell;
	}
	else if (Wapon.W_type >= 21 && Wapon.W_type <= 23) {
		Ammo.MaxAmmo = GASOLINE_MAX;
		Ammo.NowAmmo = Ammo.Gasoline;
	}
	if (Ammo.NowAmmo > Ammo.MaxAmmo) Ammo.NowAmmo = Ammo.MaxAmmo;

	UI_ReadAmmoNo(Ammo.NowAmmo, Ammo.MaxAmmo);

	//-----Hit----------

	for (int i = 0; i < ALL_BULLET_MAX; i++) {
		Bullet_X1[i] = bul[i].x;
		Bullet_Y1[i] = bul[i].y + (BLOCK_SIZE / 4);
		Bullet_X2[i] = bul[i].x + BLOCK_SIZE;
		Bullet_Y2[i] = bul[i].y + (BLOCK_SIZE / 2 + 4);

		for (int t = 0; t < ENEMY_MAX; t++) {

			if (bul[i].alive == true && enemy01[t].isDead != true && HitChecker(Bullet_X1[i], Bullet_Y1[i], Bullet_X2[i], Bullet_Y2[i],
					Enemy01_X1[t], Enemy01_Y1[t], Enemy01_X2[t], Enemy01_Y2[t])) {
				
				if (Wapon.Wapon_kind == WAPON_PISTOL)	{enemy01[t].hp -= PISTOL_DAMAGE; }
				if (Wapon.Wapon_kind == WAPON_HUMAN)	{enemy01[t].hp -= HUMAN_DAMAGE;  }
				if (Wapon.Wapon_kind == WAPON_ROCKET)	{enemy01[t].hp -= ROCKET_DAMAGE; }
				if (Wapon.Wapon_kind == WAPON_SHOTGUN)	{enemy01[t].hp -= SHOTGUN_DAMAGE;}
				if (Wapon.Wapon_kind == WAPON_SNIPER)	{enemy01[t].hp -= SNIPER_DAMAGE; }
				if (Wapon.Wapon_kind == WAPON_DOUBLES)	{enemy01[t].hp -= DOUBLES_DAMAGE;}
			}
		}
	}
	for (int j = 0; j < MINIGUN_BULMAX; j++) {
		MiniBul_X1[j] = Minigun.ShotX[j];
		MiniBul_Y1[j] = Minigun.ShotY[j] + (BLOCK_SIZE / 4);
		MiniBul_X2[j] = Minigun.ShotX[j] + BLOCK_SIZE;
		MiniBul_Y2[j] = Minigun.ShotY[j] + (BLOCK_SIZE / 2 + 4);

		for (int t = 0; t < ENEMY_MAX; t++) {
			if (Wapon.Wapon_kind == WAPON_MINIGUN && Minigun.ShotF[j] == true && enemy01[t].isDead != true && HitChecker(MiniBul_X1[j], MiniBul_Y1[j], MiniBul_X2[j], MiniBul_Y2[j],
				Enemy01_X1[t], Enemy01_Y1[t], Enemy01_X2[t], Enemy01_Y2[t])) {
				enemy01[t].hp -= MINIGUN_DAMAGE;
			}
		}
	}
}

void Bullet_Rend() {
	for (int i = 0; i < ALL_BULLET_MAX; i++) {
		if (bul[i].time != 0 && !(Wapon.Wapon_kind == WAPON_SHOTGUN) && !(Wapon.Wapon_kind == WAPON_MINIGUN) && !(Wapon.Wapon_kind == WAPON_DOUBLES)) {
			if (bul[i].turn == 0) {
				DrawGraphF(bul[i].x, bul[i].y, bulgra[bul[i].kind], TRUE);
			}
			else if (bul[i].turn == 1) {
				DrawTurnGraphF(bul[i].x, bul[i].y, bulgra[bul[i].kind], TRUE);
			}
		}
#ifdef DEBUG
		DrawBox(Bullet_X1[i], Bullet_Y1[i], Bullet_X2[i], Bullet_Y2[i], 0x0000FF, FALSE);
#endif
	}

	//------ミニガンのバレット描画-----------
	for (int j = 0; j < MINIGUN_BULMAX; j++) {
		if ((Wapon.Wapon_kind == WAPON_MINIGUN) && Minigun.ShotF[j] == true) {
			if (Minigun.ShotR[j] == false) {
				DrawGraphF(Minigun.ShotX[j], Minigun.ShotY[j], bulgra[2], TRUE);
			}
			else if (Minigun.ShotR[j] == true) {
				DrawTurnGraphF(Minigun.ShotX[j], Minigun.ShotY[j], bulgra[2], TRUE);
			}

		}
#ifdef DEBUG
		DrawBox(MiniBul_X1[j], MiniBul_Y1[j], MiniBul_X2[j], MiniBul_Y2[j], 0x0000FF, FALSE);
#endif
	}

	//------ショットガンのバレット描画-------
	for (int t = 0; t < SHOTGUN_BULMAX; t++) {
		if (Wapon.Wapon_kind == WAPON_SHOTGUN && bul[t].alive == true) {
			//RotaGraph：中心X、Y、拡大率、角度、ファイル、透過TRUE
			if (bul[t].turn == 0) {
				DrawRotaGraphF(bul[t].x + (BLOCK_SIZE / 2), bul[t].y + (BLOCK_SIZE / 2), 
					1.0f, bul[t].angle, bulgra[bul[t].kind], TRUE);
			}
			else if (bul[t].turn == 1) {
				DrawRotaGraphF(bul[t].x + (BLOCK_SIZE / 2), bul[t].y + (BLOCK_SIZE / 2), 
					1.0f, -(bul[t].angle + PI), bulgra[bul[t].kind], TRUE);
			}
		}
	}
	for (int t = 0; t < DOUBLES_BULMAX; t++) {
		if (Wapon.Wapon_kind == WAPON_DOUBLES && bul[t].alive == true) {
			//RotaGraph：中心X、Y、拡大率、角度、ファイル、透過TRUE
			if (bul[t].turn == 0) {
				DrawRotaGraphF(bul[t].x + (BLOCK_SIZE / 2), bul[t].y + (BLOCK_SIZE / 2),
					1.0f, bul[t].angle, bulgra[bul[t].kind], TRUE);
			}
			else if (bul[t].turn == 1) {
				DrawRotaGraphF(bul[t].x + (BLOCK_SIZE / 2), bul[t].y + (BLOCK_SIZE / 2),
					1.0f, -(bul[t].angle + PI), bulgra[bul[t].kind], TRUE);
			}
		}
	}
}