#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include "gui.h"

Gui::Gui() : disp(nullptr) {
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

	if(disp == nullptr) throw "display is null";
	win = glfwCreateWindow(disp->GetSizeX(), disp->GetSizeY(), "emulator", nullptr, nullptr);

	glfwMakeContextCurrent(win);

	glPixelZoom(1,-1);
	glRasterPos2f(-1,1);

	while(!glfwWindowShouldClose(win) && flg){
		glClear(GL_COLOR_BUFFER_BIT);

//		if(disp != nullptr){
			disp->FlushImage();
			glDrawPixels(disp->GetSizeX(), disp->GetSizeY(), GL_RGB, GL_UNSIGNED_BYTE, disp->GetImage());
//		}

		glfwSwapBuffers(win);
		glfwWaitEvents();
	}

	glfwDestroyWindow(win);
	onExit();
}
