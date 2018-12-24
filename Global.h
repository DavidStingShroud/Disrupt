////////////////////////////////////////////////////////////////////////////
//																		  //
//			               Ms.Flint GunFire(c)							  //
//				"Global Game Manager" from Dans N' Dears				  //
//		           ｺﾞｼﾖｳﾉｻｲﾊ ﾖｳﾘｮｳｦﾏﾓﾘ ﾃｷｾﾂ ﾆ ｺﾞﾘﾖｳﾊ ｹｲｶｸﾃｷﾆ.			  //
//																		  //
////////////////////////////////////////////////////////////////////////////
#include "DxLib.h"
#include "KeyInput.h"
#include "Wapon.h"
#include "Player.h"
#include "Stage.h"
#include "Bullet.h"
#include "time.h"
#include "UI.h"
#include "Game.h"
#include "Enemy.h"
#include "ObjectHit.h"
#include "Particle.h"
#include "Setting.h"

//----今月のDEFINE特集------------------------

	//System
#define DEBUG

#define SCREEN_WIDTH     640
#define SCREEN_HEIGHT    480
#define BLOCK_SIZE        32
#define MAP_WIDTH        (SCREEN_WIDTH / BLOCK_SIZE)
#define MAP_HEIGHT       (SCREEN_HEIGHT / BLOCK_SIZE)


	//Player
#define PLAYER_CHIP				  16

#define PLAYER_LIFE_MAX_LV1      100
#define PLAYER_LIFE_MAX_LV2      200
#define PLAYER_LIFE_MAX_LV3      500
	
	//Bullet
#define ALL_BULLET_MAX			 128


	//Particle
#define PARTICLE_MAX			 128
#define PARTICLE_MAX2			  64

//----銃関係をぶちこめ-----------------

#define WAPON_MAX 8				//使用する武器の総数

	//ステータス
#define PISTOL_DAMAGE	  8.0f  //攻　撃　力
#define PISTOL_DERAY      0.7f	//発射ディレイのこと。
#define PISTOL_SPEED     14.0f	//弾速のこと。
#define PISTOL_RANGE      6.7f	//射程距離のこと。
#define PISTOL_RELOAD	 30.0f	//リロード時間。
#define PISTOL_MAKER		0	//メーカー番号。

#define HUMAN_DAMAGE	  2.0f
#define HUMAN_DERAY       0.3f
#define HUMAN_SPEED       3.0f
#define HUMAN_RANGE      30.3f
#define HUMAN_RELOAD	 10.0f
#define HUMAN_MAKER			1
#define HUMAN_BULMAX        6	//画面に存在できる限界の数。

#define ROCKET_DAMAGE	 24.0f
#define ROCKET_DERAY      1.5f
#define ROCKET_SPEED      1.5f
#define ROCKET_RANGE    250.0f
#define ROCKET_RELOAD	 52.0f//TODO : リロード定数を基準に各武器のリロードタイムを作ること
#define ROCKET_MAKER		4

#define SROCKET_DAMAGE	 36.0f
#define SROCKET_DERAY     1.0f
#define SROCKET_SPEED     2.0f
#define SROCKET_RANGE   250.0f
#define SROCKET_RELOAD	 48.0f
#define SROCKET_MAKER		4

#define SHOTGUN_DAMAGE	  3.0f
#define SHOTGUN_DERAY     0.3f
#define SHOTGUN_SPEED    16.6f
#define SHOTGUN_RANGE     6.0f
#define SHOTGUN_RELOAD	 48.0f
#define SHOTGUN_MAKER		3
#define SHOTGUN_BULMAX      4

#define SNIPER_DAMAGE	  9.0f
#define SNIPER_DERAY      1.2f
#define SNIPER_SPEED     28.4f
#define SNIPER_RANGE    250.0f
#define SNIPER_RELOAD	 67.0f
#define SNIPER_MAKER		2

#define DOUBLES_DAMAGE	  5.0f
#define DOUBLES_DERAY     0.7f
#define DOUBLES_SPEED    16.6f
#define DOUBLES_RANGE    13.4f
#define DOUBLES_RELOAD	 60.0f
#define DOUBLES_MAKER		5
#define DOUBLES_BULMAX		7

