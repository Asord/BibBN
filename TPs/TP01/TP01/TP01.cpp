#include <GL\glew.h> 
#include <GLFW\glfw3.h> 
#include <ShaderProgram.h>
#include <fstream>
#include <iostream>

#include "Color.h"

#define _rcast(type, object) reinterpret_cast<type>(object)

using namespace std;

const GLfloat pi = 3.141592f;


void keyfunct(GLFWwindow* pWnd, int key, int scancode, int action, int mods);
void posfunct(GLFWwindow* pWnd, double xpos, double ypos);
void clickfunct(GLFWwindow* pWnd, int button, int action, int mods);
void scrollfunct(GLFWwindow* pWnd, double xoffset, double yoffset);
const char* GetGLErrorString();


struct WndData
{
	struct Point
	{
		double m_dX;
		double m_dY;
	} m_ptLastClick;
	float m_fAngle;
	float m_fValeurStart;
	float m_fAngleStart;
	CColor m_Color;
};

int main(int argc, char* argv[])
{
	const char* pString;

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 16); //Multisample
	GLFWwindow* pWnd = glfwCreateWindow(600, 600, "Hello World", nullptr, nullptr);
	if (!pWnd)
		exit(0x00000805);

	glfwMakeContextCurrent(pWnd);
	glEnable(GL_MULTISAMPLE);

	glewExperimental = GL_TRUE;
	GLenum err;

	if ((err = glewInit()) != GLEW_OK)
		exit(0x00000700);

	pString = (char*)glGetString(GL_VENDOR);
	if (!pString) pString = GetGLErrorString();
	cout << "Company : " << pString << endl;

	pString = (char*)glGetString(GL_RENDERER);
	if (!pString) pString = GetGLErrorString();
	cout << "Renderer : " << pString << endl;

	pString = (char*)glGetString(GL_VERSION);
	if (!pString) pString = GetGLErrorString();
	cout << "Version : " << pString << endl;

	WndData wndData{ {0, 0},
					  0.0f,
					  0.0f, 0.0f,
					 {0.0f, 1.0f, 1.0f}
	};

	glfwSetWindowUserPointer(pWnd, &wndData);
	glfwSetKeyCallback(pWnd, keyfunct);
	glfwSetCursorPosCallback(pWnd, posfunct);
	glfwSetMouseButtonCallback(pWnd, clickfunct);
	glfwSetScrollCallback(pWnd, scrollfunct);


	GLfloat verticesCircle[361 * (2 + 3) * 2];

	const GLfloat rayonOut = 0.8f;
	const GLfloat rayonIn = 0.6f;

	for (uint16_t i = 0; i <= 360; ++i)
	{
		GLfloat &xout = verticesCircle[i * 10];
		GLfloat &yout = verticesCircle[i * 10 + 1];
		GLfloat &rout = verticesCircle[i * 10 + 2];
		GLfloat &gout = verticesCircle[i * 10 + 3];
		GLfloat &bout = verticesCircle[i * 10 + 4];
		GLfloat &xin = verticesCircle[i * 10 + 5];
		GLfloat &yin = verticesCircle[i * 10 + 6];
		GLfloat &rin = verticesCircle[i * 10 + 7];
		GLfloat &gin = verticesCircle[i * 10 + 8];
		GLfloat &bin = verticesCircle[i * 10 + 9];

		GLfloat angle = (GLfloat)i / 180.0f*pi;
		GLfloat cosangle = cos(angle);
		GLfloat sinangle = sin(angle);

		xout = rayonOut * cosangle;
		yout = rayonOut * sinangle;
		xin = rayonIn * cosangle;
		yin = rayonIn * sinangle;
		CColor color{ (float)(i % 360), 1.0f, 1.0f };
		rin = rout = color.GetR();
		gin = gout = color.GetG();
		bin = bout = color.GetB();
	}

	GLfloat verticesCursor[] =
	{
		rayonIn, 0.0f,
		0.9f*rayonIn*cos(160.0f / 180.f*pi), 0.9f*rayonIn*sin(160.0f / 180.f*pi),
		0.9f*rayonIn*cos(-160.0f / 180.f*pi), 0.9f*rayonIn*sin(-160.0f / 180.f*pi),
	};

	CShaderProgram programCircle{
		CShader{ GL_VERTEX_SHADER, ifstream{"vertex1.vert"} },
		CShader{ GL_FRAGMENT_SHADER, ifstream{"fragment1.frag"}}
	};

	CShaderProgram programCursor{
		CShader{ GL_VERTEX_SHADER, ifstream{"vertex2.vert"}},
		CShader{ GL_FRAGMENT_SHADER, ifstream{"fragment2.frag"}}
	};

	GLuint VAO[2];
	glGenVertexArrays(2, VAO);
	GLuint VBO[2];
	glGenBuffers(2, VBO);

	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCircle), verticesCircle, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), _rcast(GLvoid*, 0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), _rcast(GLvoid*, 2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCursor), verticesCursor, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, _rcast(GLvoid*, 0));
	glEnableVertexAttribArray(0);

	GLuint nCursorColorUniform = glGetUniformLocation(programCursor.GetProgramId(), "CursorColor");
	GLuint nAngleUniform = glGetUniformLocation(programCursor.GetProgramId(), "angle");

	while (!glfwWindowShouldClose(pWnd))
	{
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);

		programCircle.Use();
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 361 * 2);

		programCursor.Use();
		glUniform3f(nCursorColorUniform, wndData.m_Color.GetR(), wndData.m_Color.GetG(), wndData.m_Color.GetB());
		glUniform1f(nAngleUniform, wndData.m_fAngle);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(pWnd);
	}

	glfwTerminate();

	return 0; 
} 


