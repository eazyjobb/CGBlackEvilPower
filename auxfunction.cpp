#include "auxfunction.h"


/////////////////////////////////////////

int wmz::power_of_two(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}

void wmz::rotate(float &x, float &y, double A) { //向量(x,y)沿垂直平面的轴逆时针旋转fin（单位弧度 ）
	double xx = x * cos(A) - y * sin(A), yy = x * sin(A) + y * cos(A);
	x = xx; y = yy;
}

wmz::Point3 wmz::det(const wmz::Point3 &a, const wmz::Point3 &b) {
	return Point3(a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

double wmz::dot(const wmz::Point3 &a, const wmz::Point3 &b) {	//三维点积
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
