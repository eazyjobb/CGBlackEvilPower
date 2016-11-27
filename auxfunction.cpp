#include "auxfunction.h"
#include <windows.h>
#include <fstream>

/////////////////////////////////////////

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


std::wstring wmz::StringToWString(const std::string &str) {
	std::wstring wstr(str.length(), L' ');
	std::copy(str.begin(), str.end(), wstr.begin());
	return wstr;
}

//ֻ�������ֽ���string��
std::string wmz::WStringToString(const std::wstring &wstr) {
	std::string str(wstr.length(), ' ');
	std::copy(wstr.begin(), wstr.end(), str.begin());
	return str;
}



std::wstring wmz::myGetOpenFileName(std::wstring obj_type) {
	for (size_t i = 0; i < obj_type.size(); ++i) {
		if (obj_type[i] == '\a') obj_type[i] = 0;
	}
	
	OPENFILENAME ofn;

	TCHAR szFilePath[MAX_PATH] = { 0 };
	
	memset(szFilePath, 0, MAX_PATH*2);
	memset(&ofn, 0, sizeof(OPENFILENAME));
	
	
	ofn.lpstrTitle = TEXT("ѡ��Ҫ���ص��ļ�");
	ofn.hInstance = GetModuleHandle(NULL);
	//ofn.hwndOwner = hWndDlg;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFilter = (LPCWSTR)obj_type.c_str(); 
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.nMaxFile = MAX_PATH;
	//ofn.lpstrInitialDir = TEXT(",");
											  // һ��fopen������·����������ԭ�򼰽������ http://blog.csdn.net/zhanggongwu/article/details/6372995
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;  //OFN_EXPLORER | OFN_ALLOWMULTISELECT;  //OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	
	if (!GetOpenFileName(&ofn)) {
		std::cerr << "open file failed" << std::endl;
		return std::wstring(TEXT(""));
	}
	else
		std::cout << "open file successful" << ' ' << sizeof(ofn) << std::endl;
	
	return std::wstring(szFilePath);
}

std::wstring wmz::myGetSaveFileName(std::wstring obj_type) {

	for (size_t i = 0; i < obj_type.size(); ++i)	if (obj_type[i] == '\a') obj_type[i] = 0;

	TCHAR   szFilename[MAX_PATH] = TEXT("");
	OPENFILENAME   ofn = { 0 };

	ofn.lStructSize = sizeof(OPENFILENAME);
	/// �ڱ��洰�� ������������ ��������ʾ��
	ofn.lpstrFilter = obj_type.c_str();
	/// ��������·�����ļ�����szFilename���棻
	ofn.lpstrFile = szFilename;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER |OFN_ENABLEHOOK |OFN_HIDEREADONLY |OFN_NOCHANGEDIR |OFN_PATHMUSTEXIST;
	
	if (!GetSaveFileName(&ofn)) {
		std::cerr << "save file failed" << std::endl;
		return std::wstring(TEXT(""));
	}
	return std::wstring(szFilename);
}