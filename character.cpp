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
    isOnGround = false;
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
        playerMins.y <= aabbMaxs.y && playerMaxs.y >= aabbMins.y &&
        playerMins.z < aabbMaxs.z && playerMaxs.z > aabbMins.z)
    {
        // std::cout << glm::to_string(aabbMins) << " " << glm::to_string(aabbMaxs) << std::endl;
        collisionRay = aabbCenter - playerCollideCenter;
        if (std::abs(collisionRay.x) > std::abs(collisionRay.z) && std::abs(collisionRay.x) >= std::abs(collisionRay.y) && ((velocity.x > 0 && collisionRay.x > 0) || (velocity.x < 0 && collisionRay.x < 0)))
        {
            velocity.x = 0;
        }
        else if (std::abs(collisionRay.z) > std::abs(collisionRay.x) && std::abs(collisionRay.z) >= std::abs(collisionRay.y) && ((velocity.z > 0 && collisionRay.z > 0) || (velocity.z < 0 && collisionRay.z < 0)))
        {
            velocity.z = 0;
        }
        else if (std::abs(collisionRay.y) > std::abs(collisionRay.x) && std::abs(collisionRay.y) > std::abs(collisionRay.z))
        {
            isOnGround = true;
            setPositionY(&cameraPos);
        }
    }
}

void Character::checkRayCollision(Chunk &chunk, GLFWwindow *window, Shader &shader)
{
    int lcPress = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    int rcPress = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    float intersectionDistance;
    glm::vec3 closestBlock = glm::vec3(0.0f);
    previousClosestDistance = 100000.0f;
    glm::vec3 intersectionPlane;

    if (lcPress == GLFW_PRESS)
    {
        for (glm::vec3 &block : chunk.blockPositions)
        {
            if (TestRayOBBIntersection(cameraPos, cameraFront, block, glm::vec3(block.x + 0.5f, block.y + 0.5f, block.z + 0.5f), glm::mat4(1), intersectionDistance, intersectionPlane))
            {
                if (intersectionDistance < previousClosestDistance)
                {
                    previousClosestDistance = intersectionDistance;
                    closestBlock = glm::vec3((block.x - chunk.chunkPosition.x) * 2.0f, -(block.y - chunk.chunkPosition.y) * 2.0f, (block.z - chunk.chunkPosition.z) * 2.0f);
                }
            }
        }
        if (previousClosestDistance <= 10.0f) // If an intersection was found
        {
            //std::cout << "Closest Intersection: " << previousClosestDistance << std::endl;
            //std::cout << "Block at " << glm::to_string(closestBlock) << " was hit! Distance: " << intersectionDistance << std::endl;
            chunk.updateBlock(0, closestBlock, shader); // Set block to air
        }
    }
    else if (rcPress == GLFW_PRESS)
    {
        for (glm::vec3 &block : chunk.blockPositions)
        {
            if (TestRayOBBIntersection(cameraPos, cameraFront, block, glm::vec3(block.x + 0.5f, block.y + 0.5f, block.z + 0.5f), glm::mat4(1), intersectionDistance, intersectionPlane))
            {
                if (intersectionDistance < previousClosestDistance)
                {
                    previousClosestDistance = intersectionDistance;
                    //std::cout << "Intersection Plane: " << glm::to_string(intersectionPlane) << std::endl;
                    closestBlock = glm::vec3((block.x - chunk.chunkPosition.x) * 2.0f, -(block.y - chunk.chunkPosition.y) * 2.0f, (block.z - chunk.chunkPosition.z) * 2.0f) - intersectionPlane;
                }
            }
        }
        if (previousClosestDistance <= 10.0f && previousClosestDistance > playerRadius) // If an intersection was found
        {
            //std::cout << "Closest Intersection: " << previousClosestDistance << std::endl;
            //std::cout << "Block at " << glm::to_string(closestBlock) << " was hit! Distance: " << intersectionDistance << std::endl;
            chunk.updateBlock(1, closestBlock, shader); // Set block to grass
        }
    }
    else if (lcPress == GLFW_RELEASE)
    {
        previousClosestDistance = 100000.0f;
    }
}

