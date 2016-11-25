#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <windows.h>

#define GLOBAL_VARIABLES_HERE
#include "Painter.h"
#include "Obj_reader.h"

//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
//关闭dos控制台窗口，貌似适用于一切windows程序

Painter test;
Texture texture;
Obj_reader obj_reader;

using std::cin;
using std::cout;
using std::endl;

double fin = 0.01;
int light_button = 0;

void renderScene(void);

void onkey() {
	char ch = _getch();

	switch (ch) {
	case 'w':
		test.go_straight();
		break;
	case 's':
		test.go_straight(-1);
		break;
	case 'a':
		test.go_right(-1);
		break;
	case 'd':
		test.go_right();
		break;
	case 'q':
		test.rotate_left(5);		
		break;
	case 'e':
		test.rotate_right(5);
		break;
	case 'u':
		test[0].rotate(0, fin*200, 0);
		break;
	case 'j':
		test[0].rotate(0, -fin*200, 0);
		break;
	case 'l':
		light_button ^= 1;
		if (light_button) {
			glEnable(GL_LIGHTING);          //启用光照  
		}
		else {
			glDisable(GL_LIGHTING);
		}
		break;
	case 'm':
		test.set_mode();	//switch mode
		printf("%d\n", test.get_mode());
		break;
	default:
		break;
	}	
}

double kk = 0;

void renderScene(void) {
	//onkey();
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST); //启用深度检测  

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		test.lookat();
		gluLookAt(c_x, c_y, c_z, c_x + c_dx, c_y + c_dy, c_z + c_dz, c_hx, c_hy, c_hz);

		//下面为画坐标轴

		glColor3f(1.0f, 0.0f, 0.0f); //x轴红色
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(100, 0, 0);
		glEnd();

		glColor3f(0.0f, 1.0f, 0.0f); //y轴绿色
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 100, 0);
		glEnd();

		glColor3f(0.0f, 0.0f, 1.0f); //z轴蓝色
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 100);
		glEnd();

		glColor3f(0.0f, 1.0f, 1.0f);
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(100, 100, 100);
		glEnd();
		
		glPushMatrix();{

			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			switch (test.get_mode()) {
			case 0:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				break;
			case 1:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				break;
			case 2:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glEnable(GL_TEXTURE_2D);   // 启用二维纹理
				break;
			case 3:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				cout << "in mode 3" << endl;
				glEnable(GL_BLEND);
				break;
			default: break;
			}
				
			glColor4f(0.8f, 0.9f, 0.9f, test.get_mode() == 3 ? 0.4f : 1.0f);

			glBegin(GL_POLYGON); {
				glVertex3f(0, 0, 0);
				glVertex3f(10, 10, 10);
				glVertex3f(0, 10, 10);
			} glEnd();

			test.show();

		}glPopMatrix();

		glutSwapBuffers();
		glFlush();

		std::cout << "Running...  " << std::endl;
	}
}

void OnReShape(int w, int h) { //画面调整后调用
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); //将当前矩阵指定为投影模式  
	glLoadIdentity();
	if (0 != h) {
		GLfloat aspect = (float)w / (float)h;
		gluPerspective(60.0f, aspect, 1.0f, 3000.0f); //透视投影  
	}
}


void SetupLights2() {
	GLfloat ambientLight[] = { 0.2f,  0.2f,  0.2f,  1.0f };//环境光  
	GLfloat diffuseLight[] = { 0.9f,  0.9f,  0.9f,  1.0f };//漫反射  
	GLfloat specularLight[] = { 1.0f,  1.0f,  1.0f,  1.0f };//镜面光  
	GLfloat lightPos[] = { 50.0f, 80.0f, 60.0f, 1.0f };//光源位置  

	glEnable(GL_LIGHTING);          //启用光照  
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);    //设置环境光源  
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);    //设置漫反射光源  
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);   //设置镜面光源  
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);        //设置灯光位置  
	glEnable(GL_LIGHT0);            //打开第一个灯光  


	glEnable(GL_COLOR_MATERIAL);    //启用材质的颜色跟踪  
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);  //指定材料着色的面  
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularLight); //指定材料对镜面光的反应  
	glMateriali(GL_FRONT, GL_SHININESS, 100);           //指定反射系数  
}



////////////////////////////////////////////////////////////////////////////////////////////

bool Mouse_flag; //鼠标是否正按着键
int Last_x, Last_y;	//上一帧鼠标所在位置

void processMousePassiveMotion(int x, int y) {
	Mouse_flag = false;
}

void processMouseActiveMotion(int x, int y)			//鼠标移动并且左键被按下时调用
{
	if (!Mouse_flag)
	{
		Mouse_flag = true;
		Last_x = x;
		Last_y = y;
		return;
	}

	test[0].rotate(0, fin * 200 * (x - Last_x)*0.07, 0);

	test[0].rotate(0, 0, -fin * 200 * (y - Last_y)*0.07);

	Last_x = x;
	Last_y = y;

}

void processMouseActiveMotion_WHEEL(int x, int y)			//鼠标移动并且中键被按下时调用
{
	if (!Mouse_flag)
	{
		Mouse_flag = true;
		Last_x = x;
		Last_y = y;
		return;
	}

	test.go_straight(-(y - Last_y)*0.07);

	Last_x = x;
	Last_y = y;

}

