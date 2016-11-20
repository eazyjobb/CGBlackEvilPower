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

#define uint unsigned int

////////////////////////////////////////////////////texture 

Texture::Texture() {
	str_to_int.clear();
	int_to_str.clear();
}

GLuint Texture::load_texture_all(const char* filename, const char * texture_name) {
	GLuint texture_ID = 0;
	
	CImage img;
	if (!SUCCEEDED(img.Load(CString(filename)))) { //  ͨ��ͼƬ�ļ�����������
		std::cerr << "texture load error! " << std::endl;
	}
	int width = img.GetWidth();
	int height = img.GetHeight();
	unsigned char *pData = NULL;
	if (img.GetPitch()<0) //GetBits�����þ��ǻ�������λ��Ϣ������ָ�룬���λͼ�Ǵ��µ��ϣ� ��ָ�򻺳���ĩ�ˣ�����ָ�򻺳����׶ˡ���img.GetPitch ����������ж����ã�С�� 0 ָ��ĩ��
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

	// ���ļ������ʧ�ܣ�����
	FILE* pFile = fopen(file_name, "rb");
	if (pFile == 0)
		return 0;

	// ��ȡ�ļ���ͼ��Ŀ�Ⱥ͸߶�
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	// ����ÿ��������ռ�ֽ����������ݴ����ݼ����������ֽ���
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	// �����������ֽ��������ڴ�
	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0) {
		fclose(pFile);
		return 0;
	}

	// ��ȡ��������
	if (fread(pixels, total_bytes, 1, pFile) <= 0) {
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// �ھɰ汾��OpenGL��
	// ���ͼ��Ŀ�Ⱥ͸߶Ȳ��ǵ������η�������Ҫ��������
	// ���ﲢû�м��OpenGL�汾�����ڶ԰汾�����ԵĿ��ǣ����ɰ汾����
	// ���⣬�����Ǿɰ汾�����°汾��
	// ��ͼ��Ŀ�Ⱥ͸߶ȳ�����ǰOpenGLʵ����֧�ֵ����ֵʱ��ҲҪ��������
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
	const GLint new_height = 256; // �涨���ź��µĴ�СΪ�߳���������
	GLint new_line_bytes, new_total_bytes;
	GLubyte* new_pixels = 0;

	// ����ÿ����Ҫ���ֽ��������ֽ���
	new_line_bytes = new_width * 3;
	while (new_line_bytes % 4 != 0)
	++new_line_bytes;
	new_total_bytes = new_line_bytes * new_height;

	// �����ڴ�
	new_pixels = (GLubyte*)malloc(new_total_bytes);
	if (new_pixels == 0)
	{
	free(pixels);
	fclose(pFile);
	return 0;
	}

	// ������������
	gluScaleImage(GL_RGB,
	width, height, GL_UNSIGNED_BYTE, pixels,
	new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

	// �ͷ�ԭ�����������ݣ���pixelsָ���µ��������ݣ�����������width��height
	free(pixels);
	pixels = new_pixels;
	width = new_width;
	height = new_height;
	}
	}
	*/
	// ����һ���µ�������
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0) {
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// ���µ������������������������
	// �ڰ�ǰ���Ȼ��ԭ���󶨵������ţ��Ա��������лָ�
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

	// ֮ǰΪpixels������ڴ����ʹ��glTexImage2D�Ժ��ͷ�
	// ��Ϊ��ʱ���������Ѿ���OpenGL���б�����һ�ݣ����ܱ����浽ר�ŵ�ͼ��Ӳ���У�
	free(pixels);

	//index

	str_to_int[std::string(texture_name)] = texture_ID;
	int_to_str[texture_ID] = std::string(texture_name);

	return texture_ID;
}

GLuint Texture::getByStr(const char *str) {
	return str_to_int[std::string(str)];
}

/* ����load_texture
* ��ȡһ��BMP�ļ���Ϊ����
* ���ʧ�ܣ�����0������ɹ�������������
*/

// copy from http://www.cnblogs.com/crazyxiaom/articles/2073586.html,
// ���ڲ��ԣ�δ��װ

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

void Object::calc_norm_vector() {	//Ĭ�������ԣ�Ĭ��ÿ���涼����ȷ�����������㼰����
	if (changed || norm_vector.size() == 0) {
		changed = false;

		norm_vector.clear(); points_deg.clear();
		for (int i = 0; i < points.size(); ++i) {
			norm_vector.push_back(wmz::Point3());
			points_deg.push_back(0);
		}

		for (int i = 0; i < faces.size(); ++i) {
			//std::cout << points.size() << ' ' << faces[i][0] << ' ' << faces[i][1] << ' ' << faces[i][2] << std::endl;
			wmz::Point3 norm_v = wmz::det(points[faces[i][1]] - points[faces[i][0]], points[faces[i][2]] - points[faces[i][1]]);
			for (int j = 0; j < faces[i].size(); ++j) {
				points_deg[faces[i][j]] += 1;
				norm_vector[faces[i][j]] = norm_vector[faces[i][j]] + norm_v;
			}
		}

		for (int i = 0; i < points.size(); ++i) {
			norm_vector[i] = norm_vector[i] / points_deg[i];
			norm_vector[i].normalize();
		}
	}
}

