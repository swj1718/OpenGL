#include <GL/glut.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <Windows.h>
#include<MMSystem.h>
#include "inc/fmod.hpp"
#pragma warning(disable :4996)
#pragma comment(lib,"winmm.lib")
#pragma comment (lib, "fmodex_vc.lib")
using namespace FMOD;
System* g_pSystem;
Sound* g_pSound[4];				//MAX_SOUND_TRACK는 사용할 음악 최대 개수
Channel* g_pChannel;
#define PI 3.141592
struct p{
	float x, y;
};


float before_mouse_x = 0;
float before_mouse_y = 0;

struct Location{

	float x;
	float y;
	float z;
	float mouse_x;
	float mouse_y;
	float mouse_z;
	float x_rot;
	float y_rot;
	
	float trans_x;
	float trans_y;
	float trans_z;

};

Location betman;

void crash(float x, float y);
#pragma warning(disable:4996)
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid Motion(int x, int y);
void SetupRC();
void DrawScene(void);
GLvoid Reshape(int w, int h);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
void Menu(int val);
void Mouse(int button, int state, int x, int y);
GLvoid Mouse_Move(int x, int y);
double drand(double low, double high);
GLubyte *pBytes;
BITMAPINFO *info[26]; // 비트맵 헤더 저장핛 변수
GLuint textures[26];
GLubyte * TexBits[26];
GLubyte * LoadDIBitmap(const char *filename, BITMAPINFO **info);
void bottom();
void wall_width();
void wall_6width();
void wall_hight();
void map();
void character();
void ai();
int high[6] = { 0 };
int location[6] = { 0 };
int locationz[6] = { 0 };
int color_rand[8] = { 0 };
int right_point = -500;
int left_point = -1500;
int up_point = -500;
int down_point = 100;
float wall_high = 300;
float L_arm = 0;
float L_arm_dir = 5;
float R_arm = 0;
float R_arm_dir = 5;
float L_leg = 0;
float L_leg_dir = 5;
float R_leg = 0;
float R_leg_dir = 5;
double mouse_pos_x = 0;		//마우스 시점
double mouse_pos_y = 0;
double character_mouse_pos_x = 0;		//마우스 시점
double character_mouse_pos_y = 0;
bool turn = false;
int count = 0;
float rotate = 0.0, zomZ = 0.0, yRotate = 0.0;
float T_y = 0;
int T_x = 0;
int Z_z = 0;
int Z_y = 0;
int count1 = 0;
int count2 = 0;
GLfloat T_z_2 = 1;
float backgruond_side = 3600;
float backgruond_high = 20 * 600;
int shape = 0;
float box_speed = 20;

float ai_trans_x = 0;
float ai_trans_y = 0;
float ai_speed_y = 5;
float ai_speed_x = 5;
float distance = 0;

int bgm_mode = 0;
GLfloat AmbientLight[] = { 0.2, 0.2, 0.2, 1 }; //조명의세기 , 주변조명
GLfloat diffuseLight[] = { 0.2, 0.2, 0.2, 1 }; // 산란 반사 조명
GLfloat specular[] = { 0.2, 0.2, 0.2, 1 }; // 거울반사 조명
GLfloat lightPos[][4] = { { -50, 20, -50, 1 }, { 50, 20, 50, 1 },
}; // 광원의 위치

//GLfloat gray[] = { 0.75, 0.75, 0.75, 1 };
GLfloat specref[] = { 1, 1, 1, 1 };

