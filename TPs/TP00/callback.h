#pragma once
#include <iostream>
#include "Color.h"
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define _cast(type) (type)
#define _scast(type) static_cast<type>
#define _rcast(type) reinterpret_cast<type>

struct Callback {
    static void callbackKey(GLFWwindow* pWnd, int key, int scancode, int action, int mods);
    static void callbackPos(GLFWwindow* pWnd, double xpos, double ypos);
};

struct Utils {
    static bool loadShader(GLint shader, const std::string& strFileName);
};
