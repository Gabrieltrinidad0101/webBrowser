#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Vertex shader source
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos, 1.0);
}
)";

// Fragment shader source
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(0.2, 0.6, 0.9, 1.0);
}
)";

// Convert pixel coords to NDC and create 4 vertices for square
void makeSquareVertices(float x, float y, float w, float h, int windowWidth, int windowHeight, float* vertices) {
    float ndc_x = 2.0f * x / windowWidth - 1.0f;
    float ndc_y = 1.0f - 2.0f * y / windowHeight;
    float ndc_w = 2.0f * w / windowWidth;
    float ndc_h = 2.0f * h / windowHeight;

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

int main() {
    const int windowWidth = 800;
    const int windowHeight = 600;

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // OpenGL 3.3 Core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Square with Pixels", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load OpenGL functions with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Indices for two triangles forming a square
    unsigned int indices[] = { 0, 1, 3, 1, 2, 3 };

    // Setup shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // (Add error checking in production!)

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Generate buffers and array object
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Dynamic vertex buffer (data updated every frame)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Square pixel params
    float squareX = 100.0f;
    float squareY = 150.0f;
    float squareW = 200.0f;
    float squareH = 200.0f;

    // Vertices array to update VBO
    float vertices[12];

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Update vertices from pixel coords
        makeSquareVertices(squareX, squareY, squareW, squareH, windowWidth, windowHeight, vertices);

        // Update VBO data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
