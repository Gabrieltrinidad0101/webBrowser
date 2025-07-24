#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// Vertex shader
const char* vertexShaderSource = R"glsl(
#version 330 core
layout(location = 0) in vec2 aPos;

out vec2 fragPos;

void main()
{
    fragPos = aPos;  // Pasamos la posición al fragment shader
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)glsl";

// Fragment shader con border radius
const char* fragmentShaderSource = R"glsl(
#version 330 core
in vec2 fragPos;

out vec4 FragColor;

uniform vec2 rectPos;   // posición inferior izquierda del rectángulo
uniform vec2 rectSize;  // tamaño del rectángulo (ancho, alto)
uniform float radius;   // radio del borde redondeado

void main()
{
    // Posición local dentro del rectángulo
    vec2 localPos = fragPos - rectPos;

    // Puntos cercanos al rectángulo dentro del área central (sin esquinas)
    vec2 clamped = clamp(localPos, vec2(radius), rectSize - vec2(radius));

    // Distancia al centro de la esquina más cercana
    float dist = length(localPos - clamped);

    // Si estamos fuera del radio, descartamos el fragmento
    if(dist > radius)
        discard;

    FragColor = vec4(0.1, 0.6, 0.9, 1.0);  // Color azul
}
)glsl";

// Función para compilar shader y verificar errores
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR compiling shader: " << infoLog << "\n";
    }
    return shader;
}

int main()
{
    // Inicializar GLFW
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Configurar GLFW: OpenGL 3.3 core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Rounded Rectangle OpenGL", nullptr, nullptr);
    if(!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Inicializar GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Compilar shaders y linkear programa
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Verificar link
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR linking program: " << infoLog << "\n";
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Cuadrado desde (0,0) a (0.8, 0.6) en coordenadas NDC [-1,1] debemos transformar después
    float vertices[] = {
        // x, y
        -0.8f, -0.6f,
         0.8f, -0.6f,
         0.8f,  0.6f,
        -0.8f,  0.6f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configurar atributo de vértices
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Desbind buffers (opcional)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Variables para uniforms
    // Convertir la posición y tamaño del rectángulo a coordenadas del shader
    // En este caso, la posición inferior izquierda y tamaño del rectángulo en NDC
    // El rectángulo va de (-0.8,-0.6) a (0.8,0.6)
    // Pasamos la posición en fragPos (coincide con coords en vertex shader)
    // NOTA: fragPos == gl_Position.xy en NDC

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Asignar uniforms:
        GLint rectPosLoc = glGetUniformLocation(shaderProgram, "rectPos");
        GLint rectSizeLoc = glGetUniformLocation(shaderProgram, "rectSize");
        GLint radiusLoc = glGetUniformLocation(shaderProgram, "radius");

        // En fragPos tenemos coords en NDC, rectPos es la esquina inferior izquierda
        glUniform2f(rectPosLoc, -0.8f, -0.6f);
        glUniform2f(rectSizeLoc, 1.6f, 1.2f); // ancho y alto del rectángulo
        glUniform1f(radiusLoc, 0.7f);          // radio del borde redondeado

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

    glfwTerminate();
    return 0;
}
