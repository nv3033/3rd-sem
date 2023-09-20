#include "3rd sem.h"
#include "include/hexsagon_header.h"
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION  
#include "stb_image.h"
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "include/camera_header.h"
#include <iostream>
#include <cmath>
#include <vector>

// Функция для генерации шума Перлина в двумерном пространстве
double noise(double x, double y) {
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

// Функция для интерполяции значений
double interpolate(double a, double b, double x) {
    double ft = x * 3.1415927;
    double f = (1 - cos(ft)) * 0.5;
    return a * (1 - f) + b * f;
}

// Функция для генерации шума Перлина с интерполяцией
double interpolatedNoise(double x, double y) {
    int intX = int(x);
    double fractionalX = x - intX;

    int intY = int(y);
    double fractionalY = y - intY;

    double v1 = noise(intX, intY);
    double v2 = noise(intX + 1, intY);
    double v3 = noise(intX, intY + 1);
    double v4 = noise(intX + 1, intY + 1);

    double i1 = interpolate(v1, v2, fractionalX);
    double i2 = interpolate(v3, v4, fractionalX);

    return interpolate(i1, i2, fractionalY);
}

int main(int argc, char* argv[]) {

        const int WIDTH = 10; // Ширина двумерного массива точек
        const int HEIGHT = 10; // Высота двумерного массива точек

        std::vector<std::vector<double>> points(WIDTH, std::vector<double>(HEIGHT));

        // Генерация шума Перлина для каждой точки
        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                double noiseValue = interpolatedNoise(x / 10.0, y / 10.0); // Масштабирование координат для более плавной генерации шума
                points[x][y] = noiseValue;
            }
        }

        // Вывод двумерного массива точек на консоль
        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                std::cout << points[x][y] << " ";
            }
            std::cout << std::endl;
        }

    




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


    //Настройка и запуск ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    //Создание окна ImGui
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = 800;
    io.DisplaySize.y = 600;

    //Создаем счетчик шестиугольников и их массив
    int hexcount = 0;
    Hexsagon hex[16];
    for (int i = 0; i < 16; i++)
        hex[i].path = *argv;

    //Создаем камеру
    float camera_position[3] = { 0.f, 0.f, 1.f };
    float camera_rotation[3] = { 0.f, 0.f, 0.f };
    bool perspective_camera = false;
    Camera camera;



    // Рендеринг цикла
    while (!glfwWindowShouldClose(window)) {
        // Очистка буфера цвета
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //Обновляем окно ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        //Описываем само окно
        ImGui::Begin("Window");/*
        ImGui::SliderFloat3("scale", scale, 0.f, 2.f);
        ImGui::SliderFloat("rotate", &rotate, 0.f, 360.f);
        ImGui::SliderFloat3("translate", translate, -1.f, 1.f);*/
        ImGui::SliderInt("count", &hexcount, 0, 16); 
        ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f);
        ImGui::SliderFloat3("camera rotation", camera_rotation, 0, 360.f);
        ImGui::End();

        //Рисуем шестиугольники (так же привзываем камеру к шейдерам)
        for (int i = 0; i < hexcount; i++) {
            glUniformMatrix4fv(glGetUniformLocation(hex[i].shaderProgram, "view_projection_matrix"), 1, GL_FALSE,
                glm::value_ptr(camera.m_projection_matrix * camera.m_view_matrix));
            hex[i].draw();
        }

        //Перемещаем шестиугольники в зависимости от их количества
        for (int i = 0; i < 16; i++) {
            if(hexcount / 2 != 0)
                hex[i].scale[0] = 1.0f / hexcount;
        }
        for (int i = 0; i < 16; i++) {
            if (i % 2 == 0) hex[i].translate[0] = i / 2 * hex[i].scale[0] * 2;
            else hex[i].translate[0] = ((-i) / 2 - 1) * hex[i].scale[0] * 2;
        }   

        //Обновлем параметры камеры
        camera.m_position = glm::vec3(camera_position[0], camera_position[1], camera_position[2]);
        camera.m_rotation = glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]);
        camera.update_projection_matrix();
        camera.update_view_matrix();

        //Рендерим окно ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //Рендерим окно
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Закрытие окна GLFW и освобождение ресурсов GLFW
    glfwTerminate();

    return 0;
}