#ifndef CHARACTER_H
#define CHARACTER_H

#include "cube.h"
#include <GLFW/glfw3.h>

class Character
{
    public:
        glm::mat4 view;
        Character(const float playerRadius, const float playerHeight, float gravity, float jumpStrength, float Sensitivity, float speed, bool fly, glm::vec3 startPosition);
        void update(Shader& shader, GLFWwindow* window, Cube collisionCubes[], int numCubes, float deltaTime, bool doCollision, bool gravityEnabled);
        void mouse_Handler(GLFWwindow* window, double xpos, double ypos);
        glm::vec3 CheckCollision(glm::vec3 posOne, float playerReach, float playerH, glm::vec3 posTwo, glm::vec3 sizeTwo);
        void setPositionY(glm::vec3* pos);
        void processInput(GLFWwindow* window, float deltaTime);
        void handleGravity(GLFWwindow* window, float deltaTime, bool gravityEnabled);

    private:
        glm::vec3 velocity;
        float gravitySpeed;
        glm::vec3 collideVec;
        glm::vec3 cameraPos = glm::vec3(1.0f, 0.0f, 1.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        float playerHeight;
        float playerRadius;
        float gravity;
        float yaw = 180.0f;
        float pitch = 45.0f;
        struct MouseState {
	        double x = 0.0, y = 0.0;
        } preMouse;
        float mouseSensitivity;
        float cameraSpeed;
        float jumpStrength;
        bool fly;
};

#endif