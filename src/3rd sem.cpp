#include "3rd sem.h"
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include "json.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION  
#include "stb_image.h"
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/type_ptr.hpp>


const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
uniform mat4 model_matrix;

out vec2 TexCoord;

void main()
{
    gl_Position = model_matrix * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
})";

// Фрагментный шейдер
const char* fragmentShaderSource = R"(
    #version 330 core
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D textureSampler;

void main()
{
    FragColor = texture(textureSampler, TexCoord);
}
)";

//Переменые для хранения значений трансформации объекта
float scale[3] = { 1.f, 1.f, 1.f };
float rotate = 0.f;
float translate[3] = { 0.f, 0.f, 0.f };

int main() {
    // Инициализация GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Конфигурация GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Создание окна GLFW
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Привязка контекста OpenGL к окну GLFW
    glfwMakeContextCurrent(window);

    // Инициализация GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glm::mat3 mat_1(4, 0, 0, 2, 8, 1, 0, 1, 0);
    glm::mat3 mat_2(4, 2, 9, 2, 0, 4, 1, 4, 2);

    glm::mat3 result_mat = mat_1 * mat_2;

    glm::vec4 vec(1, 2, 3, 4);
    glm::mat4 mat_identity(1);

    glm::vec4 result_vec = mat_identity * vec;

    //Настройка и запуск ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    //Создание окна ImGui
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = 800;
    io.DisplaySize.y = 600;


    // Компиляция и создание вершинного шейдера
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Проверка статуса компиляции вершинного шейдера
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
    }

    // Компиляция и создание фрагментного шейдера
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Проверка статуса компиляции фрагментного шейдера
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
    }

    // Создание шейдерной программы и прикрепление шейдеров
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Проверка статуса связывания шейдерной программы
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program linking failed: " << infoLog << std::endl;
    }

    // Удаление шейдеров
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Определение координат треугольника и текстурных координат
    /*
    float vertices[] = {
        // Координаты        // Текстурные координаты
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,  0.5f, 1.0f
    };
    */
    
    float vertices[] = {
        // Координаты        // Текстурные координаты

        //1-ый треугольник
        -0.5f, -0.866f, 0.0f,  0.25f, 0.067f,
         0.5f, -0.866f, 0.0f,  0.75f, 0.067f,
         1.0f,  0.0f, 0.0f,  1.0f, 0.5f,
         //2-ый
         1.0f,  0.0f, 0.0f,  1.0f, 0.5f,
         0.5f,  0.866f, 0.0f,  0.75f, 0.933f,
         -1.0f,  0.0f, 0.0f,  0.0f, 0.5f,
         //3-ый
         -0.5f, -0.866f, 0.0f,  0.25f, 0.067f,
         1.0f,  0.0f, 0.0f,  1.0f, 0.5f,
         -1.0f,  0.0f, 0.0f,  0.0f, 0.5f,
         //4-ый
         0.5f,  0.866f, 0.0f,  0.75f, 0.933f,
         -0.5f,  0.866f, 0.0f,  0.25f, 0.933f,
         -1.0f,  0.0f, 0.0f,  0.0f, 0.5f
    };

    // Создание и привязка буфера вершин
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Загрузка текстуры
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Установка параметров текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Загрузка изображения и создание текстуры
    int width, height, nrChannels;
    unsigned char* data = stbi_load("D:/GitHub/3rd-sem/src/include/ashwall7.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Рендеринг цикла
    while (!glfwWindowShouldClose(window)) {
        // Очистка буфера цвета
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //Обновляем окно ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        //Описываем само окно
        ImGui::Begin("Window");
        ImGui::SliderFloat3("scale", scale, 0.f, 2.f);
        ImGui::SliderFloat("rotate", &rotate, 0.f, 360.f);
        ImGui::SliderFloat3("translate", translate, -1.f, 1.f);
        ImGui::End();

        //Рендерим окно ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //Обновление матриц трансформации
        glm::mat4 scale_matrix(scale[0], 0, 0, 0,
            0, scale[1], 0, 0,
            0, 0, scale[2], 0,
            0, 0, 0, 1);
        float rotate_in_radians = glm::radians(rotate);
        glm::mat4 rotate_matrix(cos(rotate_in_radians), sin(rotate_in_radians), 0, 0,
            -sin(rotate_in_radians), cos(rotate_in_radians), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
        glm::mat4 translate_matrix(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            translate[0], translate[1], translate[2], 1);
        glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

        // Использование шейдерной программы
        glUseProgram(shaderProgram);

        // Привязка текстуры
        glBindTexture(GL_TEXTURE_2D, texture);

        // Отрисовка треугольника
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        // Проверка событий и обмен буферов
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Освобождение ресурсов
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Закрытие окна GLFW и освобождение ресурсов GLFW
    glfwTerminate();

    return 0;
}