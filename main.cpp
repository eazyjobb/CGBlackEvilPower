#include <iostream>
#include <cstdlib>
#include <conio.h>

#define GLOBAL_VARIABLES_HERE
#include "Painter.h"
#include "Obj_reader.h"

//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
//�ر�dos����̨���ڣ�ò��������һ��windows����

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
			glEnable(GL_LIGHTING);          //���ù���  
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
		glEnable(GL_DEPTH_TEST); //������ȼ��  

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		test.lookat();
		gluLookAt(c_x, c_y, c_z, c_x + c_dx, c_y + c_dy, c_z + c_dz, c_hx, c_hy, c_hz);

		//����Ϊ��������

		glColor3f(1.0f, 0.0f, 0.0f); //x���ɫ
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(100, 0, 0);
		glEnd();

		glColor3f(0.0f, 1.0f, 0.0f); //y����ɫ
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 100, 0);
		glEnd();

		glColor3f(0.0f, 0.0f, 1.0f); //z����ɫ
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
				glEnable(GL_TEXTURE_2D);   // ���ö�ά����
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

void OnReShape(int w, int h) { //������������
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); //����ǰ����ָ��ΪͶӰģʽ  
	glLoadIdentity();
	if (0 != h) {
		GLfloat aspect = (float)w / (float)h;
		gluPerspective(60.0f, aspect, 1.0f, 3000.0f); //͸��ͶӰ  
	}
}


void SetupLights2() {
	GLfloat ambientLight[] = { 0.2f,  0.2f,  0.2f,  1.0f };//������  
	GLfloat diffuseLight[] = { 0.9f,  0.9f,  0.9f,  1.0f };//������  
	GLfloat specularLight[] = { 1.0f,  1.0f,  1.0f,  1.0f };//�����  
	GLfloat lightPos[] = { 50.0f, 80.0f, 60.0f, 1.0f };//��Դλ��  

	glEnable(GL_LIGHTING);          //���ù���  
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);    //���û�����Դ  
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);    //�����������Դ  
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);   //���þ����Դ  
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);        //���õƹ�λ��  
	glEnable(GL_LIGHT0);            //�򿪵�һ���ƹ�  


	glEnable(GL_COLOR_MATERIAL);    //���ò��ʵ���ɫ����  
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);  //ָ��������ɫ����  
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularLight); //ָ�����϶Ծ����ķ�Ӧ  
	glMateriali(GL_FRONT, GL_SHININESS, 100);           //ָ������ϵ��  
}



////////////////////////////////////////////////////////////////////////////////////////////

bool Mouse_flag; //����Ƿ������ż�
int Last_x, Last_y;	//��һ֡�������λ��

void processMousePassiveMotion(int x, int y) {
	Mouse_flag = false;
}

void processMouseActiveMotion(int x, int y)			//����ƶ��������������ʱ����
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

void processMouseActiveMotion_WHEEL(int x, int y)			//����ƶ������м�������ʱ����
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

void processMouseActiveMotion_stop(int x, int y)			//����ƶ������Ҽ�������ʱ����
{
	Mouse_flag = false;
}
#define  GLUT_WHEEL_UP 3           //������ֲ���  
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

// ���˵��¼�
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
	//option�����Ǵ��ݹ�����value��ֵ��
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
	case LOAD:
		break;
	case LIGHT_OPEN:
		glEnable(GL_LIGHTING);          //���ù���  
		break;
	case LIGHT_CLOSE:
		glDisable(GL_LIGHTING);			//�رչ��� 
		break;
	}
	renderScene();
}
//�˵�����
void createGLUTMenus() {

	int menu, sub_MODE, sub_SAVE, sub_LIGHT;

	sub_MODE = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("�߿�ģʽ", LINE_MODE);
	glutAddMenuEntry("��ģʽ", SURFACE_MODE);
	glutAddMenuEntry("������ͼģʽ", TEXTURE_MODE);
	glutAddMenuEntry("ˮ��ģʽ", CRYSTAL_MODE);

	sub_SAVE = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("OBJ�ļ�", OBJ_SAVE);
	glutAddMenuEntry("BMP�ļ�", BMP_SAVE);

	sub_LIGHT = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("��", LIGHT_OPEN);
	glutAddMenuEntry("��", LIGHT_CLOSE);


	menu = glutCreateMenu(processMenuEvents);
	glutAddSubMenu("����Ϊ", sub_SAVE);
	glutAddMenuEntry("��ȡ", LOAD);
	glutAddSubMenu("ģʽѡ��", sub_MODE);
	glutAddSubMenu("�ƹ�", sub_LIGHT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

//������Ӧ����
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
ʹ�÷�����
	���к��ڿ���̨�����ڰ���
	������Ӧд�ò��Ǻܺã����Ȱ����¿ո�
	wsad�ֱ�Ϊǰ�������ƶ������λ��
	qeΪ������ת���������
	uj�ֱ�Ϊ����������y�ᣨ���ϵ��ᣩ��ͬ�������ת
*/


int main(int argc, char *argv[]) {	
	std::cout << "test Start" << std::endl;

	test.gl_init(&argc, argv);

	test.test_init();

	obj_reader.load_from_file("jpg_resource/M9.obj", test);
	//obj_reader.load_from_file("jpg_resource/vanille_obj.obj", test);
	
	//obj_reader.load_from_file("jpg_resource/test.obj", test);

	glutReshapeFunc(OnReShape);	//���ڴ�С�ı�ʱ����
	glutDisplayFunc(renderScene);	//��Ҫ�ػ�ʱ����
	glutIdleFunc(renderScene);	//����ʱ����
	glutKeyboardFunc(processSpecialKeys);	//���̴���

	//glutSpecialFunc(onkey);

	SetupLights2();		//�ƹ����������	

	glutMouseFunc(processMouse);	//��괥��
	glutPassiveMotionFunc(processMousePassiveMotion);

	createGLUTMenus();


	std::cout << "load texture successfully ? :  " << texture.load_texture("test.bmp", "isokaze") << std::endl;
	std::cout << "load texture successfully ? :  " << texture.load_texture_all("jpg_resource/Tex_0009_1.jpg", "M9") << std::endl;
	
	
	glutMainLoop();
	
	std::cout << "test Finish" << std::endl;

	return 0;
}
