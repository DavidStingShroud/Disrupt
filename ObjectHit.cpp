#include <stdlib.h>
//a�̓v���C���[�Ab�͓G�Ƃ������g����������B���ꂼ��������ƁA
//ax1,ay1�����@�̍���Aax2,ay2�����@�̉E���A
//bx1,by1���G�̉E��A�@bx2,by2�����@�̉E���A���i�[����K�v������B

bool HitChecker(float ax1, float ay1, float ax2, float ay2, 
				float bx1, float by1, float bx2, float by2) {
	if (!(ax2<bx1 || ax1>bx2 || ay1 > by2 || ay2 < by1)) {
		return true;
	}
	else {
		return false;
	}
}

//a����z�܂ł̗�������肽���ꍇ�͂���B���������_���ɑΉ����Ă���̂ŁA
//���W��p�x�Ȃǂ̕��G�Ȍv�Z�ɂ��g����B
float RandAtoZ(float a, float z) {
	float ans = (float)rand() * (z - a) / RAND_MAX + a;
	return ans;
}