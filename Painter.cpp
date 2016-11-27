#define GLUT_DISABLE_ATEXIT_HACK  
#define _CRT_SECURE_NO_WARNINGS

#include "Painter.h"

#include <algorithm>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <conio.h>
#include <string>
#include <atlimage.h>

#include <windows.h>
#include <GL/glut.h>

////////////////////////////////////////////////////texture 

Texture::Texture() {
	str_to_int.clear();
	int_to_str.clear();
}

GLuint Texture::load_texture_all(const char* filename, const char * texture_name) {
	std::cout << "loading from: " << filename << std::endl;

	GLuint texture_ID = 0;
	
	CImage img;
	if (!SUCCEEDED(img.Load(CString(filename)))) { //  通过图片文件名字来加在
		std::cerr << "texture load error! " << std::endl;
		return -1;
	}
	int width = img.GetWidth();
	int height = img.GetHeight();

	std::cout << width << ' ' << height << std::endl;

	unsigned char *pData = NULL;
	if (img.GetPitch()<0) //GetBits的作用就是获得纹理的位信息缓冲区指针，如果位图是从下到上， 则指向缓冲区末端，否则指向缓冲区首端。而img.GetPitch 就是起这个判断作用，小于 0 指向末端
		pData = (unsigned char *)img.GetBits() + (img.GetPitch()*(img.GetHeight() - 1));
	else
		pData = (unsigned char *)img.GetBits();

	glGenTextures(1, &texture_ID);
	if (texture_ID) {
		glBindTexture(GL_TEXTURE_2D, texture_ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pData);
	}

	str_to_int[std::string(texture_name)] = texture_ID;
	int_to_str[texture_ID] = std::string(texture_name);

	return texture_ID;
}


GLuint Texture::load_texture(const char* file_name, const char * texture_name) {
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLint last_texture_ID;
	GLuint texture_ID = 0;

	// 打开文件，如果失败，返回
	FILE* pFile = fopen(file_name, "rb");
	if (pFile == 0) {
		std::cerr << "open file failed! " << std::endl;
		return 0;
	}

	// 读取文件中图象的宽度和高度
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	// 计算每行像素所占字节数，并根据此数据计算总像素字节数
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	// 根据总像素字节数分配内存
	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0) {
		fclose(pFile);
		return 0;
	}

	// 读取像素数据
	if (fread(pixels, total_bytes, 1, pFile) <= 0) {
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// 在旧版本的OpenGL中
	// 如果图象的宽度和高度不是的整数次方，则需要进行缩放
	// 这里并没有检查OpenGL版本，出于对版本兼容性的考虑，按旧版本处理
	// 另外，无论是旧版本还是新版本，
	// 当图象的宽度和高度超过当前OpenGL实现所支持的最大值时，也要进行缩放
	/*
	{
	GLint max;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
	if (!power_of_two(width)
	|| !power_of_two(height)
	|| width > max
	|| height > max)
	{
	const GLint new_width = 256;
	const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
	GLint new_line_bytes, new_total_bytes;
	GLubyte* new_pixels = 0;

	// 计算每行需要的字节数和总字节数
	new_line_bytes = new_width * 3;
	while (new_line_bytes % 4 != 0)
	++new_line_bytes;
	new_total_bytes = new_line_bytes * new_height;

	// 分配内存
	new_pixels = (GLubyte*)malloc(new_total_bytes);
	if (new_pixels == 0)
	{
	free(pixels);
	fclose(pFile);
	return 0;
	}

	// 进行像素缩放
	gluScaleImage(GL_RGB,
	width, height, GL_UNSIGNED_BYTE, pixels,
	new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

	// 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
	free(pixels);
	pixels = new_pixels;
	width = new_width;
	height = new_height;
	}
	}
	*/
	// 分配一个新的纹理编号
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0) {
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// 绑定新的纹理，载入纹理并设置纹理参数
	// 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, last_texture_ID);

	// 之前为pixels分配的内存可在使用glTexImage2D以后释放
	// 因为此时像素数据已经被OpenGL另行保存了一份（可能被保存到专门的图形硬件中）
	free(pixels);

	//index

	str_to_int[std::string(texture_name)] = texture_ID;
	int_to_str[texture_ID] = std::string(texture_name);

	return texture_ID;
}

