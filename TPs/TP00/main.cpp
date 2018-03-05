#include <GL\glew.h>
#include <iostream>
#include "callback.h"

#define GLFW_VERSION(major, minor) \
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); \
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3)

using namespace std;

int main(int argc, char* argv[])
{
    if (!glfwInit()) exit(EXIT_FAILURE);

    GLFW_VERSION(3, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    int g_err;

    GLFWwindow* pWnd = glfwCreateWindow(800, 600, "Hello world", nullptr, nullptr);

    if(!pWnd)
    {
        cerr << "Impossible de créer la fenêtre!" << endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(pWnd);

    // GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Color for background color management
    CColor color(0.0f, 1.0f, 0.5f);
    glfwSetWindowUserPointer(pWnd, &color);

    // Callback management
    glfwSetCursorPosCallback(pWnd, Callback::callbackPos);
    glfwSetKeyCallback(pWnd, Callback::callbackKey);

    // Triangle
    GLfloat vertices[] = {
            -0.5f, 0.25f,
            0.5f, 0.25f,
            0.5f, -0.25f,
            -0.5f, -0.25f
    };

    GLuint indices[] = {
            0, 2, 3,
            0, 1, 2
    };

    // Shader
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    g_err = Utils::loadShader(vertexShader, "C:\\Users\\Killian\\Desktop\\OpenGL\\TPs\\TP00\\vertex.vert");
    if(!g_err) cout << "Erreur vertex";
    glCompileShader(vertexShader);

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    g_err = Utils::loadShader(fragmentShader, "C:\\Users\\Killian\\Desktop\\OpenGL\\TPs\\TP00\\fragment.frag");
    if(!g_err) cout << "Erreur fragment";
    glCompileShader(fragmentShader);

    /* 2eme rect */
    GLuint vertexShader2;
    vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
    g_err = Utils::loadShader(vertexShader2, "C:\\Users\\Killian\\Desktop\\OpenGL\\TPs\\TP00\\vertex2.vert");
    if(!g_err) cout << "Erreur vertex";
    glCompileShader(vertexShader2);

    GLuint fragmentShader2;
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    g_err = Utils::loadShader(fragmentShader2, "C:\\Users\\Killian\\Desktop\\OpenGL\\TPs\\TP00\\fragment2.frag");
    if(!g_err) cout << "Erreur fragment";
    glCompileShader(fragmentShader2);

    // Linkage
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader2);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);

    glDeleteShader(vertexShader2);
    glDeleteShader(fragmentShader2);

    // VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, _rcast(GLvoid*)(0));
    glEnableVertexAttribArray(0);


    // EBO
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    int sw = false;
    while(!glfwWindowShouldClose(pWnd))
    {
        sw = !sw;
        if(sw) glUseProgram(shaderProgram);
        else glUseProgram(shaderProgram2);

        glfwPollEvents();
        glClearColor(color.GetR(), color.GetG(), color.GetB(), 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, _rcast(GLvoid*)(0));

        if(sw) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glfwSwapBuffers(pWnd);

    }
    glfwTerminate();
    return 0;
}