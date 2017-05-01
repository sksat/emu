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
	
}

void Gui::Init(){
	scrnx = DEFAULT_SCRNX;
	scrny = DEFAULT_SCRNY;
}

int Gui::Start(){
	gui_th = new thread(&Gui::gui_proc, this);
}

void Gui::gui_proc(){
	GLFWwindow *win;

	glfwInit();

	win = glfwCreateWindow(scrnx, scrny, "window", NULL, NULL);

	glfwMakeContextCurrent(win);

	while(!glfwWindowShouldClose(win)){
		glfwSwapBuffers(win);
		glfwWaitEvents();
	}

	cerr<<"Gui::gui_proc() ending..."<<endl;

	glfwDestroyWindow(win);
//	glfwTerminate();
	return;
}


