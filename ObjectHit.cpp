#include <stdlib.h>
//aはプレイヤー、bは敵といった使い方をする。それぞれ解説すると、
//ax1,ay1＝自機の左上、ax2,ay2＝自機の右下、
//bx1,by1＝敵の右上、　bx2,by2＝自機の右下、を格納する必要がある。

bool HitChecker(float ax1, float ay1, float ax2, float ay2, 
				float bx1, float by1, float bx2, float by2) {
	if (!(ax2<bx1 || ax1>bx2 || ay1 > by2 || ay2 < by1)) {
		return true;
	}
	else {
		return false;
	}
}

//aからzまでの乱数を取りたい場合はこれ。浮動小数点数に対応しているので、
//座標や角度などの複雑な計算にも使える。
float RandAtoZ(float a, float z) {
	float ans = (float)rand() * (z - a) / RAND_MAX + a;
	return ans;
}