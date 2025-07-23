#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shader sources
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D text;
uniform vec3 textColor;

void main() {
    float alpha = texture(text, TexCoords).r;
    FragColor = vec4(textColor, alpha);
}
)";

// Font settings
const int FONT_SIZE = 100;
const int ATLAS_WIDTH = 1024;
const int ATLAS_HEIGHT = 1024;
stbtt_bakedchar cdata[96]; // ASCII 32..127
GLuint atlasTexture;
GLuint VAO, VBO;
GLuint shaderProgram;

void renderText(const std::string& text, float x, float y, glm::vec3 color) {
    glUseProgram(shaderProgram);
    glUniform3f(glGetUniformLocation(shaderProgram, "textColor"), color.r, color.g, color.b);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, atlasTexture);

    float xpos = x, ypos = y;
    for (char ch : text) {
        if (ch < 32 || ch >= 128) continue;

        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(cdata, ATLAS_WIDTH, ATLAS_HEIGHT, ch - 32, &xpos, &ypos, &q, 1);

        float vertices[6][4] = {
            { q.x0, q.y1, q.s0, q.t1 },
            { q.x0, q.y0, q.s0, q.t0 },
            { q.x1, q.y0, q.s1, q.t0 },

            { q.x0, q.y1, q.s0, q.t1 },
            { q.x1, q.y0, q.s1, q.t0 },
            { q.x1, q.y1, q.s1, q.t1 },
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader compile error:\n" << infoLog << std::endl;
    }
    return shader;
}

void initFont(const char* fontPath) {
    // Load font into memory
    FILE* fontFile = fopen(fontPath, "rb");
    if (!fontFile) {
        std::cerr << "Could not open font file\n";
        exit(1);
    }
    fseek(fontFile, 0, SEEK_END);
    int size = ftell(fontFile);
    fseek(fontFile, 0, SEEK_SET);
    unsigned char* fontBuffer = new unsigned char[size];
    fread(fontBuffer, 1, size, fontFile);
    fclose(fontFile);

    // Bake font bitmap
    unsigned char* bitmap = new unsigned char[ATLAS_WIDTH * ATLAS_HEIGHT];
    stbtt_BakeFontBitmap(fontBuffer, 0, FONT_SIZE, bitmap, ATLAS_WIDTH, ATLAS_HEIGHT, 32, 96, cdata);

    // Upload to OpenGL
    glGenTextures(1, &atlasTexture);
    glBindTexture(GL_TEXTURE_2D, atlasTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ATLAS_WIDTH, ATLAS_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] bitmap;
    delete[] fontBuffer;
}

int main() {
    // Init GLFW and OpenGL
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Font Atlas", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, 800, 600);

    // Compile shaders
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    glDeleteShader(vs);
    glDeleteShader(fs);

    // Set orthographic projection
    glm::mat4 proj = glm::ortho(0.0f, 1000.0f, 1000.0f, 0.0f);
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
    glUniform1i(glGetUniformLocation(shaderProgram, "text"), 0);

    // Load font
    initFont("BitcountPropSingle_Cursive-Light.ttf");  // ⬅ Replace with your font path

    // Set up VAO/VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderText("OpenGL + stb_truetype", 30.0f, 500.0f, glm::vec3(1.0, 0.9, 0.2));
        renderText("Text looks clean!", 30.0f, 420.0f, glm::vec3(0.2, 1.0, 0.7));
        renderText("ASCII chars only [32-127]", 30.0f, 340.0f, glm::vec3(0.8, 0.8, 0.8));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &atlasTexture);
    glfwTerminate();
    return 0;
}