GLuint Texture::getByStr(const char *str) {
	return str_to_int[std::string(str)];
}

/* 函数load_texture
* 读取一个BMP文件作为纹理
* 如果失败，返回0，如果成功，返回纹理编号
*/

// copy from http://www.cnblogs.com/crazyxiaom/articles/2073586.html,
// 用于测试，未封装

int test_tex = 1;

/////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////// Object
Object::Object(): x(0), y(0), z(0), alpha(0), beta(0), gama(0),\
				  changed(false) {
	points.clear(); faces.clear();
	norm_vector.clear();
}

Object::~Object() {
	points.clear(); faces.clear();
}

Object::Object(const char *str) {
	clear();
	obj_name = std::string(str);
}

void Object::clear() {
	x = y = z = alpha = beta = gama = changed = 0;
	points.clear(); faces.clear(); norm_vector.clear();
}

void Object::move(float dx, float dy, float dz, int set_cod) {
	if (set_cod) {
		x = dx; y = dy; z = dz;
	}
	else {
		x += dx; y += dy; z += dz;
	}
}

void Object::rotate(float da, float dg, float db, int set_ang) {
	if (set_ang) {
		alpha = da; beta = db; gama = dg;
	}
	else {
		alpha += da; beta += db; gama += dg;
	}
	if (fabs(alpha) > 360) alpha -= 360;
	if (fabs(beta) > 360) beta -= 360;
	if (fabs(gama) > 360) gama -= 360;
}

void Object::add_point(const wmz::Point3 &u) {
	points.push_back(u);
	changed = true;
}

void Object::add_all_points(const wmz::Point3 &v, const wmz::Point3 &vt, const wmz::Point3 &vn) {
	points.push_back(v);
	texturepos.push_back(vt);
	norm_vector.push_back(vn);
}

void Object::add_face(const wmz::Face &u) {
	faces.push_back(u);
	changed = true;
}

void Object::calc_norm_vector() {	//默认完整性：默认每个面都是正确的且有三个点及以上
	if (changed || norm_vector.size() == 0) {
		changed = false;

		norm_vector.clear(); points_deg.clear();
		for (size_t i = 0; i < points.size(); ++i) {
			norm_vector.push_back(wmz::Point3());
			points_deg.push_back(0);
		}

		for (size_t i = 0; i < faces.size(); ++i) {
			//std::cout << points.size() << ' ' << faces[i][0] << ' ' << faces[i][1] << ' ' << faces[i][2] << std::endl;
			wmz::Point3 norm_v = wmz::det(points[faces[i][1]] - points[faces[i][0]], points[faces[i][2]] - points[faces[i][1]]);
			for (size_t j = 0; j < faces[i].size(); ++j) {
				points_deg[faces[i][j]] += 1;
				norm_vector[faces[i][j]] = norm_vector[faces[i][j]] + norm_v;
			}
		}

		for (size_t i = 0; i < points.size(); ++i) {
			norm_vector[i] = norm_vector[i] / points_deg[i];
			norm_vector[i].normalize();
		}
	}
}

