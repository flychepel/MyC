#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ClassShader.h"
#include "ClassCamera.h"
#include "ClassModel.h"


#include <iostream>
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// ���������
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// ������
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true , start = false;
float dx = 0, dz = 0;

// ��������
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f); // ���������� ��������� �����

float i = 0;

int main()
{
    // glfw: ������������� � ����������������
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw: �������� ����
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL for Ravesli.com!", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // �������� GLFW, ����� �� �������� ��� ������
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: �������� ���� ���������� �� OpenGL-�������
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // �������� stb_image.h, ����� �� ���������� ����������� �������� ������������ y-��� (�� �������� ������)
    //stbi_set_flip_vertically_on_load(true);

    // ���������������� ����������� ��������� OpenGL
    glEnable(GL_DEPTH_TEST);

    // �������������� ����� ��������� ���������
    Shader ourShader("Shaders/shader.vs", "Shaders/shader.fs");

    // �������� �������

    Model box("Models//box.obj");
    Model shpritz("Models//shpriz.obj");
    Model room("Models//room.obj");
 
    // ��������� � ������ �������
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    ourShader.use();
    // ������������� ����������� �����
    ourShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    ourShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // ������� �������� ���������� ����
    ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // ��������� ���������
    ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    ourShader.setFloat("material.shininess", 64.0f);


    // ���� ����������
    while (!glfwWindowShouldClose(window))
    {
        //std::cout << i << std::endl; i++;
        // ���������� ����� ������ �� �������� ��� ������� �����
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // ��������� �����
        processInput(window);

        // ���������
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ����������, ��� ������������ ������ ������, ��� ����������� uniform-����������/�������_���������
        ourShader.use();
        ourShader.setVec3("light.position", lightPos);
        ourShader.setVec3("viewPos", camera.Position);

        // �������������� ����/��������
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        
        glm::mat4 model = glm::mat4(1.0f);
         
        // �������
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	
        ourShader.setMat4("model", model);
        box.Draw(ourShader);

        // �����
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0, 0.0f, 0.0));
        model = glm::rotate(model, glm::radians(dz) * 20, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(dx) * 20, glm::vec3(0.0f, 0.0f, 1.0f));
        ourShader.setMat4("model", model);
         if (start)
        {
            i++;
            model = glm::translate(model, glm::vec3(i, 0.0f, 0.0f));
        }
        shpritz.Draw(ourShader);

        // ����
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
        ourShader.setMat4("model", model);
        room.Draw(ourShader);
        

        // glfw: ����� ���������� front- � back- �������. ������������ ������� �����/������ (���� �� ������/�������� ������, ��������� ������ ���� � �.�.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: ����������, ������������ ���� ���������� ����� GLFW-��c�����
    glfwTerminate();
    return 0;
}

// ��������� ���� ������� �����: ������ GLFW � �������/���������� ������ ���� � ������ ����� � ��������������� ��������� ������ �������
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        dx += 0.005f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        dx -= 0.005f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        dz -= 0.005f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        dz += 0.005f;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        start = true;
}

// glfw: ������ ���, ����� ���������� ������� ���� (������������� ��� ������������ ��������), ���������� ������ callback-�������
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // ����������, ��� ���� ��������� ������������� ����� �������� ����.
    // �������� ��������, ������ � ������ ����� ����������� ������, ��� �������, �� Retina-��������
    glViewport(0, 0, width, height);
}

// glfw: ������ ���, ����� ������������ ����, ���������� ������ callback-�������
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // �����������, ��� ��� y-���������� ���� ����� �����

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: ������ ���, ����� �������������� �������� ����, ���������� ������ callback-�������
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}