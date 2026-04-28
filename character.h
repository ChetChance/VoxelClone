#ifndef CHARACTER_H
#define CHARACTER_H

#include "cube.h"
#include <GLFW/glfw3.h>
#include <glm/gtx/quaternion.hpp>
#include "chunk.h"

class Character
{
public:
    glm::mat4 view;
    float playerHeight;
    float playerRadius;
    glm::vec3 cameraPos = glm::vec3(1.0f, 0.0f, 1.0f);
    Character(const float playerRadius, const float playerHeight, float gravity, float jumpStrength, float Sensitivity, float speed, bool fly, glm::vec3 startPosition);
    void update(Shader &shader, GLFWwindow *window, float deltaTime, bool gravityEnabled);
    void mouse_Handler(GLFWwindow *window, double xpos, double ypos);
    void handleCollision(const std::vector<glm::vec3> &collisionCubes, bool doCollision);
    void CheckCollision(glm::vec3 posOne, float playerReach, float playerH, glm::vec3 posTwo, glm::vec3 sizeTwo);
    void setPositionY(glm::vec3 *pos);
    void processInput(GLFWwindow *window, float deltaTime);
    void handleGravity(GLFWwindow *window, float deltaTime, bool gravityEnabled);
    void move(float deltaTime);
    void checkRayCollision(Chunk &chunk, GLFWwindow *window, Shader &shader);
    bool TestRayOBBIntersection(
        glm::vec3 ray_origin,        // Ray origin, in world space
        glm::vec3 ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
        glm::vec3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
        glm::vec3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
        glm::mat4 ModelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
        float &intersection_distance // Output : distance between ray_origin and the intersection with the OBB
    );

private:
    glm::vec3 velocity;
    float gravitySpeed;
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float gravity;
    float yaw = 180.0f;
    float pitch = 45.0f;
    struct MouseState
    {
        double x = 0.0, y = 0.0;
    } preMouse;
    float mouseSensitivity;
    float cameraSpeed;
    float jumpStrength;
    bool fly;
    bool isOnGround = false;
    float previousClosestDistance = 100000.0f;
};

#endif