void Object::show() { //未对面的合法性作检测和纠正
	calc_norm_vector();

	glPushMatrix(); {
		glRotatef(alpha, 1, 0, 0);
		glRotatef(gama, 0, 1, 0);
		glRotatef(beta, 0, 0, 1);

		glTranslatef(x, y, z);
		
		//glBindTexture(GL_TEXTURE_2D, test_tex); //绑定测试纹理
		glBindTexture(GL_TEXTURE_2D, tmp_texture); //绑定纹理 如果该编号未被绑定，这里绑了会占用编号的
		GLfloat xx[] = {0, 1, 1, 0}, yy[] = {0, 0, 1, 1};
	
		for (size_t i = 0; i < faces.size(); ++i) {
			//std::cout << "===================" << std::endl;
			glBegin(GL_POLYGON); {
				for (size_t j = 0; j < faces[i].size(); ++j) {
					wmz::Point3 &u = points[faces[i][j]];
				//	std::cout << faces[i][j] << std::endl;
				//	u.write();
					int ind = faces[i][j];
						//设置点的法向量
						glNormal3f(norm_vector[ind].x, norm_vector[ind].y, norm_vector[ind].z);
							//printf("%d %f %f %f\n", ind, norm_vector[ind].x, norm_vector[ind].y, norm_vector[ind].z);
						//绑定纹理
						if (ind >= 0 && (size_t)ind < texturepos.size()) {
							wmz::Point3 &v = texturepos[ind];
							glTexCoord2f(v.x, v.y);
							//std::cout << v.x << "  " << v.y << ' ' << ind << std::endl;
						}
						else
							glTexCoord2f(xx[j], yy[j]);
					glVertex3f(u.x, u.y, u.z);
				}
			} glEnd();
		}
		glBindTexture(GL_TEXTURE_2D, 0);	//绑定默认纹理
	} glPopMatrix();
}
/////////////////////////////////////////////////////////////////////////////////////// Patinter

Painter::Painter(): x(100.0), y(0.0), z(0.0), \
					dx(-1.0), dy(0.0), dz(0.0), hx(0.0), hy(1.0), hz(0.0),\
					mode(1) 
{
	obj.clear();
}

Painter::~Painter() {
}

void Painter::init(std::vector <wmz::Point3> & p, std::vector <wmz::Face> & f) { // just for testing
	obj.clear();
	obj.push_back(Object::Object());
	
	for each (wmz::Point3 i in p) obj[0].add_point(i);
	for each (wmz::Face i in f) obj[0].add_face(i);
}

void Painter::gl_init(int *argc, char **argv) {
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 480);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("OpenGL by黑恶势力");

	glFrontFace(GL_CCW);	//逆时针缠绕方向为面的正面
	glEnable(GL_NORMALIZE);  //开启模型缩放时其法向量的启动归一化（未验证效果）

	glEnable(GL_BLEND);
	
	//经测试，纹理最大为16384（本机、64位 windows10
}

void Painter::add_obj(const Object &u) {
	obj.push_back(u);
}

void Painter::test_init() {
	
//	obj.clear(); 
	int nt = obj.size();

	//obj.push_back(Object());
	/*
	obj[0].add_point(wmz::Point3(20, 0, 0));
	obj[0].add_point(wmz::Point3(-20, 0, 20));
	obj[0].add_point(wmz::Point3(0, 0, -20));
	obj[0].add_point(wmz::Point3(0, 20, 0));

	obj[0].add_face(wmz::Face(0, 1, 2));	//20161102修改了顶点顺序，以便正确表示图形的正方向
	obj[0].add_face(wmz::Face(0, 3, 1));
	obj[0].add_face(wmz::Face(0, 2, 3));
	obj[0].add_face(wmz::Face(1, 3, 2));
	*/

//obj[0]: 天空盒子 
	//obj[nt].add_point(wmz::Point3(-500, -250, 500));		
	//obj[nt].add_point(wmz::Point3(-500, -250, -500));
	//obj[nt].add_point(wmz::Point3(-500, 250, -500));
	//obj[nt].add_point(wmz::Point3(-500, 250, 500));
	//obj[nt].add_face(wmz::Face(0, 1, 2, 3));

///////////////////////////////////////////////////////
	
	nt = obj.size();
	obj.push_back(Object());

	obj[nt].add_point(wmz::Point3(-20, -10, -20));
	obj[nt].add_point(wmz::Point3(-20, -10, 20));
	obj[nt].add_point(wmz::Point3(20, -10, 20));
	obj[nt].add_point(wmz::Point3(20, -10, -20));
	// 0 1 2 3, 7 4 5 6
	obj[nt].add_point(wmz::Point3(-20, 40, 20));
	obj[nt].add_point(wmz::Point3(20, 40, 20));
	obj[nt].add_point(wmz::Point3(20, 40, -20));
	obj[nt].add_point(wmz::Point3(-20, 40, -20));

	obj[nt].add_face(wmz::Face(0, 3, 2, 1));
	obj[nt].add_face(wmz::Face(0, 1, 4, 7));
	obj[nt].add_face(wmz::Face(1, 2, 5, 4));
	obj[nt].add_face(wmz::Face(2, 3, 6, 5));
	obj[nt].add_face(wmz::Face(3, 0, 7, 6));
	obj[nt].add_face(wmz::Face(4, 5, 6, 7));


	for (size_t i = 0; i < obj.size(); ++i)
		obj[i].calc_norm_vector();

	//obj[0].rotate(30, 0, 30);

}

