#include "auxfunction.h"


/////////////////////////////////////////

int wmz::power_of_two(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}

void wmz::rotate(float &x, float &y, double A) { //����(x,y)�ش�ֱƽ�������ʱ����תfin����λ���� ��
	double xx = x * cos(A) - y * sin(A), yy = x * sin(A) + y * cos(A);
	x = xx; y = yy;
}

wmz::Point3 wmz::det(const wmz::Point3 &a, const wmz::Point3 &b) {
	return Point3(a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

double wmz::dot(const wmz::Point3 &a, const wmz::Point3 &b) {	//��ά���
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
