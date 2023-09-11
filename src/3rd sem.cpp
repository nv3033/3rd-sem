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
        ImGui::End();

        //Рисуем шестиугольники
        for(int i = 0; i < hexcount; i++)
            hex[i].draw();

        for (int i = 0; i < 16; i++) {
            if(hexcount / 2 != 0)
                hex[i].scale[0] = 1.0f / hexcount;
        }
        //std::cout << hex[0].scale[0] << std::endl;
        for (int i = 0; i < 16; i++) {
            if (i % 2 == 0) hex[i].translate[0] = i / 2 * hex[i].scale[0] * 2;
            else hex[i].translate[0] = ((-i) / 2 - 1) * hex[i].scale[0] * 2;
        }

        //Рендерим окно ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Закрытие окна GLFW и освобождение ресурсов GLFW
    glfwTerminate();

    return 0;
}