int main(int argc, char* argv[])
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < 25; i++)
	{
		color_rand[i] = rand() % 100 + 50;
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 600);
	glutCreateWindow("프로젝트");

	//glutCreateMenu(Menu);

	glutDisplayFunc(DrawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Mouse_Move);
	glutPassiveMotionFunc(Motion);

	glutTimerFunc(10, TimerFunction, 1);
	SetupRC();

	glutMainLoop();


}
void SetupRC()
{
	srand(time(0));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); //은면제거
	glEnable(GL_LIGHTING); // 조명을 사용하도록 설정
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, AmbientLight);//조명효과
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);//조명효과 반사광 각도 계산 0이면 조명이 평행하기 비친다.
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0);//조명효과 폴리곤의 양면이 조명을 받을지 결정
	glShadeModel(GL_SMOOTH); // 음영 모드 쉐이딩 설정

	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos[0]);

	glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos[1]);

	glEnable(GL_COLOR_MATERIAL); //컬러트랙킹
	//glColorMaterial(GL_FRONT, GL_EMISSION);//재질속성설정 키면 첨부터 밝
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);//재질속성설정 키면 첨부터 밝

	glMaterialfv(GL_FRONT, GL_SPECULAR, specref); //앞면
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	GLuint texture;
	glBindTexture(GL_TEXTURE_2D, 0);
	//이미지 로딩
	TexBits[0] = LoadDIBitmap("wall.bmp", &info[0]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 400, 416, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBindTexture(GL_TEXTURE_2D, 1);
	//이미지 로딩
	TexBits[1] = LoadDIBitmap("머리 위.bmp", &info[1]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 16, 16, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 2);
	//이미지 로딩
	TexBits[2] = LoadDIBitmap("머리 뒤.bmp", &info[2]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 16, 16, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 3);
	//이미지 로딩
	TexBits[3] = LoadDIBitmap("머리 앞.bmp", &info[1]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 16, 16, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[3]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 4);
	//이미지 로딩
	TexBits[4] = LoadDIBitmap("머리 오.bmp", &info[4]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 16, 16, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[4]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 5);
	//이미지 로딩
	TexBits[5] = LoadDIBitmap("머리 왼.bmp", &info[5]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 16, 16, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[5]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	glBindTexture(GL_TEXTURE_2D, 6);
	//이미지 로딩
	TexBits[6] = LoadDIBitmap("몸 앞.bmp", &info[6]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 16, 32, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[6]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 7);
	//이미지 로딩
	TexBits[7] = LoadDIBitmap("몸 뒤.bmp", &info[7]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 16, 32, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[7]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 8);
	//이미지 로딩
	TexBits[8] = LoadDIBitmap("몸 왼쪽.bmp", &info[8]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 8, 32, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[8]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 9);
	//이미지 로딩
	TexBits[9] = LoadDIBitmap("몸 오른쪽.bmp", &info[9]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 8, 32, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[9]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 10);
	//이미지 로딩
	TexBits[10] = LoadDIBitmap("팔 뒤.bmp", &info[10]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 8, 32, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[10]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 11);
	//이미지 로딩
	TexBits[11] = LoadDIBitmap("팔 앞.bmp", &info[11]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 8, 32, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[11]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 12);
	//이미지 로딩
	TexBits[12] = LoadDIBitmap("어깨.bmp", &info[12]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 8, 8, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[12]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 13);
	//이미지 로딩
	TexBits[13] = LoadDIBitmap("팔 안쪽면.bmp", &info[13]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 8, 32, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[13]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 14);
	//이미지 로딩
	TexBits[14] = LoadDIBitmap("팔 바깥쪽면.bmp", &info[14]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 8, 32, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[14]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 15);
	//이미지 로딩
	TexBits[15] = LoadDIBitmap("손.bmp", &info[15]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 8, 8, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[15]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 16);
	//이미지 로딩
	TexBits[16] = LoadDIBitmap("다리앞쪽.bmp", &info[16]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 8, 32, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[16]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 17);
	//이미지 로딩
	TexBits[17] = LoadDIBitmap("다리뒤쪽.bmp", &info[17]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 8, 32, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[17]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 18);
	//이미지 로딩
	TexBits[18] = LoadDIBitmap("다리위.bmp", &info[18]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 8, 8, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[18]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 19);
	//이미지 로딩
	TexBits[19] = LoadDIBitmap("다리 바깥쪽면.bmp", &info[19]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 8, 32, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[19]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 20);
	//이미지 로딩
	TexBits[20] = LoadDIBitmap("다리 안쪽면.bmp", &info[20]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 8, 32, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[20]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 21);
	//이미지 로딩
	TexBits[21] = LoadDIBitmap("발바닥.bmp", &info[21]);

	// 텍스쳐 설정 정의
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 8, 8, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TexBits[21]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 22);
	//이미지 로딩
	glClearColor(0, 0, 1, 1.0f);  // 배경색 설정
}
void DrawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();//1
	glLoadIdentity();

	//glRotatef(mouse_pos_x*0.8, 0, 1, 0);	//마우스 움직일시 회전 //작업

	//glTranslatef(right, up, back);
	//glTranslatef(-right, Z_y, Z_z - up); // 카메라 이동
	//glRotatef(T_x, 1, 0, 0);//90일떄 직각
	//glRotatef(T_y, 0, 1, 0);

	glTranslatef(0, -100, 600);
	character();	// 배트맨
	glRotatef(betman.x_rot, 0, 1, 0);//90일떄 직각
	glRotatef(betman.y_rot, 1, 0, 0);
	glTranslatef(betman.x, betman.y , betman.z ); // 캐릭터 이동
	
	map();
	if (bgm_mode == 0)	//	사운드
	{
		sndPlaySoundA("3D_Maze.wav", SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT), bgm_mode = 1;
		//g_pSystem->playSound(FMOD_CHANNEL_FREE, g_pSound[2], 0, &g_pChannel);
		bgm_mode = 1;
	}



	glPushMatrix();
	glTranslatef(0, -100, 600);


	glTranslatef(0, 0, -600);
	bottom();//밑면

	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1000 + ai_trans_x, -200, -3900 + ai_trans_y);
	ai();
	glPopMatrix();

	glPushMatrix();
	//마우스 움직일시 회전
	
	glPushMatrix();
	
	
	glPopMatrix();

	//glColor3f(1, 0, 0);
	glPopMatrix();


	glPopMatrix();



	glutSwapBuffers();
}
GLvoid Reshape(int w, int h)
{
	// 윈도우의 폭과 넓이 설정
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION); //투영모드
	glLoadIdentity();
	gluPerspective(60, 1200 / 600, 1.0, 10000.0);// 좌우 크기 600 600
	glTranslatef(0.0, 0.0, -2000.0);//시점 조절 //-2000 3인칭 % 200 1인칭
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutSwapBuffers();


}