void keyfunct(GLFWwindow* pWnd, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE)
		return;

	switch (key)
	{
	case GLFW_KEY_W:
	{
		GLint modes[2];
		glGetIntegerv(GL_POLYGON_MODE, modes);
		if (modes[0] == GL_LINE)
			modes[0] = GL_FILL;
		else
			modes[0] = GL_LINE;
		glPolygonMode(GL_FRONT_AND_BACK, modes[0]);
	}
	break;
	case GLFW_KEY_A:
		if (glIsEnabled(GL_MULTISAMPLE))
			glDisable(GL_MULTISAMPLE);
		else
			glEnable(GL_MULTISAMPLE);
	}
}

void posfunct(GLFWwindow* pWnd, double xpos, double ypos)
{
	WndData& wndData = *_rcast(WndData*, glfwGetWindowUserPointer(pWnd));
	if (glfwGetMouseButton(pWnd, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && glfwGetMouseButton(pWnd, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		int width, height;
		glfwGetWindowSize(pWnd, &width, &height);
		float angleStart = (float)atan2(height / 2.0 - wndData.m_ptLastClick.m_dY, wndData.m_ptLastClick.m_dX - width / 2.0);
		float angleFinish = (float)atan2(height / 2.0 - ypos, xpos - width / 2.0);
		wndData.m_fAngle = wndData.m_fAngleStart + angleFinish - angleStart;
		float h = wndData.m_fAngle * 180 / pi;
		if (h >= 360)
			h -= 360;
		if (h < 0)
			h += 360;
		wndData.m_Color.SetHSV(h, wndData.m_Color.GetS(), wndData.m_Color.GetV());
	}
	if (glfwGetMouseButton(pWnd, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && glfwGetMouseButton(pWnd, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		int width, height;
		glfwGetWindowSize(pWnd, &width, &height);
		float v = wndData.m_fValeurStart + (wndData.m_ptLastClick.m_dY - ypos) / height;
		if (v > 1)
			v = 1;
		if (v < 0)
			v = 0;
		wndData.m_Color.SetHSV(wndData.m_Color.GetH(), wndData.m_Color.GetS(), v);
	}
}

void clickfunct(GLFWwindow* pWnd, int button, int action, int mods)
{
	WndData& wndData = *_rcast(WndData*, glfwGetWindowUserPointer(pWnd));
	if ((button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT) && action == GLFW_PRESS)
	{
		glfwGetCursorPos(pWnd, &wndData.m_ptLastClick.m_dX, &wndData.m_ptLastClick.m_dY);
		wndData.m_fAngleStart = wndData.m_fAngle;
		wndData.m_fValeurStart = wndData.m_Color.GetV();
	}
}

void scrollfunct(GLFWwindow* pWnd, double xoffset, double yoffset)
{
	WndData& wndData = *_rcast(WndData*, glfwGetWindowUserPointer(pWnd));
	float sat = wndData.m_Color.GetS();
	sat += (float)yoffset / 100;
	if (sat > 1) sat = 1;
	if (sat < 0) sat = 0;
	wndData.m_Color.SetHSV(wndData.m_Color.GetH(), sat, wndData.m_Color.GetV());
}


const char* GetGLErrorString()
{
	switch (glGetError())
	{
	case GL_NO_ERROR:
		return "GL_NO_ERROR";
	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION";
	case GL_STACK_OVERFLOW:
		return "GL_STACK_OVERFLOW";
	case GL_STACK_UNDERFLOW:
		return "GL_STACK_UNDERFLOW";
	case GL_OUT_OF_MEMORY:
		return "GL_OUT_OF_MEMORY";
		return "Erreur inconnue";
	}
}