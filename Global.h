////////////////////////////////////////////////////////////////////////////
//																		  //
//			               Ms.Flint GunFire(c)							  //
//				"Global Game Manager" from Dans N' Dears				  //
//		           �޼ֳɻ�� ֳخ����� ÷�� � ���ֳ� ����÷�.					  //
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

//----������DEFINE���W------------------------
//�����ŃQ�[���̑S�̓I�Ȑ��l��M��邼�c����ɂ��₪��c�c�B
#define DEBUG

#define SCREEN_WIDTH     (640)
#define SCREEN_HEIGHT    (480)
#define BLOCK_SIZE        (32)
#define MAP_WIDTH        (SCREEN_WIDTH / BLOCK_SIZE)
#define MAP_HEIGHT       (SCREEN_HEIGHT / BLOCK_SIZE)

#define ALL_BULLET_MAX        128

//�퓬�J�n���ɕ�U�����ő�̗͒l�̐ݒ肾�c�B���ɂ�Lv1�ŏ\�����c�c�B
#define PLAYER_LIFE_MAX_LV1      100//
#define PLAYER_LIFE_MAX_LV2      200//
#define PLAYER_LIFE_MAX_LV3      500//

//----�e�֌W���Ԃ�����-----------------
//���했�̃S�^�S�^�������l�̐ݒ肾�c�����ꒃ�������킪�~�������A�����ō��邩���ȁc�c�B

#define WAPON_MAX 8				//�g�p���镐��̑���

	//�X�e�[�^�X
#define PISTOL_DAMAGE	  8.0f  //�U�@���@��
#define PISTOL_DERAY      0.7f	//���˃f�B���C�̂��ƁB
#define PISTOL_SPEED     14.0f	//�e���̂��ƁB
#define PISTOL_RANGE      6.7f	//�˒������̂��ƁB
#define PISTOL_MAKER		0	//���[�J�[�ԍ��B

#define HUMAN_DAMAGE	  2.0f
#define HUMAN_DERAY       0.3f
#define HUMAN_SPEED       3.0f
#define HUMAN_RANGE      30.3f
#define HUMAN_MAKER			1
#define HUMAN_BULMAX        6	//��ʂɑ��݂ł�����E�̐��B���������p�ӂ����̂ł��Ўg���Ăق����B

#define ROCKET_DAMAGE	 12.0f
#define ROCKET_DERAY      1.5f
#define ROCKET_SPEED      1.5f
#define ROCKET_RANGE    250.0f
#define ROCKET_MAKER		4

#define SROCKET_DAMAGE	 30.0f
#define SROCKET_DERAY     1.0f
#define SROCKET_SPEED     2.0f
#define SROCKET_RANGE   250.0f
#define SROCKET_MAKER		4

#define SHOTGUN_DAMAGE	  3.0f
#define SHOTGUN_DERAY     0.3f
#define SHOTGUN_SPEED    16.6f
#define SHOTGUN_RANGE     6.0f
#define SHOTGUN_MAKER		3
#define SHOTGUN_BULMAX      4

#define SNIPER_DAMAGE	  9.0f
#define SNIPER_DERAY      1.2f
#define SNIPER_SPEED     28.4f
#define SNIPER_RANGE    250.0f
#define SNIPER_MAKER		2

#define DOUBLES_DAMAGE	  5.0f
#define DOUBLES_DERAY     0.7f
#define DOUBLES_SPEED    16.6f
#define DOUBLES_RANGE    13.4f
#define DOUBLES_MAKER		5
#define DOUBLES_BULMAX		7

#define MINIGUN_DAMAGE	  5.0f
#define MINIGUN_DERAY     0.2f
#define MINIGUN_SPEED    17.2f
#define MINIGUN_RANGE    50.0f
#define MINIGUN_MAKER		5
#define MINIGUN_BULMAX     40

#define PYLO_DAMAGE		  5.0f
#define PYLO_DERAY        0.0f	//�f�[�^������Ă��Ȃ��悤�Ɍ�������̂́A���ۂ͑S���قȂ鏈����p�ӂ��悤��
#define PYLO_SPEED        0.0f	//�l���Ă��邾���B�Ƃ肠�����S���炢������������������[�X���Ă������̂ł́H
#define PYLO_RANGE        0.0f	//����������F�s�X�g��[�]�A�y�ʃV���b�g�K��[�]�A�X�i�C�p�[���C�t��[�]�A�~�j�K��[�]
#define PYLO_MAKER			0

