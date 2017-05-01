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

void Gui::gui_proc(){
	GLFWwindow *win;

	glfwInit();

	win = glfwCreateWindow(scrnx, scrny, "window", NULL, NULL);

	glfwMakeContextCurrent(win);

	while(!glfwWindowShouldClose(win)){
		glfwSwapBuffers(win);
		glfwWaitEvents();
	}

	glfwTerminate();
	return;
}