void Object::show() { //δ����ĺϷ��������;���
	calc_norm_vector();

	glPushMatrix(); {
		/*
		std::cout << "==========================" << std::endl;
		std::cout << alpha << ' ' << gama << ' ' << beta << ' ' << x << ' ' << y << ' ' << z << std::endl;
		*/
		glRotatef(alpha, 1, 0, 0);
		glRotatef(gama, 0, 1, 0);
		glRotatef(beta, 0, 0, 1);

		glTranslatef(x, y, z);
		
		//glBindTexture(GL_TEXTURE_2D, test_tex); //�󶨲�������
		glBindTexture(GL_TEXTURE_2D, 2); //�󶨲�������
		GLfloat xx[] = {0, 1, 1, 0}, yy[] = {0, 0, 1, 1};
	
		for (uint i = 0; i < faces.size(); ++i) {
			//std::cout << "===================" << std::endl;
			glBegin(GL_POLYGON); {
				for (uint j = 0; j < faces[i].size(); ++j) {
					wmz::Point3 &u = points[faces[i][j]];
				//	std::cout << faces[i][j] << std::endl;
				//	u.write();
					int ind = faces[i][j];
						//���õ�ķ�����
						glNormal3f(norm_vector[ind].x, norm_vector[ind].y, norm_vector[ind].z);
							//printf("%d %f %f %f\n", ind, norm_vector[ind].x, norm_vector[ind].y, norm_vector[ind].z);
						//������
						if (ind < texturepos.size()) {
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
		glBindTexture(GL_TEXTURE_2D, 0);	//��Ĭ������
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

	glutCreateWindow("OpenGL͸��ͶӰ");

	glFrontFace(GL_CCW);	//��ʱ����Ʒ���Ϊ�������
	glEnable(GL_NORMALIZE);  //����ģ������ʱ�䷨������������һ����δ��֤Ч����

	glEnable(GL_BLEND);
	
	//�����ԣ��������Ϊ16384��������64λ windows10
}

void Painter::add_obj(const Object &u) {
	obj.push_back(u);
}

void Painter::test_init() {
	return;
//	obj.clear(); 
	int nt = obj.size();
	obj.push_back(Object());
	/*
	obj[0].add_point(wmz::Point3(20, 0, 0));
	obj[0].add_point(wmz::Point3(-20, 0, 20));
	obj[0].add_point(wmz::Point3(0, 0, -20));
	obj[0].add_point(wmz::Point3(0, 20, 0));

	obj[0].add_face(wmz::Face(0, 1, 2));	//20161102�޸��˶���˳���Ա���ȷ��ʾͼ�ε�������
	obj[0].add_face(wmz::Face(0, 3, 1));
	obj[0].add_face(wmz::Face(0, 2, 3));
	obj[0].add_face(wmz::Face(1, 3, 2));
	*/

//obj[0]: ��պ��� 
	obj[nt].add_point(wmz::Point3(-500, -250, 500));		
	obj[nt].add_point(wmz::Point3(-500, -250, -500));
	obj[nt].add_point(wmz::Point3(-500, 250, -500));
	obj[nt].add_point(wmz::Point3(-500, 250, 500));
	obj[nt].add_face(wmz::Face(0, 1, 2, 3));

///////////////////////////////////////////////////////
	return;
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


	for (int i = 0; i < obj.size(); ++i)
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
	//�۲�����ϵ����������ϵ�����෴��
	x += k * (-dz);
	y += k * dy;
	z += k * (dx);
}


void Painter::rotate_left(float fin) { // �ӽ���תfin����λ�Ƕȣ�
	fin = fin / 180.0 * PI;
	wmz::rotate(dx, dz, -fin);
}

void Painter::rotate_right(float fin) { // �ӽ���תfin����λ�Ƕȣ�
	fin = fin / 180.0 * PI;
	wmz::rotate(dx, dz, fin);
}

double angle = 0;
/*
ʵ���¼��
	1.ò�Ʊ�������ȫ���ƶ���ȫ����ת���ã�
	2.LINES �� LINES_STRIP ������ http://blog.csdn.net/xiaoxiaoyusheng2012/article/details/44197283
	3. ��OpenGL�����GL_TRIANGLE_STRIP�Ȼ������������е����ַ�ʽ http://blog.csdn.net/xiajun07061225/article/details/7455283
	4.��*������gluPerspective��gluLookAt�Ĺ�ϵ http://www.cnblogs.com/chengmin/archive/2011/09/12/2174004.html
		ps: 20161030�� ���ǻ�������ֵ���ʾ���˵�����
	5. ��θ�����Ļ��һ��ȡԭ�ռ����� http://blog.csdn.net/zhongjling/article/details/8488844
	6. OpenGL��ʹ��jpgͼƬ������ http://blog.sina.com.cn/s/blog_a72cd1960102v8nq.html��http://blog.csdn.net/jiese1990/article/details/8474333

	���ӹ����У����cpp����includeͬһ��ͷ�ļ��������ͷ�ļ����������������������ʱ���ض������\
��ֻ����һ��cpp��˵����޷���������������⡣������Ӧ���ú�+extern������⡣

*/

void Painter::on_reshape(int w, int h) {}

void Painter::show() {
	for (int i = 0; i < obj.size(); ++i) {
		if (mode == 3) glDepthMask(GL_FALSE);	//���뽫��Ȼ���������Ϊֻ������Ȼǰ�����Ʒ�Ḳ�Ǻ������Ʒ
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

int Painter::get_objnumber() const {
	return obj.size();
}

void Painter::test(int *argc, char **argv) {}