//----�ő���m�ۂł���e�e�̐�------
//�ő�܂Ŏ��Ă�e��̐ݒ肾�c�~�����������p�ӂ��ȁc�c

#define BULLET_MAX       500
#define OXIGEN_MAX       333
#define ROCKET_MAX        48
#define SHELL_MAX         54
#define POWBULLET_MAX     30
#define GASOLINE_MAX     300

//----�~�j�K���p�̍��W�Ƃ����낢��-----------

struct MINIGUN_PARAM {
	float  ShotX[MINIGUN_BULMAX];
	float  ShotY[MINIGUN_BULMAX];
	bool   ShotF[MINIGUN_BULMAX];
	int    ShotT[MINIGUN_BULMAX];
	int    ShotC[MINIGUN_BULMAX];
	bool   ShotR[MINIGUN_BULMAX];//false=���Atrue=�E�B�~�j�K���p�̏����B�A�˂ł��镐��͂��̃^�C�v���̗p���悤�ˁB
}; 
extern struct MINIGUN_PARAM Minigun;
extern int Shotdelay;
extern int Shotdelaycount;			//������Q�́A���ˊԊu�Ɏg���Ă���܂��B

//----�G�֌W-----------
//�G�̊�{�X�e�[�^�X�̐ݒ肾�c�B����܂苭���ƒɂ��ڂ݂邼�c�c

#define ALL_ENEMY_MAX	256//0 ~ 63 : Creep, 64 ~ 127 : TurboFastMichelle, 128 ~ 191 : Bloodsoak, 192 ~ 255 : Human�B���r�l�b�g�̓��X�g���B
#define ENEMY_MAX		 64//�G����̂l�`�w�B���C�t�����ȂǁA�G�̎�ނ��킸�s�������͏�̂l�`�w���g�����ƁB

	//�X�e�[�^�X
#define ENEMY01_LIFE		24//�̗́B
#define ENEMY01_ATTACK01	14//�̓�����B

#define ENEMY02_LIFE		40
#define ENEMY02_ATTACK01	10//�̓�����B
#define ENEMY02_ATTACK02	 5//�V���b�g�K���B1���ӂ�̃_���[�W�B���T�B

#define ENEMY03_LIFE		60
#define ENEMY03_ATTACK01	15//�̓�����B
#define ENEMY03_ATTACK02	 7//���[�U�[�U���B1/60�b������̃_���[�W�B

#define ENEMY04_LIFE		32
#define ENEMY04_ATTACK01	 1//�̓�����B
#define ENEMY04_ATTACK02	13//���C�t���_���[�W�B3�_�o�[�X�g�B


//----���̑��萔-------
//�����ꏊ�����Ɍ�����Ȃ����A�����ɏ����Ƃ����c

#define PI 3.141592654//�����Ƃ����鑤�ʂŖ��ɗ��B���͔ނ̑��݂Ɋ��ӂ��A���q���˂΂Ȃ�Ȃ��B



//-----enum enough----------------
//�A�ԂŐ��l�����悤�ȗv�f���������߂邼�c�\���̂Ƃ͏����Ⴄ�c�c�悭�l���ď����񂾂ȁc�c

enum GAME_STATE {
	GAMESTATE_TITLE = 0,
	GAMESTATE_MAIN,
	GAMESTATE_PAUSE,
};

enum STATE {//�����܂ł��u�v���C���[�́v��ԊǗ��萔�B
	STATE_NORMAL = 0,
	STATE_JUMP,
	STATE_ATTACK,
	STATE_DAMAGE,
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
	WAPON_BASE,//�����Ɏ�ނ����肷��K�v������Ƃ��ƁA�~�j�K���ȊO�͑�̂����
};

enum ITEM_KIND {
	ITEM_AMMO_BULLET = 0,	//�e��E2mm
	ITEM_AMMO_OXIGEN,		//�e��E�_�f
	ITEM_AMMO_ROCKET,		//�e��E���P�b�g
	ITEM_AMMO_SHELL,		//�e��E�V�F��
	ITEM_AMMO_POW_BULLET,	//�e��E.32
	ITEM_AMMO_GASOLINE,		//�e��E�R��
	ITEM_LIFE_POTION,		//�̗͉񕜁E��
	ITEM_LIFE_CUPSULE,		//�̗͉񕜁E��
	ITEM_MEDI_KIT,			//�x������
	ITEM_MISC_WAPONCASE,	//QC�g��+1
	ITEM_MISC_ARMYBELT,		//QC�g��+2
	ITEM_MISC_GUNHOLSTER,	//QC�g��+3
	ITEM_MISC_AMMOCASE,		//�������x1.5
	ITEM_MISC_AMMOBUG,		//�������x2.0
};