void SpecialKeyboard(int key, int x, int y)
{

}


void TimerFunction(int value)//작업
{

	ai_trans_y += ai_speed_y;
	if (ai_trans_y > 1200)
	{
		ai_speed_y = 0;
		ai_trans_x += ai_speed_x;
		if (ai_trans_x > 2000)
		{
			ai_speed_x *= -1;
		}
		if (ai_trans_x < 0)
		{
			ai_speed_x *= -1;
			ai_speed_y = 5;
			ai_speed_y *= -1;
		}
	}
	if (ai_trans_y < 0)
	{

		ai_speed_y *= -1;
	}
	distance = sqrt(((ai_trans_x - 1000) - betman.x)* ((ai_trans_x - 1000) - betman.x) + ((ai_trans_y - 3900) - (betman.z - 100))* ((ai_trans_y - 3900) - (betman.z - 100)));

	if (distance < 500)
	{
		betman.x = 0;
		betman.z = 0;
	}
	if (turn == true)
	{
		T_y = T_y - 0.5;
	}
	if (betman.z < -50)		// 이 밑에 부분이 너무 이상해 우진아
	{

		betman.y += 10;
	}
	if (betman.y <-1000)
	{
		betman.x = 0;
		betman.y = 0;
		betman.z = 0;

	}
	/*if (right<-500 && up<100 && up>-500)//glTranslatef(right,-400+down, up-100); //입구쪽
	{
	right = -500;
	}
	if (right>500 && up<100 && up>-500)//입구쪽
	{
	right = 500;
	}*/
	for (float i = 0; i < 3; i++)//1~3구역 왼쪽
	{
		crash(i, 0);
	}

	for (float i = -2; i > -5; i--)//1~3구역 오른쪽
	{
		crash(i, 0);
	}

	for (float i = -3; i < 2; i++)//4~6구역 왼쪽
	{
		crash(i, 2);
	}
	for (float i = -4; i < 2; i++)//7~9구역 왼쪽
	{
		crash(i, 3);
	}
	for (float i = 1; i < 3; i++)//16~18구역 왼쪽
	{
		crash(i, 5);
	}
	for (float i = -1; i >-4; i--)
	{
		crash(i, 5);
	}
	for (float i = -1; i < 0; i++)//19~21구역 왼쪽
	{
		crash(i, 6);
	}

	for (float i = -1; i < 2; i++)//22~24구역 왼쪽
	{
		crash(i, 7);
	}
	for (float i = -3; i >-5; i--)
	{
		crash(i, 7);
	}
	for (float i = -1; i < 0; i++)//25~27구역 왼쪽
	{
		crash(i, 8);
	}

	for (float i = -3; i < 3; i++)//28~30구역 왼쪽
	{
		crash(i, 9);
	}
	for (float i = 0; i < 2; i++)//34~36구역 왼쪽
	{
		crash(i, 11);
	}
	for (float i = -2; i >-5; i--)
	{
		crash(i, 11);
	}
	for (float i = 0; i < 2; i++)//37~39구역 왼쪽
	{
		crash(i, 12);
	}
	for (float i = 1; i < 2; i++)//40~42구역 왼쪽
	{
		crash(i, 13);
	}
	for (float i = -1; i < 0; i++)
	{
		crash(i, 13);
	}
	for (float i = -3; i < -2; i++)
	{
		crash(i, 13);
	}
	for (float i = 1; i < 2; i++)//43~45구역 왼쪽
	{
		crash(i, 14);
	}
	for (float i = -3; i < 2; i++)//46~48구역 왼쪽
	{
		crash(i, 15);
	}
	for (float i = -3; i < -2; i++)//49~51
	{
		crash(i, 16);
	}
	for (float i = 1; i < 3; i++)//52~54구역 왼쪽
	{
		crash(i, 17);
	}
	for (float i = -1; i >-5; i--)
	{
		crash(i, 17);
	}
	for (float i = -4; i < 2; i++)//58~60구역 왼쪽
	{
		crash(i, 19);
	}


	if (betman.x <= -3450)//왼쪽 끝 한줄짜리 벽
	{
		betman.x = -3450;
	}
	if (betman.x >= 3450)//오른쪽 끝 한줄짜리 벽
	{
		betman.x = 3450;
	}

	glutPostRedisplay();             // 화면 재 출력
	glutTimerFunc(10, TimerFunction, 1);         // 타이머함수 재 설정
}
//int right_point = -500;
//int left_point = -1500;
//int up_point = -500;
//int down_point = 100;
void crash(float x, float y)
{
	if (betman.x>right_point - 1000 * x - 30 + 50 && betman.x<right_point - 1000 * x + 50 && betman.z<down_point - 600 * y && betman.z>up_point - 600 * y)//벽왼쪽
	{
		betman.x = right_point + 50 - 1000 * x;

	}
	if (betman.x<left_point - 1000 * x + 30 - 50 && betman.x>left_point - 1000 * x - 50 && betman.z<down_point - 600 * y && betman.z>up_point - 600 * y)//벽왼쪽
	{
		betman.x = left_point - 50 - 1000 * x;

	}
	if (betman.x<right_point - 1000 * x  && betman.x>left_point - 1000 * x  && betman.z>up_point - 600 * y - 50 && betman.z<up_point - 600 * y + 30 - 50)//벽위쪽
	{
		betman.z = up_point - 50 - 600 * y;

	}
	if (betman.x<right_point - 1000 * x  && betman.x>left_point - 1000 * x   && betman.z<down_point - 600 * y + 50 && betman.z>down_point - 600 * y - 30 + 50)//벽아래쪽
	{
		betman.z = down_point + 50 - 600 * y;

	}
}


