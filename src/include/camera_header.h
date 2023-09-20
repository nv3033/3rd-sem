#pragma once

#include <glm/trigonometric.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

class Camera {
public:
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::mat4 m_view_matrix;
    glm::mat4 m_projection_matrix;
    Camera::Camera()
    {
        //Создание камеры
        update_view_matrix();
        update_projection_matrix();
    }

    void Camera::update_view_matrix() //см. https://ru.wikipedia.org/wiki/%D0%9C%D0%B0%D1%82%D1%80%D0%B8%D1%86%D0%B0_%D0%BF%D0%B5%D1%80%D0%B5%D1%85%D0%BE%D0%B4%D0%B0
    {
        //---
        //Обновляем матрицы вращения
        float rotate_in_radians_x = glm::radians(-m_rotation.x);
        glm::mat4 rotate_matrix_x(1, 0, 0, 0,
            0, cos(rotate_in_radians_x), sin(rotate_in_radians_x), 0,
            0, -sin(rotate_in_radians_x), cos(rotate_in_radians_x), 0,
            0, 0, 0, 1);

        float rotate_in_radians_y = glm::radians(-m_rotation.y);
        glm::mat4 rotate_matrix_y(cos(rotate_in_radians_y), 0, -sin(rotate_in_radians_y), 0,
            0, 1, 0, 0,
            sin(rotate_in_radians_y), 0, cos(rotate_in_radians_y), 0,
            0, 0, 0, 1);

        float rotate_in_radians_z = glm::radians(-m_rotation.z);
        glm::mat4 rotate_matrix(cos(rotate_in_radians_z), sin(rotate_in_radians_z), 0, 0,
            -sin(rotate_in_radians_z), cos(rotate_in_radians_z), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
        //---

        //---
        //обновляем матрицу позиции
        glm::mat4 translate_matrix(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            -m_position[0], -m_position[1], -m_position[2], 1);
        //---

        //матрица вида - произведение всех матриц трансформации
        m_view_matrix = rotate_matrix_y * rotate_matrix_x * translate_matrix;
    }

    void Camera::update_projection_matrix()//строим перспективу
    {
        float r = 0.1f;
        float t = 0.1f;
        float f = 10;
        float n = 0.1f;
        m_projection_matrix = glm::mat4(n / r, 0, 0, 0,
            0, n / t, 0, 0,
            0, 0, (-f - n) / (f - n), -1,
            0, 0, -2 * f * n / (f - n), 0);
    }
};