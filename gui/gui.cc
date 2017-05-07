#include <iostream>
#include <GLFW/glfw3.h>
#include "gui.h"

using namespace std;

vector<Gui*> Gui::instances;

Gui::Gui(){
	instances.push_back(this);
	Init();
}

Gui::~Gui(){
	th_flg = false;
	gui_th->join();
//	for(;!th_flg;);
	cerr<<"neko!w"<<endl;
}

void Gui::Init(){
	disp = nullptr;

	scrnx = DEFAULT_SCRNX;
	scrny = DEFAULT_SCRNY;

	//threadの初期化も行うべき
}

int Gui::Start(){
	th_flg = true;
	gui_th = new thread(&Gui::gui_proc, this);
}

void errCallBack(int e, const char *d){
	cerr<<"errcode="<<e<<" description:"<<d<<endl;
}

static int hoge = 0;

void keyCallBack(GLFWwindow *win, int key, int scancode, int act, int mod){
	cerr<<"key:"<<key<<endl;
	glfwMakeContextCurrent(win);
	hoge++;
	int i = hoge%3;
	if(i==0) glClearColor(1.0, 0.0, 0.0, 1.0);
	if(i==1) glClearColor(0.0, 1.0, 0.0, 1.0);
	if(i==2) glClearColor(0.0, 0.0, 1.0, 1.0);
}

void Gui::gui_proc(){
	GLFWwindow *win;

	glfwSetErrorCallback(errCallBack);
	glfwInit();

	win = glfwCreateWindow(scrnx, scrny, "window", NULL, NULL);
//	glfwSetKeyCallback(win, keyCallBack);

	glfwMakeContextCurrent(win);

	glPixelZoom(1,-1);
	glRasterPos2f(-1,1);

	while(!glfwWindowShouldClose(win) && th_flg){
		glClear(GL_COLOR_BUFFER_BIT);

		if(disp != nullptr){
//			glRasterPos2f(-1,1);
			disp->FlushImage();
			glDrawPixels(scrnx, scrny, GL_RGB, GL_UNSIGNED_BYTE, disp->GetImage());
		}

		glfwSwapBuffers(win);
		glfwWaitEvents();
	}

	cerr<<"Gui::gui_proc() ending..."<<endl;

	glfwDestroyWindow(win);
//	glfwTerminate();

	th_flg = true;

	return;
}