void bottom()
{
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	{
		//바닥 아랫면
		glVertex3d(-backgruond_side, -450, 0);
		glVertex3d(backgruond_side, -450, 0);
		glVertex3d(backgruond_side, -450, -backgruond_high);
		glVertex3d(-backgruond_side, -450, -backgruond_high);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		//바닥 윗면

		glVertex3d(-backgruond_side, -350, 0);
		glVertex3d(backgruond_side, -350, 0);
		glVertex3d(backgruond_side, -350, -backgruond_high);
		glVertex3d(-backgruond_side, -350, -backgruond_high);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		//바닥 앞면

		glVertex3d(-backgruond_side, -450, 0);
		glVertex3d(-backgruond_side, -350, 0);
		glVertex3d(backgruond_side, -350, 0);
		glVertex3d(backgruond_side, -450, 0);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		//바닥 뒷면
		glVertex3d(-backgruond_side, -450, -backgruond_high);
		glVertex3d(backgruond_side, -450, -backgruond_high);
		glVertex3d(backgruond_side, -350, -backgruond_high);
		glVertex3d(-backgruond_side, -350, -backgruond_high);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		//바닥 왼쪽면
		glVertex3d(-backgruond_side, -450, -backgruond_high);
		glVertex3d(-backgruond_side, -350, -backgruond_high);
		glVertex3d(-backgruond_side, -350, 0);
		glVertex3d(-backgruond_side, -450, 0);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		//바닥 오른쪽
		glVertex3d(backgruond_side, -450, -backgruond_high);
		glVertex3d(backgruond_side, -450, 0);
		glVertex3d(backgruond_side, -350, 0);
		glVertex3d(backgruond_side, -350, -backgruond_high);
	}
	glEnd();


}

void wall_width()
{

	glPushMatrix();
	glColor3f(0, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);//건물 앞면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-500, -450, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(500, -450, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(500, wall_high, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(-500, wall_high, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);//건물 뒷면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-500, -450, -100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-500, wall_high, -100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(500, wall_high, -100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(500, -450, -100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);//건물 왼쪽면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-500, -450, -100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-500, -450, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(-500, wall_high, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(-500, wall_high, -100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);//건물 오른쪽면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(500, -450, -100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(500, wall_high, -100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(500, wall_high, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(500, -450, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);//건물 아랫면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-500, -450, -100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-500, -450, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(500, -450, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(500, -450, -100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);//건물 윗면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-500, wall_high, -100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-500, wall_high, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(500, wall_high, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(500, wall_high, -100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


}

