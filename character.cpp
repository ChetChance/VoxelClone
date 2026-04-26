#include "character.h"

Character::Character(const float playerRadius, const float playerHeight, float gravity, float jumpStrength, float Sensitivity, float speed, bool fly, glm::vec3 startPosition)
{
    glm::mat4 view = glm::mat4(1.0f);

    cameraPos = startPosition;
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    gravitySpeed = 0.0f;
    this->gravity = gravity;
    this->jumpStrength = jumpStrength;
    this->playerHeight = playerHeight;
    this->playerRadius = playerRadius;
    mouseSensitivity = Sensitivity;
    cameraSpeed = speed;
    this->fly = fly;
}

void Character::update(Shader &shader, GLFWwindow *window, float deltaTime, bool gravityEnabled)
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    shader.setMat4("view", glm::value_ptr(view));

    handleGravity(window, deltaTime, gravityEnabled);
    isOnGround = false;
    processInput(window, deltaTime);
}

void Character::mouse_Handler(GLFWwindow *window, double xpos, double ypos)
{
    int lcPress = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    yaw += (xpos - preMouse.x) * mouseSensitivity;
    if (pitch <= -85.0f && (preMouse.y - ypos) * mouseSensitivity < 0.0f)
    {
        pitch = -85.0f;
    }
    else if (pitch >= 85.0f && (preMouse.y - ypos) * mouseSensitivity > 0.0f)
    {
        pitch = 85.0f;
    }
    else
    {
        pitch += (preMouse.y - ypos) * mouseSensitivity;
    }
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
    preMouse = {xpos, ypos};
}

void Character::handleCollision(const std::vector<glm::vec3> &collisionCubes, bool doCollision)
{
    if (doCollision)
    {
        /* code */

        for (size_t i = 0; i < collisionCubes.size(); i++)
        {
            CheckCollision(cameraPos, playerRadius, playerHeight, collisionCubes[i], glm::vec3(0.5f, 0.5f, 0.5f));
        }
    }
}

void Character::CheckCollision(glm::vec3 posOne, float playerReach, float playerH, glm::vec3 posTwo, glm::vec3 sizeTwo) // AABB - AABB collision
{
    glm::vec3 playerMins(
        posOne.x - playerReach,
        posOne.y - playerH,
        posOne.z - playerReach);
    glm::vec3 playerMaxs(
        posOne.x + playerReach,
        posOne.y,
        posOne.z + playerReach);
    glm::vec3 playerCenter(
        posOne.x,
        posOne.y - playerH / 2.0f,
        posOne.z);
    glm::vec3 aabbMins(
        posTwo.x,
        posTwo.y,
        posTwo.z);
    glm::vec3 aabbMaxs(
        posTwo.x + sizeTwo.x,
        posTwo.y + sizeTwo.y,
        posTwo.z + sizeTwo.z);
    glm::vec3 aabbCenter(
        posTwo.x + sizeTwo.x / 2.0f,
        posTwo.y + sizeTwo.y / 2.0f,
        posTwo.z + sizeTwo.z / 2.0f);
    glm::vec3 playerCollideCenter(
        posOne.x,
        posOne.y - playerH + sizeTwo.y / 2.0f,
        posOne.z);

    glm::vec3 collisionRay;

    if (playerMins.x < aabbMaxs.x && playerMaxs.x > aabbMins.x &&
        playerMins.y < aabbMaxs.y && playerMaxs.y > aabbMins.y &&
        playerMins.z < aabbMaxs.z && playerMaxs.z > aabbMins.z)
    {
        // std::cout << glm::to_string(aabbMins) << " " << glm::to_string(aabbMaxs) << std::endl;
        collisionRay = aabbCenter - playerCollideCenter;
        if (std::abs(collisionRay.x) > std::abs(collisionRay.z) && std::abs(collisionRay.x) >= std::abs(collisionRay.y) && ((velocity.x > 0 && collisionRay.x > 0) || (velocity.x<0 && collisionRay.x <0)))
        {
            velocity.x = 0;
        }
        else if (std::abs(collisionRay.z) > std::abs(collisionRay.x) && std::abs(collisionRay.z) >= std::abs(collisionRay.y) && ((velocity.z > 0 && collisionRay.z > 0) || (velocity.z < 0 && collisionRay.z <0)))
        {
            velocity.z = 0;
        }
        else if (std::abs(collisionRay.y) > std::abs(collisionRay.x) && std::abs(collisionRay.y) > std::abs(collisionRay.z))
        {
            velocity.y = 0;
            isOnGround = true;
            setPositionY(&cameraPos);
        }
    }
}

void Character::setPositionY(glm::vec3 *pos)
{
    (*pos).y = std::ceilf((*pos).y * 2.0f) / 2.0f;
}

void Character::processInput(GLFWwindow *window, float deltaTime)
{
    velocity = glm::vec3(0.0f, velocity.y, 0.0f);
    cameraSpeed = 2.0f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        velocity += glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        velocity -= glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        velocity -= glm::normalize(glm::cross(cameraFront, cameraUp));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        velocity += glm::normalize(glm::cross(cameraFront, cameraUp));
    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
    {
        gravitySpeed = 0.0f;
        cameraPos.y = 2.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        std::cout << "Position: " << glm::to_string(cameraPos) << std::endl;
    }
}

void Character::handleGravity(GLFWwindow *window, float deltaTime, bool gravityEnabled)
{
    if (gravityEnabled)
    {
        velocity.y += gravity * deltaTime;
        if (isOnGround)
        {
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            {
                velocity.y = jumpStrength;
            }
        }
    }
    else
    {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            cameraPos.y += jumpStrength * deltaTime;
        }
        else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            cameraPos.y -= jumpStrength * deltaTime;
        }
    }
}

void Character::move(float deltaTime)
{
    cameraPos += velocity * cameraSpeed * deltaTime;
}