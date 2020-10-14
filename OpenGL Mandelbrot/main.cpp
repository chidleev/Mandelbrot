#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


float zoom = 2.f;
glm::vec3 center = glm::vec3(0, 0, 0);
float t = 100;
glm::vec2 const_num = glm::vec2(0, 0);

double time = 0;


int main()
{
    // glfw: инициализаци€ и конфигурирование
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    //  онстанты
    const unsigned int SCR_WIDTH = mode->width;
    const unsigned int SCR_HEIGHT = mode->height;


    // glfw: создание окна
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, " ", glfwGetPrimaryMonitor(), NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // glad: загрузка всех указателей на OpenGL-функции
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    Shader shader("Shaders/vertex.cpp", "Shaders/fragment.cpp");


    //создаем пр€моугольник
    float vertices[] = {
        // координаты
         1,  1, 0.0f,   // верхн€€ права€
         1, -1, 0.0f,   // нижн€€ права€
        -1, -1, 0.0f,   // нижн€€ лева€
        -1,  1, 0.0f,   // верхн€€ лева€ 
    };

    int elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glBindVertexArray(0);


    // ÷икл рендеринга
    while (!glfwWindowShouldClose(window))
    {
        // ќбработка ввода
        processInput(window);


        shader.use();

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, center);
        transform = glm::scale(transform, glm::vec3(zoom * float(SCR_WIDTH) / SCR_HEIGHT, zoom, 1));

        unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        shader.setInt("max_iter", t);
        shader.setFloat("x", sin(const_num.x));
        shader.setFloat("y", sin(const_num.y));
        shader.setFloat("time", glfwGetTime());


        // ¬ыполнение рендеринга
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}



// ќбработка всех событий ввода
void processInput(GLFWwindow* window)
{     
    double dt = glfwGetTime() - time;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        center += glm::vec3(0, dt * zoom, 0);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        center += glm::vec3(-dt * zoom, 0, 0);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        center += glm::vec3(0, -dt * zoom, 0);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        center += glm::vec3(dt * zoom, 0, 0);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        zoom /= 1.01;
        t += 0.1;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        zoom *= 1.01;
        t -= 0.1;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        const_num.y += 0.001;

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        const_num.y -= 0.001;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        const_num.x += 0.001;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        const_num.x -= 0.001;

    time = glfwGetTime();
}

//реагируем на изменение размеров окна
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}