void wall_6width()
{

	glPushMatrix();
	glColor3f(0, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);//건물 앞면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-500, -450, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(500, -450, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(500, wall_high, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(-500, wall_high, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);//건물 뒷면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-500, -450, -600);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-500, wall_high, -600);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(500, wall_high, -600);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(500, -450, -600);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);//건물 왼쪽면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-500, -450, -600);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-500, -450, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(-500, wall_high, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(-500, wall_high, -600);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);//건물 오른쪽면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(500, -450, -600);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(500, wall_high, -600);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(500, wall_high, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(500, -450, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);//건물 아랫면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-500, -450, -600);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-500, -450, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(500, -450, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(500, -450, -600);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);//건물 윗면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-500, wall_high, -600);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-500, wall_high, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(500, wall_high, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(500, wall_high, -600);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


}

void wall_hight()
{

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glColor3f(0, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);//건물 앞면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-500, -450, -100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-500, -450, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(-500, wall_high, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(-500, wall_high, -100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);//건물 뒷면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(500, -450, -100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(500, wall_high, -100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(500, wall_high, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(500, -450, 0);

	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);//건물 왼쪽면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-500, -450, -100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-500, wall_high, -100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(500, wall_high, -100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(500, -450, -100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);//건물 오른쪽면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-500, -450, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(500, -450, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(500, wall_high, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(-500, wall_high, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);//건물 아랫면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-500, -450, -100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-500, -450, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(500, -450, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(500, -450, -100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);//건물 윗면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-500, wall_high, -100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-500, wall_high, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(500, wall_high, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(500, wall_high, -100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);


	glPopMatrix();

}

GLvoid Mouse(int button, int state, int x, int y)
{



	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
	}



}

GLvoid Mouse_Move(int x, int y)
{

}

GLvoid Motion(int x, int y)
{
	betman.mouse_x =  -600 + x;
	betman.mouse_y = 300 - y;

	if (betman.mouse_x < before_mouse_x)
	{
		betman.x_rot -= 1;
		if (betman.x_rot <= -360)
			betman.x_rot = -1;
		betman.trans_z = cos(betman.x_rot*(PI / 180.0)) * 0.8;
		betman.trans_x = -sin(betman.x_rot*(PI / 180.0)) * 0.8;
	}
	if (betman.mouse_x > before_mouse_x)
	{
		betman.x_rot += 1;
		if (betman.x_rot >= 360)
			betman.x_rot = 1;
		betman.trans_z = cos(betman.x_rot*(PI / 180.0)) * 0.8;
		betman.trans_x = -sin(betman.x_rot*(PI / 180.0)) * 0.8;
	}


	before_mouse_x = betman.mouse_x;
	before_mouse_y = betman.mouse_y;
	printf("trans_x : %f, trans_z : %f \n", betman.trans_x, betman.trans_z);
	glutPostRedisplay();
}


GLvoid Keyboard(unsigned char key, int x, int y)//작업
{

	if (key == '1')//1인칭
	{

		glMatrixMode(GL_PROJECTION); //투영모드
		glLoadIdentity();
		gluPerspective(60, 1200 / 600, 1.0, 10000.0);// 좌우 크기 600 600
		glTranslatef(0.0, 0.0, 200.0);//시점 조절 //-2000 3인칭 % 200 1인칭
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glutSwapBuffers();

	}
	if (key == '2')//3인칭
	{

		glMatrixMode(GL_PROJECTION); //투영모드
		glLoadIdentity();
		gluPerspective(60, 1200 / 600, 1.0, 10000.0);// 좌우 크기 600 600
		glTranslatef(0.0, 0.0, -2000.0);//시점 조절 //-2000 3인칭 % 200 1인칭
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glutSwapBuffers();

	}
	if (key == 'z') // xz평면으로보기
	{
		T_x = 90;
		Z_z = 400;
		Z_y = -800;
	}
	if (key == 'x') //xy평면으로보기
	{
		T_x = 0;
		Z_z = 0;
		Z_y = 0;
	}
	if (key == 'g'){
		count1++;

		if (count1 == 1)
		{
			glEnable(GL_LIGHT0);
		}
		else if (count1 >= 2)
		{
			count1 = 0;
			glDisable(GL_LIGHT0);

		}

	}
	if (key == 'h'){
		count2++;

		if (count2 == 1)
		{
			glEnable(GL_LIGHT1);
		}
		else if (count2 >= 2)
		{
			count2 = 0;
			glDisable(GL_LIGHT1);

		}

	}
	if (key == 'a')
	{
		betman.trans_z = cos((betman.x_rot + 90)*(PI / 180)) * 10;
		betman.trans_x = -sin((betman.x_rot + 90)*(PI / 180)) * 10;
		betman.x = betman.x - betman.trans_x;
		betman.z = betman.z - betman.trans_z;
	}
	if (key == 'd')
	{
		betman.trans_z = cos((betman.x_rot + 90)*(PI / 180.0)) * 10;
		betman.trans_x = -sin((betman.x_rot + 90)*(PI / 180.0)) * 10;
		betman.x = betman.x + betman.trans_x;
		betman.z = betman.z + betman.trans_z;
	}

	if (key == 'w')
	{

		betman.z += betman.trans_z+10; 
		betman.x += betman.trans_x;

		L_arm += L_arm_dir;
		if (L_arm >= 45)
			L_arm_dir *= -1;
		if (L_arm <= -45)
			L_arm_dir *= -1;

		R_arm += R_arm_dir;
		if (R_arm >= 45)
			R_arm_dir *= -1;
		if (R_arm <= -45)
			R_arm_dir *= -1;

		L_leg += L_leg_dir;
		if (L_leg >= 45)
			L_leg_dir *= -1;
		if (L_leg <= -45)
			L_leg_dir *= -1;

		R_leg += R_leg_dir;
		if (R_leg >= 45)
			R_leg_dir *= -1;
		if (R_leg <= -45)
			R_leg_dir *= -1;

	}
	if (key == 's')
	{
		betman.z = betman.z - betman.trans_z;
		betman.x = betman.x - betman.trans_x;

		L_arm += L_arm_dir;
		if (L_arm >= 45)
			L_arm_dir *= -1;
		if (L_arm <= -45)
			L_arm_dir *= -1;

		R_arm += R_arm_dir;
		if (R_arm >= 45)
			R_arm_dir *= -1;
		if (R_arm <= -45)
			R_arm_dir *= -1;

		L_leg += L_leg_dir;
		if (L_leg >= 45)
			L_leg_dir *= -1;
		if (L_leg <= -45)
			L_leg_dir *= -1;

		R_leg += R_leg_dir;
		if (R_leg >= 45)
			R_leg_dir *= -1;
		if (R_leg <= -45)
			R_leg_dir *= -1;
	}
	
	glutPostRedisplay();
}

