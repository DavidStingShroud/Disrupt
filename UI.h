extern int WaponWindowIm;
extern int ScoreWindowIm;
extern int LifeWindowIm;
extern int LifeBarIm;
extern int LifeBarDeadIm;
extern int ExpWindowIm;
extern int ExpLifeIm;
extern int ExpLifeDeadIm;
extern int WaponIconIm[64];
extern int Unable_EquipIm;
void UI_ReadAmmoNo(int num, int nummax);
void UI_ReadScore(int num);
void UI_LifeSetter(int life, int lifemax);
void UI_Init();
void UI_Release();
void UI_Update();
void UI_Rend();