enum ENEMY_KIND {
	ENEMY_CREEP = 0,
	ENEMY_TFM,
	ENEMY_BLOODSOAK,
	ENEMY_HUMAN,
};

//----STRUCT------------------------
//�P�̗v�f�ɕK�v�Ȑ��l���\���̂ɂ��ď������ޏꏊ���c�`�b�c�ǂ����������Q��₪���āc�c

struct KEY {//���͂�S������Ă��Ă����g�����I�Ȃ�B�ł��邱�Ƃ͏��Ȃ�����ǗD�G�B
	int input[256];
};

struct PLAYER {
	STATE state;
	int gra[12];
	float hp;
	float hp_max;
	int AnimNo;
	int Anicount;
	float x;
	float y;
	float vel_x;
	float vel_y;
	bool isGrounded;
	int Jcount;
	int turn;
	bool lookup;
	bool lookdown;
	int fire;
	bool QC_Enabled[3];//QC�g�̗��p����p
};

struct ENEMY : public PLAYER {
	int gra[6];
	ENEMY_KIND kind;
	float hp;
	float atk01, atk02, atk03;
	int AnimNo;
	int Anicount;
	float x;
	float y;
	float vel_x;
	float vel_y;
	bool isDead = true;
	bool isNear;//������x�߂���true�ƂȂ�܂��B
	int turn;//0�͉E�B1�����B
};

struct WAPON {
	float x, y;
	int gra[24];
	int W_type;
	WAPON_KIND Wapon_kind;
	bool isFired;//���˂��Ă��Ԃ���true�ɂȂ��Ă�ƍK������ˁ`�B
	int Fire_gra[10];//�t�@�C�A�A�j���[�V������WAPON���ۗL�B�c�O�Ȃ���o���b�g�ł͂Ȃ��B
	int fireAni[ALL_BULLET_MAX];
	int firecount[ALL_BULLET_MAX];
	bool isFlash[ALL_BULLET_MAX];//�t���b�V�����Ĉ�x�����ł�����ˁB
	float sprash;//�g�U�x�������B
	bool have[WAPON_MAX];//������������Ă��邩�ǂ���(�e�핐��̔ԍ��ɑΉ�)
	int FastQC[4];//�N�C�b�N�`�F���W�p�z��BWAPON_KIND���i�[����B
	int NumQC[9];//�i���o�[�Ő؂�ւ���^�C�v�̂p�b�B����͎g��Ȃ��B
};

struct BULLET {
	int kind;
	float x, y;
	float angle;
	int turn;//0�͉E�A1�͍��ł����[���I
	bool hit;
	bool alive;
	float time;//��s���Ԃ������B
	float cool;
};

struct AMMO {//�c��e�����Ǘ����܂��B
	int Bullet;
	int Oxigen;
	int Rocket;
	int Shell;
	int Pow_Bullet;
	int Gasoline;
	int NowAmmo;//�������Ă�e���B
	int MaxAmmo;//�ő��������e���B
};

struct ITEM {
	int gra[16];
	int kind;
	float x, y;
	bool alive;
};

struct TILE {
	int gra[64];
	int StageBG_Use;
	int StageBG[12];
};


//-----�\���̃V���[�P�[�X----------
//�p�ӂ����\���͕̂K�������ő�������E����悤�ɂ��Ƃ��c�����Ŗ��ɗ����킩��˂�����ȁc�c

extern struct KEY Key;
extern struct PLAYER P;
extern struct ENEMY enemy01[ENEMY_MAX];
extern struct WAPON Wapon;
extern BULLET bul[ALL_BULLET_MAX];
extern struct AMMO Ammo;
extern struct ITEM Item;
extern struct TILE tile;


//----���̑������������Ă��Ăق�������----
//�����ɂ͑��ň�����悤�ɂ���v�f�������ꏊ���c�ǂ����Ŏg�������ȓz�͂Ƃ肠�����l�߂Ƃ��c�c

extern float deltatime;//Game.cpp�ɋL�q
extern int bulgra[4];
extern int Number[10];//��ԍ��摜�i�[�z���BGame.cpp�ɋL�q
extern int Ammo_No[3];
extern int Ammo_NoMax[3];
extern int Score;
extern int Score_No[8];