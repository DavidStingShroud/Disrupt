#include "Global.h"

int StageBG_Use;
int StageBG[12];

int UseMap[MAP_HEIGHT][MAP_WIDTH + 1];//�T�C�Y�͍���30,��40

//�}�b�v�͂ق�A���ꂾ�A�G�N�Z���̃}�b�p�[�����邾��H����g���ĂˁB
static int Map01[MAP_HEIGHT][MAP_WIDTH] =
{

	{ -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1 },
	
	{ -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1 },

	{ -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1,  02,-1,-1,-1,-1,  -1,-1,02,02,-1,  -1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1,  02,-1,-1,-1,-1,  -1,-1,02,02,-1,  -1,-1,-1,-1,-1 },
	{ 00,00,00,00,00,  00,00,00,00,00,  00,00,00,00,00,  00,00,00,00,00 },

};

//{ 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01 },
//{ 01,01,01,01,01,  01,01,01,01,01,  01,01,01,01,01,  01,01,01,01,01 },
//{ 01,01,01,01,01,  01,01,01,01,01,  01,01,01,01,01,  01,01,01,01,01 },
//{ 01,01,01,01,01,  -1,01,01,01,01,  01,01,-1,01,-1,  01,01,01,01,01 },
//{ 01,01,01,-1,01,  -1,-1,01,01,01,  -1,01,-1,-1,-1,  01,01,01,-1,01 },
//
//{ 01,01,01,-1,01,  -1,-1,-1,01,01,  -1,-1,-1,01,-1,  -1,01,01,-1,01 },
//{ 05,-1,01,-1,-1,  -1,-1,-1,01,-1,  -1,-1,-1,-1,-1,  -1,01,-1,-1,01 },
//{ 05,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,01 },
//{ 05,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,01 },
//{ 05,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,01 },
//
//{ 05,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,01 },
//{ 05,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,01 },
//{ 05,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,01 },
//{ 05,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1,01 },
//{ 05,00,00,00,00,  00,00,00,00,00,  00,00,00,00,00,  00,00,00,00,01 },

int STAGE_GetBlock(float x, float y) {
	int bx = (int)x / BLOCK_SIZE;
	int by = (int)y / BLOCK_SIZE;

	return UseMap[by][bx];
}

bool STAGE_HitCheck(int x, int y) {
	int block = STAGE_GetBlock((int)x, (int)y);
	if (block != -1)return true;
	return false;
}


int STAGE_GetBlockX(int x) {

	int bx = x / BLOCK_SIZE;

	return bx * BLOCK_SIZE;
}

int STAGE_GetBlockY(int y) {

	int by = y / BLOCK_SIZE;

	return by * BLOCK_SIZE;
}



void Stage_Init()
{

	for (int y = 0; y <= MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			UseMap[y][x] = Map01[y][x];
		}
	}
	tile.StageBG_Use = tile.StageBG[0];
}

void Stage_Release()
{

}

void Stage_Update()
{

}

void Stage_RendBG() {
	DrawGraph(0, 0 - BLOCK_SIZE, tile.StageBG_Use, FALSE);
}

void Stage_Rend()
{
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			if (UseMap[y][x] >= 0) {
				DrawGraph(BLOCK_SIZE * x, BLOCK_SIZE * y, tile.gra[UseMap[y][x]], FALSE);
			}
		}
	}
}