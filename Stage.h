extern int UseMap[15][20 + 1];
extern int Map01[15][20];
int STAGE_GetBlock(float x, float y);
bool STAGE_HitCheck(int x, int y);
int STAGE_GetBlockX(int x);
int STAGE_GetBlockY(int y);

void Stage_RendBG();

void Stage_Init();
void Stage_Release();
void Stage_Update();
void Stage_Rend();