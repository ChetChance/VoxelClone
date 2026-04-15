#include "cube.h"

void Cube::init(glm::vec3 pos, unsigned int texture)
{
    textureID = texture;
    position = pos;
    CubeID = pos;
    size = glm::vec3(0.5f, 0.5f, 0.5f);
    float tempVertices[] = {
        // FRONT (z = 0.5)
        0.0f, 0.0f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.5f, 0.0f, 0.0f,

        // BACK (z = 0.0)
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f,

        // LEFT (x = 0.0)
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        // RIGHT (x = 0.5)
        0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        // BOTTOM (y = 0.0)
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.5f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        // TOP (y = 0.5)
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 1.0f};
    std::copy(std::begin(tempVertices), std::end(tempVertices), vertices);
}

void Cube::bufferize()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // An EBO is a buffer, just like a vertex buffer object, that stores indices that OpenGL uses to decide what vertices to draw.
    // unsigned int EBO;
    // glGenBuffers(1, &EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //  position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Cube::draw(Shader &shader, bool aCube, bool bCube, bool pxCube, bool nxCube, bool pzCube, bool nzCube)
{
    model = glm::translate(glm::mat4(1.0f), position);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(VAO);
    shader.setMat4("model", glm::value_ptr(model));
    if (aCube)
    {
        if (bCube)
        {
            if (nxCube)
            {
                if(pxCube)
                {
                    if(pzCube)
                    {
                        if(nzCube)
                        {
                            glDrawArrays(GL_TRIANGLES, 0, 0);
                        }
                        else
                        {
                            glDrawArrays(GL_TRIANGLES, 0, 6);
                        }
                    }
                    else
                    {
                        glDrawArrays(GL_TRIANGLES, 0, 12);
                    }
                }
                else
                {
                    glDrawArrays(GL_TRIANGLES, 0, 18);
                }
            }
            else
            {

                glDrawArrays(GL_TRIANGLES, 0, 24);
            }
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, 30);
        }
    }

    else if (bCube)
    {
        glDrawArrays(GL_TRIANGLES, 0, 24);
        glDrawArrays(GL_TRIANGLES, 30, 36);
    }
    else if (pxCube)
    {
        glDrawArrays(GL_TRIANGLES, 0, 18);
        glDrawArrays(GL_TRIANGLES, 24, 36);
    }
    else if (nxCube)
    {
        glDrawArrays(GL_TRIANGLES, 0, 12);
        glDrawArrays(GL_TRIANGLES, 18, 36);
    }
    else if (pzCube)
    {
        glDrawArrays(GL_TRIANGLES, 6, 36);
    }
    else if (nzCube)
    {
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawArrays(GL_TRIANGLES, 12, 36);
    }
}