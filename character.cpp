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

void Character::update(Shader &shader, GLFWwindow *window, Cube collisionCubes[], int numCubes, float deltaTime, bool doCollision, bool gravityEnabled)
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    shader.setMat4("view", glm::value_ptr(view));
    collideVec = glm::vec3(0.0f, 0.0f, 0.0f);
    if (doCollision)
    {
        /* code */

        for (size_t i = 0; i < numCubes; i++)
        {
            collideVec += CheckCollision(cameraPos, playerRadius, playerHeight, collisionCubes[i].position, collisionCubes[i].size);
        }
    }

    processInput(window, deltaTime);
    handleGravity(window, deltaTime, gravityEnabled);
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

glm::vec3 Character::CheckCollision(glm::vec3 posOne, float playerReach, float playerH, glm::vec3 posTwo, glm::vec3 sizeTwo) // AABB - AABB collision
{
    glm::vec3 playerMins(
        posOne.x - playerReach,
        posOne.y - playerH,
        posOne.z - playerReach);
    glm::vec3 playerMaxs(
        posOne.x + playerReach,
        posOne.y - playerH + playerReach,
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

    if (glm::abs(playerMins.x) < glm::abs(aabbMaxs.x) && glm::abs(playerMaxs.x) > glm::abs(aabbMins.x) &&
        glm::abs(playerMins.y) < glm::abs(aabbMaxs.y) && glm::abs(playerMaxs.y) > glm::abs(aabbMins.y) &&
        glm::abs(playerMins.z) < glm::abs(aabbMaxs.z) && glm::abs(playerMaxs.z) > glm::abs(aabbMins.z))
    {
        // std::cout << glm::to_string(aabbMins) << " " << glm::to_string(aabbMaxs) << std::endl;
        collisionRay = aabbCenter - playerCollideCenter;
        if (std::abs(collisionRay.x) > std::abs(collisionRay.z) && std::abs(collisionRay.x) >= std::abs(collisionRay.y))
        {
            collisionRay.z = 0.0f;
            collisionRay.y = 0.0f;
        }
        else if (std::abs(collisionRay.z) > std::abs(collisionRay.x) && std::abs(collisionRay.z) >= std::abs(collisionRay.y))
        {
            collisionRay.x = 0.0f;
            collisionRay.y = 0.0f;
        }
        else if (std::abs(collisionRay.y) > std::abs(collisionRay.x) && std::abs(collisionRay.y) > std::abs(collisionRay.z))
        {
            collisionRay.x = 0.0f;
            collisionRay.z = 0.0f;
        }
        else
        {
            collisionRay = glm::vec3(0.0f, 0.0f, 0.0f);
        }
        return collisionRay;
    }

    // retrieve vector between center circle and closest point AABB and check if length <= radius
    return glm::vec3(0.0f, 0.0f, 0.0f);
}

void Character::setPositionY(glm::vec3 *pos)
{
    (*pos).y = std::ceilf((*pos).y * 2.0f) / 2.0f;
}

void Character::processInput(GLFWwindow *window, float deltaTime)
{
    velocity = glm::vec3(0.0f, velocity.y, 0.0f);
    glm::vec3 collideVecXZ = glm::vec3(collideVec.x, 0.0f, collideVec.z);
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
    if (collideVecXZ != glm::vec3(0.0f, 0.0f, 0.0f) && velocity != glm::vec3(0.0f, 0.0f, 0.0f))
    {
        velocity = glm::normalize(velocity) - glm::normalize(collideVecXZ);
    }
    cameraPos += glm::vec3(velocity.x, gravitySpeed, velocity.z) * cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        std::cout << "Position: " << glm::to_string(cameraPos) << std::endl;
    }
}

void Character::handleGravity(GLFWwindow *window, float deltaTime, bool gravityEnabled)
{
    if (gravityEnabled)
    {
        gravitySpeed += gravity * deltaTime;
        if (collideVec.y != 0.0f)
        {
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            {
                gravitySpeed = jumpStrength;
            }
            else
            {
                gravitySpeed = 0.0f;
                setPositionY(&cameraPos);
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