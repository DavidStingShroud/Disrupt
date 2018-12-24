////////////////////////////////////////////////////////////////////////////
//																		  //
//			               Ms.Flint GunFire(c)							  //
//				"Global Game Manager" from Dans N' Dears				  //
//		           �޼ֳɻ�� ֳخ����� ÷�� � ���ֳ� ����÷�.			  //
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

//----������DEFINE���W------------------------

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

//----�e�֌W���Ԃ�����-----------------

#define WAPON_MAX 8				//�g�p���镐��̑���

	//�X�e�[�^�X
#define PISTOL_DAMAGE	  8.0f  //�U�@���@��
#define PISTOL_DERAY      0.7f	//���˃f�B���C�̂��ƁB
#define PISTOL_SPEED     14.0f	//�e���̂��ƁB
#define PISTOL_RANGE      6.7f	//�˒������̂��ƁB
#define PISTOL_RELOAD	 30.0f	//�����[�h���ԁB
#define PISTOL_MAKER		0	//���[�J�[�ԍ��B

#define HUMAN_DAMAGE	  2.0f
#define HUMAN_DERAY       0.3f
#define HUMAN_SPEED       3.0f
#define HUMAN_RANGE      30.3f
#define HUMAN_RELOAD	 10.0f
#define HUMAN_MAKER			1
#define HUMAN_BULMAX        6	//��ʂɑ��݂ł�����E�̐��B

#define ROCKET_DAMAGE	 24.0f
#define ROCKET_DERAY      1.5f
#define ROCKET_SPEED      1.5f
#define ROCKET_RANGE    250.0f
#define ROCKET_RELOAD	 52.0f//TODO : �����[�h�萔����Ɋe����̃����[�h�^�C������邱��
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
#define PYLO_DERAY        0.0f	//�f�[�^������Ă��Ȃ��悤�Ɍ�������̂́A���ۂ͑S���قȂ鏈����p�ӂ��悤��
#define PYLO_SPEED        0.0f	//�l���Ă��邾���B�Ƃ肠�����S���炢������������������[�X���Ă������̂ł́H
#define PYLO_RANGE        0.0f	//����������F�s�X�g��[�]�A�y�ʃV���b�g�K��[�]�A�X�i�C�p�[���C�t��[�]�A�~�j�K��[�]
#define PYLO_MAKER			0

//----�ő���m�ۂł���e�e�̐�------

#define BULLET_MAX       500
#define OXIGEN_MAX       333
#define ROCKET_MAX        48
#define SHELL_MAX         54
#define POWBULLET_MAX     30
#define GASOLINE_MAX     300

//----�~�j�K���p�̍��W�Ƃ����낢��-----------

struct MINIGUN_PARAM {
	float  ShotX[MINIGUN_BULMAX];//X���W
	float  ShotY[MINIGUN_BULMAX];//Y���W
	bool   ShotF[MINIGUN_BULMAX];//���˃t���O
	int    ShotT[MINIGUN_BULMAX];//���󎞊�
	int    ShotC[MINIGUN_BULMAX];//�J�E���g
	bool   ShotR[MINIGUN_BULMAX];//false=���Atrue=�E�B�~�j�K���p�̏����B�A�˂ł��镐��͂��̃^�C�v���̗p���悤�ˁB
	bool   ShotH[MINIGUN_BULMAX];//�q�b�g�t���O
}; 
extern struct MINIGUN_PARAM Minigun;
extern int Shotdelay;
extern int Shotdelaycount;			//������Q�́A���ˊԊu�Ɏg���Ă���܂��B

//----�G�֌W-----------

//  0 ~  63 : Creep, 
// 64 ~ 127 : Wasp, 
//128 ~ 191 : Brute, 
//192 ~ 255 : Bloodsoak,
//256 ~ 319 : Torture�B���r�l�b�g�̓��X�g���B
#define ALL_ENEMY_MAX	320
#define ENEMY_MAX		 64//�G����̂l�`�w�B���C�t�����ȂǁA�G�̎�ނ��킸�s�������͏�̂l�`�w���g�����ƁB

	//�X�e�[�^�X
#define ENEMY01_LIFE		27//�̗́B
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

//�{�X�̗͖ڈ��F
	//(Creep��B���ۂ͂����Ɠ|���Â炭�Ȃ�Ǝv����)
	// 500 : �s�X�g��200�����x
	// 730 : �V���b�g�K���S�e�A�s�X�g��30�����x
	//1000 : �V���b�g�K���S�e�A�X�i�C�p�[���C�t���S�e�A�����̃s�X�g���ł悤�₭�|����
	//3000 : �X�[�p�[�V���b�g�K���S�e�A�~�j�K���S�e�A����������̃��P�b�g�ł悤�₭�|����
	//3700 : �Ȃ�Ƃ��|����
#define BOSS01_LIFE		750


//----���̑��萔-------

#define PI 3.141592654//�����Ƃ����鑤�ʂŖ��ɗ��B���͔ނ̑��݂Ɋ��ӂ��A���q���˂΂Ȃ�Ȃ��B



//-----enum enough----------------

enum GAME_STATE {
	GAMESTATE_TITLE = 0,
	GAMESTATE_MAIN,
	GAMESTATE_PAUSE,
};

enum STATE {//�����܂ł��u�v���C���[�́v��ԊǗ��萔�B
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
	ITEM_MISC_QC01,	//QC�g��+1
	ITEM_MISC_QC02,		//QC�g��+2
	ITEM_MISC_QC03,	//QC�g��+3
	ITEM_MISC_AMMO15,		//�������x1.5
	ITEM_MISC_AMMO20,		//�������x2.0
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
	int turn;//0�͉E�B1�����B
	int AnimNo;
	int Anicount;
	bool isAlive;
	bool isGrounded;

};

struct KEY {//���͂�S������Ă��Ă����g�����I�Ȃ�B�ł��邱�Ƃ͏��Ȃ�����ǗD�G�B
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
	int Damagecount;//���G���ԗp
	bool QC_Enabled[3];//QC�g�̗��p����p
};

struct ENEMY : public OBJECT {
	ENEMY_KIND kind;
	float hp;
	float atk01, atk02, atk03;
	bool isNear;//������x�߂���true�ƂȂ�܂��B
};

struct WAPON : public OBJECT{
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

struct BULLET : public OBJECT {
	int kind;
	float angle;
	bool hit;
	float time;//��s���Ԃ������B
	float cool;
	bool isBorned;//��������t���O
	float ReloadTime;//�����[�h�ɂ����鎞��(�U���s�\����)
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


//-----�\���̃V���[�P�[�X----------

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


//----���̑������������Ă��Ăق�������----

extern float deltatime;
extern int Number[10];
extern int Ammo_No[3];
extern int Ammo_NoMax[3];
extern int Score;
extern int Score_No[8];