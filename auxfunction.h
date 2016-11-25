#pragma once

#ifndef AUXFUNCTION
#define AUXFUNCTION

#include <iostream>
#include <vector>
#include <cmath>

namespace wmz {

	template <class T>
		inline T SQR(T x) { return x * x; }

	class Point3 {
	public:
		float x, y, z;
		Point3(float a = 0, float b = 0, float c = 0) : x(a), y(b), z(c) {}
		void set(float a = 0, float b = 0, float c = 0) { x = a; y = b; z = c; }
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
		float norm() const {
			return sqrt(SQR(x) + SQR(y) + SQR(z));
		}
		void normalize() {
			float l = this->norm();
			x /= l; y /= l; z /= l;
		}
		void write() const {
			std::cout << x << ' ' << y << ' ' << z << std::endl;
		}
	};
	class Face { //三角面以点的下标在点集里检索
	public:
		//int a, b, c;
		std::vector <int > index, vtind, vnind;	//点下标的检索，纹理贴图检索、法向量检索
		
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

		int & operator [](const int &rhs) {	//回调显示第rhs个点下标
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

		//正式请勿使用这两个构造函数，这两个构造函数仅用于测试
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

	void rotate(float &x, float &y, float A = 10);	//自定义二维向量旋转函数

	Point3 det(const Point3 &a, const Point3 &b); 	//三维叉积

	double dot(const Point3 &a, const Point3 &b);	//三维点积

	int power_of_two(int n); //检查一个数是否是2的整数次幂

};

#endif