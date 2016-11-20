
#ifndef PAINTER
#define PAINTER 

#include <vector>
#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include <unordered_map>

#define BMP_Header_Length 54

#ifdef GLOBAL_VARIABLES_HERE
#define EXTERN 
#else
#define EXTERN extern
#endif

EXTERN float c_x, c_y, c_z; // (��ϵͳ�������
EXTERN float c_dx, c_dy, c_dz;
EXTERN float c_hx, c_hy, c_hz;

const double PI = acos(-1);

#include "auxfunction.h"

class Object {	//������
public://��������������
	Object() ;
	~Object() ;
	Object(const char *str);

public:
	void clear();

	void move(float dx, float dy, float dz, int set_cod = 0);	//�������ƶ�(dx,dy,dz),�Ƿ�Ϊ����λ��ģʽ
	void rotate(float da, float dg, float db, int set_ang = 0); //��x��ת���Ƕȣ�y�ᣬz�ᣬ�Ƿ����趨�Ƕ�ģʽ

	void add_point(const wmz::Point3 &u) ;	//Ϊ��Ʒ�㼯���ӵ�
	void add_all_points(const wmz::Point3 &v, const wmz::Point3 &vt, const wmz::Point3 &vn) ;

	void add_face(const wmz::Face &u);		//Ϊ��Ʒ�㼯������
	//void draw_face() ;
	void show();							//��openGL�ύ�滭����Ʒ������

	void calc_norm_vector(); //��ȡ�����������Ϊÿ�����������ÿ������ķ�����

private:
	float x, y, z; //�������ڿռ�����
	float alpha, gama, beta; //������x��ת���Ƕȣ�y��ת���Ƕȣ�z��ת���Ƕ�
	std::vector <wmz::Face > faces;	//����ÿ����ĵ��ż���
	std::vector <wmz::Point3> points, norm_vector, texturepos;	//�������е� �䷨���� ��������ͼ����
	std::vector <float > points_deg;	//����ÿ�������ڶ��ٸ��棬���ڼ��㷨����
	bool changed;	//�������һ�η����������Ƿ��иı�
	std::string obj_name;
	
	std::vector <std::vector<int > >  group;	//һ����Ʒ�����ж�����֣�ÿ�����ֿ��ܶ���������������ƶ���
};

class Texture {
public:
	Texture();
	GLuint load_texture(const char* file_name, const char * texture_name) ; //���ֻ�����ڶ�bmpλͼ
	GLuint load_texture_all(const char* filename, const char * texture_name) ;	//���ò�ƿ��Զ�ȡ���и�ʽ�������ϺͲ���
	GLuint getByStr(const char *str);


private:
	std::unordered_map<std::string, GLuint > str_to_int;
	std::unordered_map<GLuint, std::string > int_to_str;
};


class Painter {	//��ͼ��
public://��������������
	Painter();
	~Painter();

public://���ýӿ�
	//ͨ�����б������б��������
	void test_init();
	void init(std::vector <wmz::Point3> & p, std::vector <wmz::Face> & f) ; //Painter���ʼ�����������������
	void gl_init(int *argc, char **argv); //gl��صĳ�ʼ��

	void add_obj(const Object &u) ;

	void on_reshape(int w, int h); //���ڳߴ�仯��Ļص����� 

	void go_straight(float k = 1);  //������������ǰ�ߣ�k = -1 �����
	void go_right(float k = 1);		//����������������
	void rotate_left(float fin);	//�ӽ���ת
	void rotate_right(float fin);	//�ӽ���ת
	void rotate_updown(float fin);	//�ӽ�����ת��δʵ��
	void lookat();					//����ȫ�ֲ������Թ����������λ��

	void show();					//�滭painter���е�������Ʒ	

	void set_mode(int nxt_mode = -1);	//�趨��ʾģʽ
	int get_mode() const ;				//��ȡ��ǰ��ʾģʽ

	int get_objnumber() const ;


	void test(int *argc, char **argv); //�����ú���

	Object& operator [](const int &rhs) { //�ص���rhs������
		if (rhs < 0 || rhs >= (int)obj.size()) {
			std::cout << "obj operator [] out of range" << std::endl;
		}
		return obj[rhs];
	}

private: //˽�ýӿ�

private: //˽������
	std::vector <Object > obj;
	float x, y, z; // �����λ��
	float dx, dy, dz; // ���������ķ�������
	float hx, hy, hz; // �����ͷ������
	int mode; //��ģ��ģʽ��0���߿�1���棬2������3��ˮ����
};

#endif

