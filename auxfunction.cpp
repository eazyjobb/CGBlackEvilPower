#include "auxfunction.h"


/////////////////////////////////////////

std::wstring wmz::StringToWString(const std::string &str)
{
	std::wstring wstr(str.length(), L' ');
	std::copy(str.begin(), str.end(), wstr.begin());
	return wstr;
}

//ֻ�������ֽ���string��
std::string wmz::WStringToString(const std::wstring &wstr)
{
	std::string str(wstr.length(), ' ');
	std::copy(wstr.begin(), wstr.end(), str.begin());
	return str;
}

int wmz::power_of_two(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}

void wmz::rotate(float &x, float &y, float A) { //����(x,y)�ش�ֱƽ�������ʱ����תfin����λ���� ��
	float xx = x * cos(A) - y * sin(A), yy = x * sin(A) + y * cos(A);
	x = (float)xx; y = (float) yy;
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
