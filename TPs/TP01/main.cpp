#include <GL\glew.h>
#include "glfw3.h"

#include <Shader.h>
#include <ShaderProgram.h>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
    if (!glfwInit()) exit(EXIT_FAILURE);

    glfwAdvancedHint(3, 3, OPENGL_CORE_PROFILE | OPENGL_RESIZABLE_DISABLED);

    GLFWwindow* pWnd = glfwCreateWindow(800, 600, "Hello world", nullptr, nullptr);

    if(!pWnd)
        terminate("Impossible de créer la fenêtre!");

    glfwMakeContextCurrent(pWnd);

    // GLEW
    glewExperimental = GL_TRUE;
    glewInit();

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
    CShaderProgram shaderProgram{
            CShader{GL_VERTEX_SHADER, ifstream("C:\\Users\\Killian\\Desktop\\OpenGL\\TPs\\TP01\\vertex.vert") },
            CShader{GL_FRAGMENT_SHADER, ifstream("C:\\Users\\Killian\\Desktop\\OpenGL\\TPs\\TP01\\fragment.frag") }
    };

    // VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    GLuint VBO;
    glSetBuffer(VBO, 1, GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, _rcast(GLvoid*, 0));
    glEnableVertexAttribArray(0);

    // EBO
    GLuint EBO;
    glSetBuffer(EBO, 1, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices);

    while(!glfwWindowShouldClose(pWnd))
    {
        shaderProgram.Use();

        glfwPollEvents();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, _rcast(GLvoid*, 0));

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glfwSwapBuffers(pWnd);

    }
    glfwTerminate();
    return 0;
}