GLubyte * LoadDIBitmap(const char *filename, BITMAPINFO **info)
{
	FILE *fp;
	GLubyte *bits;
	int bitsize, infosize;
	BITMAPFILEHEADER header;
	// 바이너리 읽기 모드로 파일을 연다
	if ((fp = fopen(filename, "rb")) == NULL)
		return NULL;
	// 비트맵 파일 헤더를 읽는다.
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		fclose(fp);
		return NULL;
	}
	// 파일이 BMP 파일인지 확인핚다.
	if (header.bfType != 'MB') {
		fclose(fp);
		return NULL;
	}
	// BITMAPINFOHEADER 위치로 갂다.
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	// 비트맵 이미지 데이터를 넣을 메모리 핛당을 핚다.
	if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL) {
		fclose(fp);
		exit(0);
		return NULL;
	}
	// 비트맵 인포 헤더를 읽는다.
	if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	// 비트맵의 크기 설정
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth *
		(*info)->bmiHeader.biBitCount + 7) / 8.0 *
		abs((*info)->bmiHeader.biHeight);
	// 비트맵의 크기만큼 메모리를 핛당핚다.
	if ((bits = (unsigned char *)malloc(bitsize)) == NULL) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	// 비트맵 데이터를 bit(GLubyte 타입)에 저장핚다.
	if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize) {
		free(*info); free(bits);
		fclose(fp);
		return NULL;
	}
	fclose(fp);
	return bits;
}


