#pragma once
#include "iostream"
#include "GLFW/glfw3.h"

#define OPENGL_CORE_PROFILE 1
#define OPENGL_RESIZABLE_DISABLED 2
// Add more if needed

#define _rcast(type, object) reinterpret_cast<type>(object)
#define _scast(type, object) static_cast<type>(object)
#define _cast(type, object) cast<type>(object)

#define glSetBuffer(BUF, number, type, method, bindable) \
glGenBuffers(number, &BUF); \
glBindBuffer(type, BUF); \
glBufferData(type, sizeof(bindable), bindable, method)

void glfwAdvancedHint(int major, int minor, int flags)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, minor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, major);

    if (flags & 1)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (flags & 2)
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void terminate(const char* message)
{
    std::cerr << message << std::endl;
    glfwTerminate();
    std::exit(-1);
}


