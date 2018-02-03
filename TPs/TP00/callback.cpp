#include "callback.h"
#include <cmath>

#include <fstream>
#include <sstream>

void Callback::callbackPos(GLFWwindow *pWnd, double xpos, double ypos)
{
    CColor &color = *(static_cast<CColor*>(glfwGetWindowUserPointer(pWnd)));

    int nWidth, nHeight;
    glfwGetWindowSize(pWnd, &nWidth, &nHeight);

    color.SetHSV(
            color.GetH(),
            _cast(float)(xpos/nWidth),
            _cast(float)(nHeight-ypos) / nHeight
    );

    glClearColor(
            color.GetR(),
            color.GetG(),
            color.GetB(),
            1.0f
    );
}

void Callback::callbackKey(GLFWwindow *pWnd, int key, int scancode, int action, int mods)
{

    CColor &color = *(static_cast<CColor*>(glfwGetWindowUserPointer(pWnd)));

    if(action == GLFW_PRESS || action == GLFW_REPEAT )
    {
        double hue = 0.0f;

        switch(key)
        {
            case(GLFW_KEY_UP):
                hue = fmod(color.GetH() + 1.0, 360.0);
                break;
            case(GLFW_KEY_DOWN):
                hue = fmod(color.GetH() - 1.0, 360.0);
                break;
            default:
                break;
        }

        color.SetHSV(
                _cast(float)hue,
                color.GetS(),
                color.GetV()
        );
    }

    glClearColor(
            color.GetR(),
            color.GetG(),
            color.GetB(),
            1.0f
    );
}

bool Utils::loadShader(GLint shader, const std::string &strFileName) {
    std::ifstream vertexShaderFile{ strFileName };
    if (!vertexShaderFile.is_open())
        return false;

    std::stringstream vertexShaderStream;
        vertexShaderStream << vertexShaderFile.rdbuf();

    std::string vertexShaderString{ vertexShaderStream.str() };
        const GLchar* vertexShaderSource = vertexShaderString.c_str(); //vertexShaderSource est correct tant que vertexShaderString n'est pas modifié

    glShaderSource(shader, 1, &vertexShaderSource, nullptr);
        return true;
}