#define MINIGUN_DAMAGE	  2.0f
#define MINIGUN_DERAY     0.2f
#define MINIGUN_SPEED    17.2f
#define MINIGUN_RANGE    50.0f
#define MINIGUN_MAKER		5
#define MINIGUN_BULMAX     40

#define PYLO_DAMAGE		  5.0f
#define PYLO_DERAY        0.0f	//データを作っていないように見えるものの、実際は全く異なる処理を用意しようと
#define PYLO_SPEED        0.0f	//考えているだけ。とりあえず４つくらい実装したらもうリリースしてもいいのでは？
#define PYLO_RANGE        0.0f	//初期武器候補：ピストル[ﾚ]、軽量ショットガン[ﾚ]、スナイパーライフル[ﾚ]、ミニガン[ﾚ]
#define PYLO_MAKER			0

//----最大限確保できる銃弾の数------

#define BULLET_MAX       500
#define OXIGEN_MAX       333
#define ROCKET_MAX        48
#define SHELL_MAX         54
#define POWBULLET_MAX     30
#define GASOLINE_MAX     300

//----ミニガン用の座標とかいろいろ-----------

struct MINIGUN_PARAM {
	float  ShotX[MINIGUN_BULMAX];//X座標
	float  ShotY[MINIGUN_BULMAX];//Y座標
	bool   ShotF[MINIGUN_BULMAX];//発射フラグ
	int    ShotT[MINIGUN_BULMAX];//滑空時間
	int    ShotC[MINIGUN_BULMAX];//カウント
	bool   ShotR[MINIGUN_BULMAX];//false=左、true=右。ミニガン用の処理。連射できる武器はこのタイプを採用しようね。
	bool   ShotH[MINIGUN_BULMAX];//ヒットフラグ
}; 
extern struct MINIGUN_PARAM Minigun;
extern int Shotdelay;
extern int Shotdelaycount;			//こちら２つは、発射間隔に使っております。

//----敵関係-----------

//  0 ~  63 : Creep, 
// 64 ~ 127 : Wasp, 
//128 ~ 191 : Brute, 
//192 ~ 255 : Bloodsoak,
//256 ~ 319 : Torture。ラビネットはリストラ。
#define ALL_ENEMY_MAX	320
#define ENEMY_MAX		 64//敵限定のＭＡＸ。ライフ処理など、敵の種類を問わず行う処理は上のＭＡＸを使うこと。

	//ステータス
#define ENEMY01_LIFE		27//体力。
#define ENEMY01_ATTACK01	14//体当たり。

#define ENEMY02_LIFE		40
#define ENEMY02_ATTACK01	10//体当たり。
#define ENEMY02_ATTACK02	 5//ショットガン。1発辺りのダメージ。ｘ５。

#define ENEMY03_LIFE		60
#define ENEMY03_ATTACK01	15//体当たり。
#define ENEMY03_ATTACK02	 7//レーザー攻撃。1/60秒あたりのダメージ。

#define ENEMY04_LIFE		32
#define ENEMY04_ATTACK01	 1//体当たり。
#define ENEMY04_ATTACK02	13//ライフルダメージ。3点バースト。

//ボス体力目安：
	//(Creep基準。実際はもっと倒しづらくなると思われる)
	// 500 : ピストル200発程度
	// 730 : ショットガン全弾、ピストル30発程度
	//1000 : ショットガン全弾、スナイパーライフル全弾、数発のピストルでようやく倒せる
	//3000 : スーパーショットガン全弾、ミニガン全弾、ありったけのロケットでようやく倒せる
	//3700 : なんとか倒せる
#define BOSS01_LIFE		750


//----その他定数-------

#define PI 3.141592654//きっとあらゆる側面で役に立つ。私は彼の存在に感謝し、崇拝せねばならない。



//-----enum enough----------------

enum GAME_STATE {
	GAMESTATE_TITLE = 0,
	GAMESTATE_MAIN,
	GAMESTATE_PAUSE,
};

enum STATE {//あくまでも「プレイヤーの」状態管理定数。
	STATE_NORMAL = 0,
	STATE_JUMP,
	STATE_ATTACK,
};

enum WAPON_KIND {
	WAPON_PISTOL = 0,
	WAPON_HUMAN,
	WAPON_ROCKET,
	WAPON_SHOTGUN,
	WAPON_SNIPER,
	WAPON_DOUBLES,
	WAPON_MINIGUN,
	WAPON_PYLO,
	WAPON_BASE,//厳密に種類を決定する必要があるときと、ミニガン以外は大体これで
};

