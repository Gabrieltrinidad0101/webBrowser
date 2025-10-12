#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>
#include <vector>
#include "ComponentUI/ComponentUI.h"
#include "UI/Ui.h"
// Vertex shader source
const char *vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos, 1.0);
}
)";

// Fragment shader source con uniform color
const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec4 uColor;

void main() {
    FragColor = uColor;
}
)";

struct SquareUI
{
    int x;
    int y;
    int w;
    int h;
    RGBA bgColor;

    SquareUI() = default;

    SquareUI(int x_, int y_, int w_, int h_, RGBA bgColor_)
    {
        x = x_;
        y = y_;
        w = w_;
        h = h_;
        bgColor = bgColor_;
    }
};

// Convert pixel coords to NDC and create 4 vertices for square
void makeSquareVertices(SquareUI componentUI, int windowWidth, int windowHeight, float *vertices)
{
    float ndc_x = 2.0f * componentUI.x / windowWidth - 1.0f;
    float ndc_y = 1.0f - (2.0f * componentUI.y / windowHeight);
    float ndc_w = 2.0f * componentUI.w / windowWidth;
    float ndc_h = 2.0f * componentUI.h / windowHeight;

    // Top-left
    vertices[0] = ndc_x;
    vertices[1] = ndc_y;
    vertices[2] = 0.0f;

    // Bottom-left
    vertices[3] = ndc_x;
    vertices[4] = ndc_y - ndc_h;
    vertices[5] = 0.0f;

    // Bottom-right
    vertices[6] = ndc_x + ndc_w;
    vertices[7] = ndc_y - ndc_h;
    vertices[8] = 0.0f;

    // Top-right
    vertices[9] = ndc_x + ndc_w;
    vertices[10] = ndc_y;
    vertices[11] = 0.0f;
}

// Esta función dibuja un cuadrado con el color y posición dados
void drawSquare(SquareUI componentUI,
                int windowWidth, int windowHeight,
                unsigned int VBO, unsigned int VAO, unsigned int shaderProgram)
{

    float vertices[12];
    makeSquareVertices(componentUI, windowWidth, windowHeight, vertices);

    // Actualizar buffer con los vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    // Usar el shader
    glUseProgram(shaderProgram);

    // Pasar el color como uniform
    int colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    glUniform4f(colorLoc, componentUI.bgColor[0], componentUI.bgColor[1], componentUI.bgColor[2], componentUI.bgColor[3]);

    // Dibujar
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void drawComponentUI(ComponentUI componentUI,
                     int windowWidth, int windowHeight,
                     unsigned int VBO, unsigned int VAO, unsigned int shaderProgram)
{

    SquareUI squareUI(componentUI.cssStyle.x, componentUI.cssStyle.y, componentUI.cssStyle.wWithBorder, componentUI.cssStyle.hWithBorder, componentUI.cssStyle.bgColor);
    drawSquare(squareUI, windowWidth, windowHeight, VBO, VAO, shaderProgram);

    if (componentUI.cssStyle.borderRightWidth > 0)
    {
        SquareUI border(componentUI.cssStyle.x + componentUI.cssStyle.wWithBorder, componentUI.cssStyle.y, componentUI.cssStyle.borderRightWidth, componentUI.cssStyle.hWithBorder, componentUI.cssStyle.borderRightColor);
        drawSquare(border, windowWidth, windowHeight, VBO, VAO, shaderProgram);
    }

    if (componentUI.cssStyle.borderLeftWidth > 0)
    {
        SquareUI border(componentUI.cssStyle.x - componentUI.cssStyle.borderLeftWidth, componentUI.cssStyle.y, componentUI.cssStyle.borderLeftWidth, componentUI.cssStyle.hWithBorder, componentUI.cssStyle.borderLeftColor);
        drawSquare(border, windowWidth, windowHeight, VBO, VAO, shaderProgram);
    }

    if (componentUI.cssStyle.borderTopWidth > 0)
    {
        SquareUI border(componentUI.cssStyle.x, componentUI.cssStyle.y - componentUI.cssStyle.borderTopWidth, componentUI.cssStyle.wWithBorder, componentUI.cssStyle.borderTopWidth, componentUI.cssStyle.borderTopColor);
        drawSquare(border, windowWidth, windowHeight, VBO, VAO, shaderProgram);
    }

    if (componentUI.cssStyle.borderBottomWidth > 0)
    {
        SquareUI border(componentUI.cssStyle.x, componentUI.cssStyle.y + componentUI.cssStyle.hWithBorder, componentUI.cssStyle.wWithBorder, componentUI.cssStyle.borderBottomWidth, componentUI.cssStyle.borderBottomColor);
        drawSquare(border, windowWidth, windowHeight, VBO, VAO, shaderProgram);
    }
}



void initUI(std::vector<ComponentUI*> componentChildUIs)
{
    const int windowWidth = 800;
    const int windowHeight = 600;

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "Squares with color", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return;
    }

    unsigned int indices[] = {0, 1, 3, 1, 2, 3};

    // Shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // VAO, VBO, EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);


    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (ComponentUI *n : componentChildUIs)
        {
            drawComponentUI(*n, windowWidth, windowHeight, VBO, VAO, shaderProgram);
        }

        // Ejemplo: dibujar 3 cuadrados con diferentes colores y posiciones

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return;
}
