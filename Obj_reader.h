
#ifndef OBJ_READER
#define OBJ_READER

#pragma once
// GLTEST.cpp : 定义控制台应用程序的入口点。
//
#define GLEW_STATIC

#include <iostream>
#include <string>       
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

#include <gl/glut.h>
#include <gl/glu.h>
#include <gl/gl.h>

#include "auxfunction.h"

//using namespace std;

class Obj_reader {
public:
	void clear() ; 
	int load_from_file(const char *str) ;
	int load_from_file(const char *str, Painter &painter);
	int save_to_file(const char *str);
private:
	std::vector<wmz::Point3 > v, vt, vn;
	std::vector<std::vector<int > > faces, normalvec, texturepos;
};

#endif