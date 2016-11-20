#pragma once

#ifndef AUXFUNCTION
#define AUXFUNCTION

#include <vector>
#include <cmath>

namespace wmz {
#define SQR(x) ((x)*(x))

	class Point3 {
	public:
		float x, y, z;
		Point3(float a = 0, float b = 0, float c = 0) : x(a), y(b), z(c) {}
		Point3 operator + (const Point3 &rhs) const {
			return Point3(x + rhs.x, y + rhs.y, z + rhs.z);
		}
		Point3 operator - (const Point3 &rhs) const {
			return Point3(x - rhs.x, y - rhs.y, z - rhs.z);
		}
		Point3 operator * (float rhs) const {
			return Point3(rhs * x, rhs * y, rhs * z);
		}
		Point3 operator / (float rhs) const {
			return Point3(x / rhs, y / rhs, z / rhs);
		}
		double norm() const {
			return sqrt(SQR(x) + SQR(y) + SQR(z));
		}
		void normalize() {
			double l = this->norm();
			x /= l; y /= l; z /= l;
		}
		void write() const {
			std::cout << x << ' ' << y << ' ' << z << std::endl;
		}
	};
	class Face { //�������Ե���±��ڵ㼯�����
	public:
		//int a, b, c;
		std::vector <int > index, vtind, vnind;	//���±�ļ�����������ͼ����������������
		
		Face() {
			index.clear(); vtind.clear(); vnind.clear();
		}
		void add(int v = 0, int vt = 0, int vn = 0) {
			index.push_back(v); vtind.push_back(vt); vnind.push_back(vn);
		}
		int operator [](const int &rhs) const {
			if (rhs < 0 || rhs >= (int)index.size()) {
				//std::cout << "Out of Range!" << std::endl;
				//if (rhs == 0) index.push_back(0);
				return index[0];
			}
			return index[rhs];
		}

		int & operator [](const int &rhs) {	//�ص���ʾ��rhs�����±�
			if (rhs < 0 || rhs >= (int)index.size()) {
				//std::cout << "Out of Range!" << std::endl;
				if (rhs == 0) index.push_back(0);
				return index[0];
			}
			return index[rhs];
		}
		unsigned int size() const {
			return index.size();
		}

		//��ʽ����ʹ�����������캯�������������캯�������ڲ���
		Face(int a, int b, int c) {
			index.clear();
			index.push_back(a);
			index.push_back(b);
			index.push_back(c);
		}
		Face(int a, int b, int c, int d) {
			index.clear();
			index.push_back(a);
			index.push_back(b);
			index.push_back(c);
			index.push_back(d);
		}
	};

	void rotate(float &x, float &y, double A = 10);	//�Զ����ά������ת����

	Point3 det(const Point3 &a, const Point3 &b); 	//��ά���

	double dot(const Point3 &a, const Point3 &b);	//��ά���

	int power_of_two(int n); //���һ�����Ƿ���2����������

#undef SQR
};

#endif