void map()
{
	//왼쪽 끝 벽
	glPushMatrix();
	glTranslatef(-3500, 0, 500);
	for (int i = 0; i < 12; i++)
	{
		glTranslatef(0, 0, -1000);
		wall_hight();
	}
	glPopMatrix();
	//오른쪽 끝 벽
	glPushMatrix();
	glTranslatef(3600, 0, 500);
	for (int i = 0; i < 12; i++)
	{
		glTranslatef(0, 0, -1000);
		wall_hight();
	}
	glPopMatrix();

	glPushMatrix();
	//1~3구역
	glTranslatef(4000, 0, 0);
	for (int i = 0; i < 3; i++)
	{
		glTranslatef(-1000, 0, 0);
		wall_6width();
	}

	glPopMatrix();

	glPushMatrix();
	glTranslatef(-4000, 0, 0);
	for (int i = 0; i < 3; i++)
	{
		glTranslatef(1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();

	//7~9구역
	glPushMatrix();
	glTranslatef(-3000, 0, -1200);
	for (int i = 0; i < 5; i++)
	{
		glTranslatef(1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();
	//10~12구역
	glPushMatrix();
	glTranslatef(-3000, 0, -1800);
	for (int i = 0; i < 6; i++)
	{
		glTranslatef(1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();
	//13~15 빈공간
	//16~18

	glPushMatrix();
	glTranslatef(-4000, 0, -3000);
	for (int i = 0; i < 2; i++)
	{

		glTranslatef(1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3000, 0, -3000);
	for (int i = 0; i < 3; i++)
	{

		glTranslatef(-1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();
	//19~21
	glPushMatrix();
	glTranslatef(1000, 0, -3600);
	for (int i = 0; i < 1; i++)
	{

		glTranslatef(-1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();

	//22~24
	glPushMatrix();
	glTranslatef(-3000, 0, -4200);
	for (int i = 0; i < 3; i++)
	{

		glTranslatef(1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4000, 0, -4200);
	for (int i = 0; i < 2; i++)
	{

		glTranslatef(-1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();
	//25~27
	glPushMatrix();
	glTranslatef(1000, 0, -4800);
	for (int i = 0; i < 1; i++)
	{

		glTranslatef(-1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();
	//28~30
	glPushMatrix();
	glTranslatef(-4000, 0, -5400);
	for (int i = 0; i < 6; i++)
	{

		glTranslatef(1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();
	//31~33 빈공간
	//34~36
	glPushMatrix();
	glTranslatef(-3000, 0, -6600);
	for (int i = 0; i < 2; i++)
	{

		glTranslatef(1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4000, 0, -6600);
	for (int i = 0; i < 3; i++)
	{

		glTranslatef(-1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();
	//37~39
	glPushMatrix();
	glTranslatef(-3000, 0, -7200);
	for (int i = 0; i < 2; i++)
	{

		glTranslatef(1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();
	//40~42
	glPushMatrix();
	glTranslatef(-3000, 0, -7800);
	for (int i = 0; i < 1; i++)
	{

		glTranslatef(1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1000, 0, -7800);
	for (int i = 0; i < 1; i++)
	{

		glTranslatef(1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1000, 0, -7800);
	for (int i = 0; i < 1; i++)
	{

		glTranslatef(1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();
	//43~45
	glPushMatrix();
	glTranslatef(-3000, 0, -8400);
	for (int i = 0; i < 1; i++)
	{

		glTranslatef(1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();
	//46~48
	glPushMatrix();
	glTranslatef(-3000, 0, -9000);
	for (int i = 0; i < 5; i++)
	{

		glTranslatef(1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();
	//49~51
	glPushMatrix();
	glTranslatef(1000, 0, -9600);
	for (int i = 0; i < 1; i++)
	{

		glTranslatef(1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();
	//52~54
	glPushMatrix();
	glTranslatef(-4000, 0, -10200);
	for (int i = 0; i < 2; i++)
	{

		glTranslatef(1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-1000, 0, -10200);
	for (int i = 0; i < 4; i++)
	{

		glTranslatef(1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();
	//55~57 빈공간
	//58~60
	glPushMatrix();
	glTranslatef(-3000, 0, -11400);
	for (int i = 0; i < 6; i++)
	{

		glTranslatef(1000, 0, 0);
		wall_6width();
	}
	glPopMatrix();
}

void ai()
{
	glColor3f(1, 0, 0);
	glutSolidSphere(300, 30, 30);
}
void character()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 6);
	glBegin(GL_QUADS);// 캐릭터 몸통 앞면
	{
		glColor3f(1, 1, 1);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-100, 250, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-100, 0, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(100, 0, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(100, 250, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 7);
	glBegin(GL_QUADS);// 캐릭터 몸통 뒷면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-100, 250, 100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-100, 0, 100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(100, 0, 100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(100, 250, 100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 8);
	glBegin(GL_QUADS);// 캐릭터 몸통 왼쪽면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-100, 250, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-100, 0, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(-100, 0, 100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(-100, 250, 100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 9);
	glBegin(GL_QUADS);// 캐릭터 몸통 오른쪽면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(100, 250, 100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(100, 0, 100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(100, 0, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(100, 250, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 3);
	glBegin(GL_QUADS);// 캐릭터 머리 앞면
	{
		glColor3f(1, 1, 1);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-100, 380, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-100, 250, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(100, 250, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(100, 380, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 2);
	glBegin(GL_QUADS);// 캐릭터 머리 뒷면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-100, 380, 100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-100, 250, 100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(100, 250, 100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(100, 380, 100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 5);
	glBegin(GL_QUADS);// 캐릭터 머리 왼쪽면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-100, 380, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-100, 250, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(-100, 250, 100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(-100, 380, 100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 4);
	glBegin(GL_QUADS);// 캐릭터 머리 오른쪽면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(100, 380, 100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(100, 250, 100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(100, 250, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(100, 380, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 1);
	glBegin(GL_QUADS);// 캐릭터 머리 윗면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-100, 380, 100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-100, 380, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(100, 380, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(100, 380, 100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslatef(0, 250, 0);
	glRotatef(L_arm, 1, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 11);
	glBegin(GL_QUADS);// 캐릭터 왼쪽팔 앞면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-200, 0, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-200, -250, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(-100, -250, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(-100, 0, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 11);
	glBegin(GL_QUADS);// 캐릭터 왼쪽팔 뒷면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-200, 0, 100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-200, -250, 100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(-100, -250, 100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(-100, 0, 100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 14);
	glBegin(GL_QUADS);// 캐릭터 왼쪽팔 왼쪽면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-200, 0, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-200, -250, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(-200, -250, 100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(-200, 0, 100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 13);
	glBegin(GL_QUADS);// 캐릭터 왼쪽팔 오른쪽면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-100, 0, 100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-100, -250, 100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(-100, -250, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(-100, 0, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 12);
	glBegin(GL_QUADS);// 캐릭터 왼쪽팔 윗면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-200, 0, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-200, 0, 100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(-100, 0, 100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(-100, 0, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 15);
	glBegin(GL_QUADS);// 캐릭터 왼쪽팔 밑면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-200, -250, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-200, -250, 100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(-100, -250, 100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(-100, -250, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 250, 0);
	glRotatef(-R_leg, 1, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 10);
	glBegin(GL_QUADS);// 캐릭터 오른팔 앞면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(100, 0, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(100, -250, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(200, -250, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(200, 0, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 10);
	glBegin(GL_QUADS);// 캐릭터 오른팔 뒷면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(100, 0, 100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(100, -250, 100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(200, -250, 100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(200, 0, 100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 13);
	glBegin(GL_QUADS);// 캐릭터 오른팔 왼쪽면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(100, 0, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(100, -250, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(100, -250, 100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(100, 0, 100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 14);
	glBegin(GL_QUADS);// 캐릭터 오른팔 오른쪽면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(200, 0, 100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(200, -250, 100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(200, -250, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(200, 0, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 12);
	glBegin(GL_QUADS);// 캐릭터 오른팔 윗면
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(100, 0, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(100, 0, 100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(200, 0, 100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(200, 0, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 15);
	glBegin(GL_QUADS);// 캐릭터 오른팔 밑면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(100, -250, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(100, -250, 100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(200, -250, 100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(200, -250, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glPushMatrix();
	glRotatef(-L_leg, 1, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 16);
	glBegin(GL_QUADS);// 캐릭터 왼다리 앞면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-100, 0, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-100, -250, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(0, -250, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(0, 0, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 17);
	glBegin(GL_QUADS);// 캐릭터 왼다리 뒷면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-100, 0, 100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-100, -250, 100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(0, -250, 100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(0, 0, 100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 19);
	glBegin(GL_QUADS);// 캐릭터 왼다리 왼쪽면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-100, 0, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-100, -250, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(-100, -250, 100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(-100, 0, 100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 20);
	glBegin(GL_QUADS);// 캐릭터 왼다리 오른쪽면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(0, 0, 100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(0, -250, 100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(0, -250, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(0, 0, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 18);
	glBegin(GL_QUADS);// 캐릭터 왼다리 윗면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-100, 0, 100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-100, 0, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(0, 0, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(0, 0, 100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 21);
	glBegin(GL_QUADS);// 캐릭터 왼다리 밑면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(-100, -250, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(-100, -250, 100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(0, -250, 100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(0, -250, 0);
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(L_leg, 1, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 16);
	glBegin(GL_QUADS);// 캐릭터 오른다리 앞면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(0, 0, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(0, -250, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(100, -250, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(100, 0, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 17);
	glBegin(GL_QUADS);// 캐릭터 오른다리 뒷면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(0, 0, 100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(0, -250, 100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(100, -250, 100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(100, 0, 100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 20);
	glBegin(GL_QUADS);// 캐릭터 오른다리 왼쪽면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(0, 0, 100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(0, -250, 100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(0, -250, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(0, 0, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 19);
	glBegin(GL_QUADS);// 캐릭터 오른다리 오른쪽면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(100, 0, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(100, -250, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(100, -250, 100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(100, 0, 100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 18);
	glBegin(GL_QUADS);// 캐릭터 오른다리 윗면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(0, 0, 100);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(0, 0, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(100, 0, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(100, 0, 100);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 21);
	glBegin(GL_QUADS);// 캐릭터 오른다리 밑면
	{

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(0, -250, 0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(0, -250, 100);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(100, -250, 100);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(100, -250, 0);
	}
	glEnd();
	glPopMatrix();

}