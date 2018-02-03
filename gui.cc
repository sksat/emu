#include <GLFW/glfw3.h>
#include <iostream>
#include "gui.h"

Gui::Gui(){
	Init();
}

Gui::~Gui(){
	End();
}

void Gui::Init(){}

void Gui::Start(){
	flg = true;
	th = std::thread(&Gui::proc, this);
}

void Gui::End(){
	flg = false;
	if(th.joinable())
		th.join();
}

void Gui::proc(){
	GLFWwindow *win;
	glfwInit();
	win = glfwCreateWindow(100, 100, "emulator", nullptr, nullptr);
	glfwMakeContextCurrent(win);
	glPixelZoom(1,-1);
	glRasterPos2f(-1,1);
	while(!glfwWindowShouldClose(win) && flg){
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(win);
		glfwWaitEvents();
	}

	glfwDestroyWindow(win);
	onExit();
}