void processMouseActiveMotion_stop(int x, int y)			//鼠标移动并且右键被按下时调用
{
	Mouse_flag = false;
}
#define  GLUT_WHEEL_UP 3           //定义滚轮操作  
#define  GLUT_WHEEL_DOWN 4  
void processMouse(int button, int state, int x, int y)
{
	cout << "mouse " << button << endl;
	if (button == GLUT_LEFT_BUTTON)
	{
		Mouse_flag = false;
		glutMotionFunc(processMouseActiveMotion);
	}
	else if (button == GLUT_MIDDLE_BUTTON) {
		Mouse_flag = false;
		glutMotionFunc(processMouseActiveMotion_WHEEL);
		Mouse_flag = false;
		printf("MIDDLE\n");
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		glutMotionFunc(processMouseActiveMotion_stop);
		Mouse_flag = false;
		printf("RIGHT\n");
	}
}

// 各菜单事件
#define LINE_MODE 1
#define SURFACE_MODE 2
#define TEXTURE_MODE 3
#define CRYSTAL_MODE 4
#define OBJ_SAVE 5
#define BMP_SAVE 6
#define LOAD 7
#define LIGHT_OPEN 8
#define LIGHT_CLOSE 9


void processMenuEvents(int option) {
	//option，就是传递过来的value的值。
	switch (option)
	{
	case LINE_MODE:
		test.set_mode(0);
		break;
	case SURFACE_MODE:
		test.set_mode(1);
		break;
	case TEXTURE_MODE:
		test.set_mode(2);
		break;
	case CRYSTAL_MODE:
		test.set_mode(3);
		break;
	case OBJ_SAVE:
		break;
	case BMP_SAVE:
		break;
	case LOAD: {
		cout << "hehhehehehehehehe" << endl;
		
		//////////////////////////////////////////////////////////////////////
		OPENFILENAME ofn;
		TCHAR szFilePath[MAX_PATH] = { 0 };

		memset(szFilePath, 0, MAX_PATH);
		memset(&ofn, 0, sizeof(OPENFILENAME));

		ofn.lpstrTitle = TEXT("选择要加载的PE文件");
		ofn.hInstance = GetModuleHandle(NULL);
		//ofn.hwndOwner = hWndDlg;
		ofn.lpstrFile = szFilePath;
		ofn.lpstrFilter = TEXT("All File(*.*)\0*.*\0\0");
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrInitialDir = TEXT(",");
		ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;  //OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

		if (!GetOpenFileName(&ofn))
			return;
		
		std::wstring wstr(szFilePath);
		std::string str;

		str = wmz::WStringToString(wstr);

		std::wcout << wstr << endl;
		std::cout << str << endl;
		
		obj_reader.load_from_file(str.c_str(), test);
		cout << "load finish!" << endl;
	}
		break;
	case LIGHT_OPEN:
		glEnable(GL_LIGHTING);          //启用光照  
		break;
	case LIGHT_CLOSE:
		glDisable(GL_LIGHTING);			//关闭光照 
		break;
	}
	renderScene();
}
//菜单构成
void createGLUTMenus() {

	int menu, sub_MODE, sub_SAVE, sub_LIGHT;

	sub_MODE = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("线框模式", LINE_MODE);
	glutAddMenuEntry("面模式", SURFACE_MODE);
	glutAddMenuEntry("纹理贴图模式", TEXTURE_MODE);
	glutAddMenuEntry("水晶模式", CRYSTAL_MODE);

	sub_SAVE = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("OBJ文件", OBJ_SAVE);
	glutAddMenuEntry("BMP文件", BMP_SAVE);

	sub_LIGHT = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("开", LIGHT_OPEN);
	glutAddMenuEntry("关", LIGHT_CLOSE);


	menu = glutCreateMenu(processMenuEvents);
	glutAddSubMenu("保存为", sub_SAVE);
	glutAddMenuEntry("读取", LOAD);
	glutAddSubMenu("模式选择", sub_MODE);
	glutAddSubMenu("灯光", sub_LIGHT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

//按键响应部分
void processSpecialKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		test.go_straight();
		break;
	case 's':
		test.go_straight(-1);
		break;
	case 'a':
		test.go_right(-1);
		break;
	case 'd':
		test.go_right();
		break;
	}
}

/*
使用方法：
(此法已过期，仅用于debug，现在支持ui内的控制)
	运行后在控制台窗口内按键
	按键响应写得不是很好，请先按两下空格。
	wsad分别为前后左右移动摄像机位置
	qe为左右旋转摄像机朝向
	uj分别为中心物体绕y轴（向上的轴）不同方向的旋转
*/


int main(int argc, char *argv[]) {	
	std::cout << "test Start" << std::endl;

	test.gl_init(&argc, argv);

	test.test_init();

	//obj_reader.load_from_file("jpg_resource/M9.obj", test);
	//obj_reader.load_from_file("jpg_resource/vanille_obj.obj", test);
	
	//obj_reader.load_from_file("jpg_resource/test.obj", test);

	glutReshapeFunc(OnReShape);	//窗口大小改变时运行
	glutDisplayFunc(renderScene);	//需要重画时运行
	glutIdleFunc(renderScene);	//空闲时运行
	glutKeyboardFunc(processSpecialKeys);	//键盘触发

	//glutSpecialFunc(onkey);

	SetupLights2();		//灯光与材质设置	

	glutMouseFunc(processMouse);	//鼠标触发
	glutPassiveMotionFunc(processMousePassiveMotion);

	createGLUTMenus();


	std::cout << "load texture successfully ? :  " << texture.load_texture("test.bmp", "isokaze") << std::endl;
	std::cout << "load texture successfully ? :  " << texture.load_texture_all("jpg_resource/Tex_0009_1.jpg", "M9") << std::endl;
	
	
	glutMainLoop();
	
	std::cout << "test Finish" << std::endl;

	return 0;
}