void Painter::lookat() {
	c_x = x; c_y = y; c_z = z;
	c_dx = dx; c_dy = dy; c_dz = dz;
	c_hx = hx; c_hy = hy; c_hz = hz;
}

void Painter::go_straight(float k) {
	x += k * dx;
	y += k * dy;
	z += k * dz;
}

void Painter::go_right(float k) {
	//观察坐标系与世界坐标系手性相反！
	x += k * (-dz);
	y += k * dy;
	z += k * (dx);
}


void Painter::rotate_left(float fin) { // 视角左转fin（单位角度）
	fin = fin / 180.0 * PI;
	wmz::rotate(dx, dz, -fin);
}

void Painter::rotate_right(float fin) { // 视角右转fin（单位角度）
	fin = fin / 180.0 * PI;
	wmz::rotate(dx, dz, fin);
}

double angle = 0;
/*
实验记录：
	1.貌似保留物体全局移动与全局旋转更好！
	2.LINES 与 LINES_STRIP 的区别 http://blog.csdn.net/xiaoxiaoyusheng2012/article/details/44197283
	3. 【OpenGL】理解GL_TRIANGLE_STRIP等绘制三角形序列的三种方式 http://blog.csdn.net/xiajun07061225/article/details/7455283
	4.（*）再议gluPerspective和gluLookAt的关系 http://www.cnblogs.com/chengmin/archive/2011/09/12/2174004.html
		ps: 20161030： 还是会碰到奇怪的显示不了的问题
	5. 如何根据屏幕上一点取原空间坐标 http://blog.csdn.net/zhongjling/article/details/8488844
	6. OpenGL中使用jpg图片做纹理 http://blog.sina.com.cn/s/blog_a72cd1960102v8nq.html、http://blog.csdn.net/jiese1990/article/details/8474333

	链接过程中，多个cpp可能include同一个头文件，因此在头文件里包含变量定义会造成链接时的重定义错误，\
宏只对于一个cpp来说因此无法避免这个链接问题。具体解决应该用宏+extern解决问题。

*/

void Painter::on_reshape(int w, int h) {}

void Painter::show() {
	for (size_t i = 0; i < obj.size(); ++i) {
		if (i + 1 != obj.size()) continue;
		if (mode == 3) glDepthMask(GL_FALSE);	//必须将深度缓冲区设置为只读，不然前面的物品会覆盖后面的物品
		else glDepthMask(GL_TRUE);
				
		obj[i].show();
	}
}

void Painter::set_mode(int nxt_mode) {
	if (nxt_mode != -1)	mode = nxt_mode;
	else mode = (mode + 1) % 4; //just for test
}

int Painter::get_mode() const {
	return mode;  
}

size_t Painter::get_objnumber() const {
	return obj.size();
}

void Painter::test(int *argc, char **argv) {}