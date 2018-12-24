#include "Global.h"

struct PARTICLE Gore[PARTICLE_MAX];
struct PARTICLE GoreTall[PARTICLE_MAX2];

void Particle_Init()
{
	for (int i = 0; i < PARTICLE_MAX; i++) {
			Gore[i].Anicount = 0;

			if (i < 32) { Gore[i].AnimNo = 0; }				//  0 : 吹き出し

			if (i >= 32 && i <  64) { Gore[i].AnimNo =  4; }// 32 : 局所出血

			if (i >= 64 && i <  96) { Gore[i].AnimNo =  8; }// 64 : 爆発出血

			if (i >= 96 && i < 128) { Gore[i].AnimNo = 12; }// 96 : 飛散出血
	}
	for (int t = 0; t < PARTICLE_MAX2; t++) {
		GoreTall[t].Anicount = 0;
		GoreTall[t].AnimNo = 0;
	}
}

void Particle_Release()
{
}

void Particle_Update()
{
	for (int i = 0; i < PARTICLE_MAX; i++) {
		if (Gore[i].isAlive == true) {
			Gore[i].Anicount++;
			if (Gore[i].Anicount > 4) {
				Gore[i].AnimNo++;
				Gore[i].Anicount = 0;
			}

			if (i < 32) {
				if (Gore[i].AnimNo > 3) {
					Gore[i].AnimNo = 0;
					Gore[i].isAlive = false;
				}
			}

			else if (i >= 32 && i < 64) {
				if (Gore[i].AnimNo > 7) {
					Gore[i].AnimNo = 4;
					Gore[i].isAlive = false;
				}
			}

			else if (i >= 64 && i < 96) {
				if (Gore[i].AnimNo > 11) {
					Gore[i].AnimNo = 8;
					Gore[i].isAlive = false;
				}
			}

			else if (i >= 96 && i < 128) {
				if (Gore[i].AnimNo > 19) {
					Gore[i].AnimNo = 12;
					Gore[i].isAlive = false;
				}
			}


		}
	}

	for (int t = 0; t < PARTICLE_MAX2; t++) {
		if (GoreTall[t].isAlive == true) {
			GoreTall[t].Anicount++;
			if (GoreTall[t].Anicount > 4) {
				GoreTall[t].AnimNo++;
				GoreTall[t].Anicount = 0;
			}
			if (GoreTall[t].AnimNo > 7) {
				GoreTall[t].AnimNo = 0;
				GoreTall[t].isAlive = false;
			}
		}
	}
}

void Particle_Render()
{
	for (int i = 0; i < PARTICLE_MAX; i++) {
		if (Gore[i].isAlive == true) {
			if (Gore[i].turn == 0) {
				DrawGraphF(Gore[i].x - (BLOCK_SIZE / 2), Gore[i].y - (BLOCK_SIZE * 2), Gore[i].gra[Gore[i].AnimNo], TRUE);
			}
			else  {
				DrawTurnGraphF(Gore[i].x - (BLOCK_SIZE * 2), Gore[i].y - (BLOCK_SIZE * 2), Gore[i].gra[Gore[i].AnimNo], TRUE);
			}
		}
	}

	for (int t = 0; t < PARTICLE_MAX2; t++) {
		if (GoreTall[t].isAlive == true) {
			if (GoreTall[t].turn == 0) {
				DrawGraphF(GoreTall[t].x - (BLOCK_SIZE / 2), GoreTall[t].y - (BLOCK_SIZE * 4), GoreTall[t].gra[GoreTall[t].AnimNo], TRUE);
			}
			else {
				DrawTurnGraphF(GoreTall[t].x - (BLOCK_SIZE * 2), GoreTall[t].y - (BLOCK_SIZE * 4), GoreTall[t].gra[GoreTall[t].AnimNo], TRUE);
			}
		}
	}
}
