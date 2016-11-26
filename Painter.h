
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

EXTERN float c_x, c_y, c_z; // (向系统耦合屈服
EXTERN float c_dx, c_dy, c_dz;
EXTERN float c_hx, c_hy, c_hz;

const double PI = acos(-1);

#include "auxfunction.h"

class Object {	//物体类
public://构造与析构函数
	Object() ;
	~Object() ;
	Object(const char *str);

public:
	void clear();

	void move(float dx, float dy, float dz, int set_cod = 0);	//将物体移动(dx,dy,dz),是否为设置位置模式
	void rotate(float da, float dg, float db, int set_ang = 0); //沿x轴转动角度，y轴，z轴，是否是设定角度模式

	void add_point(const wmz::Point3 &u) ;	//为物品点集增加点
	void add_all_points(const wmz::Point3 &v, const wmz::Point3 &vt, const wmz::Point3 &vn) ;

	void add_face(const wmz::Face &u);		//为物品点集增加面
	//void draw_face() ;
	void show();							//向openGL提交绘画该物品的申请

	void calc_norm_vector(); //读取完整个物体后为每该物体计算其每个顶点的法向量

	size_t tmp_texture;
private:
	float x, y, z; //物体所在空间坐标
	float alpha, gama, beta; //物体沿x轴转动角度，y轴转动角度，z轴转动角度
	std::vector <wmz::Face > faces;	//物体每个面的点编号检索
	std::vector <wmz::Point3> points, norm_vector, texturepos;	//物体所有点 其法向量 其纹理贴图坐标
	std::vector <float > points_deg;	//物体每个点属于多少个面，用于计算法向量
	bool changed;	//距离最后一次法向量计算是否有改变
	std::string obj_name;
	
	std::vector <std::vector<int > >  group;	//一个物品可以有多个部分，每个部分可能都互相独立，可以移动？
};

class Texture {
public:
	Texture();
	GLuint load_texture(const char* file_name, const char * texture_name) ; //这个只能用于读bmp位图
	GLuint load_texture_all(const char* filename, const char * texture_name) ;	//这个貌似可以读取所有格式，待整合和测试
	GLuint getByStr(const char *str);


private:
	std::unordered_map<std::string, GLuint > str_to_int;
	std::unordered_map<GLuint, std::string > int_to_str;
};


class Painter {	//画图类
public://构造与析构函数
	Painter();
	~Painter();

public://公用接口
	//通过点列表与面列表表达多面体
	void test_init();
	void init(std::vector <wmz::Point3> & p, std::vector <wmz::Face> & f) ; //Painter类初始化函数，读入多面体
	void gl_init(int *argc, char **argv); //gl相关的初始化

	void add_obj(const Object &u) ;

	void on_reshape(int w, int h); //窗口尺寸变化后的回调函数 

	void go_straight(float k = 1);  //按方向向量向前走（k = -1 则向后
	void go_right(float k = 1);		//按方向向量向右走
	void rotate_left(float fin);	//视角左转
	void rotate_right(float fin);	//视角右转
	void rotate_updown(float fin);	//视角上下转，未实现
	void lookat();					//设置全局参数，以供设置摄像机位置

	void show();					//绘画painter具有的所有物品	

	void set_mode(int nxt_mode = -1);	//设定显示模式
	int get_mode() const ;				//获取当前显示模式

	size_t get_objnumber() const ;


	void test(int *argc, char **argv); //测试用函数

	Object& operator [](const int &rhs) { //回调第rhs个物体
		if (rhs < 0 || rhs >= (int)obj.size()) {
			std::cout << "obj operator [] out of range" << std::endl;
		}
		return obj[rhs];
	}

	
private: //私用接口

private: //私用数据
	std::vector <Object > obj;
	float x, y, z; // 摄像机位置
	float dx, dy, dz; // 摄像机朝向的方向向量
	float hx, hy, hz; // 摄像机头顶朝向
	int mode; //画模型模式，0：线框，1：面，2：纹理，3：水晶；
};

#endif

