#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include "gui.h"

constexpr char quadVertexShader[] = R"##(
	attribute vec4 position;
	attribute vec2 uv;
	varying vec2 vUv;
	void main() {
		gl_Position = position;
		vUv = uv;
	}
)##";

constexpr char quaduFragmentShader[] = R"##(
	varying vec2 vUv;
	uniform sampler2D texture;
	void main() {
		gl_FragColor = texture2D( texture, vUv );
	}
)##";

constexpr GLfloat planeVertices[] = {
	-1.0f,  1.0f, 0.0f,	// position
	0.0f,  0.0f,		// uv
	-1.0f, -1.0f, 0.0f,
	0.0f,  1.0f,
	1.0f, -1.0f, 0.0f,
	1.0f,  1.0f,
	1.0f,  1.0f, 0.0f,
	1.0f,  0.0f,
};

constexpr GLushort planeIndices[] = {0,1,3,2};

GLuint programObject;
GLuint vertexShader, fragmentShader;
GLuint textureId;

GLuint load_shader(GLenum type, const char *src){
	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, 0);
	glCompileShader(shader);
	return shader;
}

GLuint setup_tex(GLubyte *pixels){
	GLuint textureId;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 320, 200, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	return textureId;
}

void setup_vtx(GLuint positionLoc, GLuint texCoordLoc, GLuint samplerLoc) {
	GLuint vertexPosObject, indexObject;

	glGenBuffers(1, &vertexPosObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPosObject );
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW );

	glGenBuffers(1, &indexObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices), planeIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vertexPosObject);
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 5 * 4, 0);
	glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 5 * 4, (const GLvoid *)(3 * 4));
	glEnableVertexAttribArray(positionLoc);
	glEnableVertexAttribArray(texCoordLoc);

	glUniform1i(samplerLoc, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexObject);
}

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

	std::cout << "GUI proc start" << std::endl;
	EM_ASM(
		var c = document.createElement("canvas");
		c.setAttribyte("id", "vram");
		alert(c.id);
	);

//	glfwInit();

	if(disp == nullptr) throw "display is null";
//	win = glfwCreateWindow(disp->GetSizeX(), disp->GetSizeY(), "emulator", nullptr, nullptr);

//	glfwMakeContextCurrent(win);
/*
	vertexShader = load_shader(GL_VERTEX_SHADER, quadVertexShader);
	fragmentShader = load_shader(GL_FRAGMENT_SHADER, quaduFragmentShader);
	programObject = glCreateProgram();
	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);
	glLinkProgram(programObject);
	glUseProgram(programObject);

	textureId = setup_tex(disp->GetImage());
	GLuint positionLoc = glGetAttribLocation(programObject, "position");
	GLuint texCoordLoc = glGetAttribLocation(programObject, "uv");
	GLuint samplerLoc  = glGetUniformLocation(programObject, "texture");
	setup_vtx(positionLoc, texCoordLoc, samplerLoc);
*/
//	glPixelZoom(1,-1);
//	glRasterPos2f(-1,1);

	while(/*!glfwWindowShouldClose(win) &&*/ flg){
		if(disp != nullptr){
			disp->FlushImage();
			//glDrawPixels(disp->GetSizeX(), disp->GetSizeY(), GL_RGB, GL_UNSIGNED_BYTE, disp->GetImage());
//			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, disp->GetSizeX(), disp->GetSizeY(), GL_RGB, GL_UNSIGNED_BYTE, disp->GetImage());
		}

//		glClear(GL_COLOR_BUFFER_BIT);
//		glDrawElements(GL_TRIANGLE_STRIP, sizeof(planeIndices)/sizeof(planeIndices[0]), GL_UNSIGNED_SHORT, 0);

//		glfwSwapBuffers(win);
//		glfwPollEvents();
	}

//	glfwDestroyWindow(win);
//	glfwTerminate();
	onExit();
}