enum ITEM_KIND {
	ITEM_AMMO_BULLET = 0,	//弾薬・2mm
	ITEM_AMMO_OXIGEN,		//弾薬・酸素
	ITEM_AMMO_ROCKET,		//弾薬・ロケット
	ITEM_AMMO_SHELL,		//弾薬・シェル
	ITEM_AMMO_POW_BULLET,	//弾薬・.32
	ITEM_AMMO_GASOLINE,		//弾薬・燃料
	ITEM_LIFE_POTION,		//体力回復・小
	ITEM_LIFE_CUPSULE,		//体力回復・中
	ITEM_MEDI_KIT,			//支援物資
	ITEM_MISC_QC01,	//QC拡張+1
	ITEM_MISC_QC02,		//QC拡張+2
	ITEM_MISC_QC03,	//QC拡張+3
	ITEM_MISC_AMMO15,		//所持上限x1.5
	ITEM_MISC_AMMO20,		//所持上限x2.0
};

enum ENEMY_KIND {
	ENEMY_CREEP = 0,
	ENEMY_TFM,
	ENEMY_BLOODSOAK,
	ENEMY_HUMAN,
};

//----STRUCT------------------------
static struct OBJECT {
	int gra[32];
	float x, y;
	float vel_x, vel_y;
	int turn;//0は右。1が左。
	int AnimNo;
	int Anicount;
	bool isAlive;
	bool isGrounded;

};

struct KEY {//入力を全部取ってきてくれる使い魔的なやつ。できることは少ないけれど優秀。
	int input[256];
};

struct PLAYER : public OBJECT {
	STATE state;
	float hp;
	float hp_max;
	int Jcount;
	bool lookup;
	bool lookdown;
	int fire;
	bool isDamaged;
	int Damagecount;//無敵時間用
	bool QC_Enabled[3];//QC枠の利用判定用
};

struct ENEMY : public OBJECT {
	ENEMY_KIND kind;
	float hp;
	float atk01, atk02, atk03;
	bool isNear;//ある程度近いとtrueとなります。
};

struct WAPON : public OBJECT{
	int W_type;
	WAPON_KIND Wapon_kind;
	bool isFired;//発射してる状態だとtrueになってると幸せだよね～。
	int Fire_gra[10];//ファイアアニメーションはWAPONが保有。残念ながらバレットではない。
	int fireAni[ALL_BULLET_MAX];
	int firecount[ALL_BULLET_MAX];
	bool isFlash[ALL_BULLET_MAX];//フラッシュって一度だけでいいよね。
	float sprash;//拡散度を示す。
	bool have[WAPON_MAX];//武器を所持しているかどうか(各種武器の番号に対応)
	int FastQC[4];//クイックチェンジ用配列。WAPON_KINDを格納する。
	int NumQC[9];//ナンバーで切り替えるタイプのＱＣ。今回は使わない。
};

struct BULLET : public OBJECT {
	int kind;
	float angle;
	bool hit;
	float time;//飛行時間を示す。
	float cool;
	bool isBorned;//生成直後フラグ
	float ReloadTime;//リロードにかかる時間(攻撃不能時間)
};

struct AMMO {//残り弾数を管理します。
	int Bullet;
	int Oxigen;
	int Rocket;
	int Shell;
	int Pow_Bullet;
	int Gasoline;
	int NowAmmo;//今抱えてる弾数。
	int MaxAmmo;//最大抱えられる弾数。
};

struct PARTICLE : OBJECT {

};

struct ITEM : public OBJECT {
	bool isTaked;
};

struct TILE {
	int gra[64];
	int StageBG_Use;
	int StageBG[12];
};


//-----構造体ショーケース----------

extern KEY Key;
extern PLAYER P;
extern ENEMY enemy01[ENEMY_MAX];
extern WAPON Wapon;
extern BULLET bul[ALL_BULLET_MAX];
extern AMMO Ammo;
extern PARTICLE Gore[PARTICLE_MAX];
extern PARTICLE GoreTall[PARTICLE_MAX2];
extern ITEM Item;
extern TILE Tile;


//----その他他所から取ってきてほしいもの----

extern float deltatime;
extern int Number[10];
extern int Ammo_No[3];
extern int Ammo_NoMax[3];
extern int Score;
extern int Score_No[8];