bool Character::TestRayOBBIntersection(
    glm::vec3 ray_origin,        // Ray origin, in world space
    glm::vec3 ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
    glm::vec3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
    glm::vec3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
    glm::mat4 ModelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
    float &intersection_distance, // Output : distance between ray_origin and the intersection with the OBB
    glm::vec3 &intersection_plane
)
{

    // Intersection method from Real-Time Rendering and Essential Mathematics for Games

    float tMin = 0.0f;
    float tMax = 100000.0f;

    glm::vec3 OBBposition_worldspace(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);

    glm::vec3 delta = OBBposition_worldspace - ray_origin;

    // Test intersection with the 2 planes perpendicular to the OBB's X axis
    {
        glm::vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
        float e = glm::dot(xaxis, delta);
        float f = glm::dot(ray_direction, xaxis);

        if (fabs(f) > 0.001f)
        { // Standard case

            float t1 = (e + aabb_min.x) / f; // Intersection with the "left" plane
            float t2 = (e + aabb_max.x) / f; // Intersection with the "right" plane
            // t1 and t2 now contain distances betwen ray origin and ray-plane intersections

            // We want t1 to represent the nearest intersection,
            // so if it's not the case, invert t1 and t2
            if (t1 > t2)
            {
                float w = t1;
                t1 = t2;
                t2 = w; // swap t1 and t2
                xaxis = -xaxis; // invert xaxis to get the correct normal
            }

            // tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
            if (t2 < tMax)
                tMax = t2;
            // tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
            if (t1 > tMin){
                tMin = t1;
                intersection_plane = xaxis;
            }

            // And here's the trick :
            // If "far" is closer than "near", then there is NO intersection.
            // See the images in the tutorials for the visual explanation.
            if (tMax < tMin)
                return false;
        }
        else
        { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
            if (-e + aabb_min.x > 0.0f || -e + aabb_max.x < 0.0f)
                return false;
        }
    }

    // Test intersection with the 2 planes perpendicular to the OBB's Y axis
    // Exactly the same thing than above.
    {
        glm::vec3 yaxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);
        float e = glm::dot(yaxis, delta);
        float f = glm::dot(ray_direction, yaxis);

        if (fabs(f) > 0.001f)
        {

            float t1 = (e + aabb_min.y) / f;
            float t2 = (e + aabb_max.y) / f;

            if (t1 > t2)
            {
                float w = t1;
                t1 = t2;
                t2 = w;
            }else
            {
                yaxis = -yaxis;
            }

            if (t2 < tMax)
                tMax = t2;
            if (t1 > tMin){
                tMin = t1;
                intersection_plane = yaxis;
            }
            if (tMin > tMax)
                return false;
        }
        else
        {
            if (-e + aabb_min.y > 0.0f || -e + aabb_max.y < 0.0f)
                return false;
        }
    }

    // Test intersection with the 2 planes perpendicular to the OBB's Z axis
    // Exactly the same thing than above.
    {
        glm::vec3 zaxis(ModelMatrix[2].x, ModelMatrix[2].y, ModelMatrix[2].z);
        float e = glm::dot(zaxis, delta);
        float f = glm::dot(ray_direction, zaxis);

        if (fabs(f) > 0.001f)
        {

            float t1 = (e + aabb_min.z) / f;
            float t2 = (e + aabb_max.z) / f;

            if (t1 > t2)
            {
                float w = t1;
                t1 = t2;
                t2 = w;
                zaxis = -zaxis;
            }

            if (t2 < tMax)
                tMax = t2;
            if (t1 > tMin){
                tMin = t1;
                intersection_plane = zaxis;
            }
            if (tMin > tMax)
                return false;
        }
        else
        {
            if (-e + aabb_min.z > 0.0f || -e + aabb_max.z < 0.0f)
                return false;
        }
    }

    intersection_distance = tMin;
    return true;
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
        if (isOnGround)
        {
            velocity.y = 0.0f;
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            {
                velocity.y = jumpStrength;
            }
        }else{
            velocity.y += gravity * deltaTime;
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

glm::vec3 Character::SnapToAxis(const glm::vec3& v) {
    float absX = std::abs(v.x);
    float absY = std::abs(v.y);
    float absZ = std::abs(v.z);

    glm::vec3 result = glm::vec3(0.0f);

    if (absX >= absY && absX >= absZ) {
        result.x = (v.x >= 0) ? 1.0f : -1.0f;
    } else if (absY >= absX && absY >= absZ) {
        result.y = (v.y >= 0) ? 1.0f : -1.0f;
    } else {
        result.z = (v.z >= 0) ? 1.0f : -1.0f;
    }

    return result;
}