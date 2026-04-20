#include "chunk.h"

Chunk::Chunk(unsigned int chunkSize, Shader &shader, glm::vec3 chunkPosition)
{
    this->chunkSize = chunkSize;
    this->chunkPosition = chunkPosition;

    for (int i = 0; i < chunkSize; i++)
    {
        for (int j = 0; j < chunkSize; j++)
        {
            for (int k = 0; k < chunkSize; k++)
            {
                int yOffset = rand() % 2 + 1;
                if (k + yOffset > 6)
                    blockVal.emplace_back(cobblestoneVal);
                else if (k + yOffset > 3)
                    blockVal.emplace_back(grassVal);
                else
                    blockVal.emplace_back(airVal);
            }
        }
    }

    model = glm::translate(model, chunkPosition);
    bufferizeChunkMesh(blockVal, shader);
}

void Chunk::bufferizeChunkMesh(std::vector<unsigned char> blockVal, Shader &shader)
{
    buildChunkMesh(blockVal, chunkVertices);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, chunkVertices.size() * sizeof(float), chunkVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    // std::cout << "Chunk mesh generated with " << chunkVertices.size() / 6 << " vertices." << std::endl;
}

void Chunk::buildChunkMesh(const std::vector<unsigned char> &blockVal, std::vector<float> &meshVertices)
{
    Cube prototype;
    prototype.init(glm::vec3(0.0f), 0);

    struct FaceInfo
    {
        int dx, dy, dz, offset;
    };
    static const FaceInfo faces[] = {
        {1, 0, 0, 90},   // right
        {-1, 0, 0, 60},  // left
        {0, 0, 1, 0},    // front
        {0, 0, -1, 30},  // back
        {0, -1, 0, 150}, // top
        {0, 1, 0, 120}   // bottom
    };

    meshVertices.reserve(chunkSize * chunkSize * chunkSize * 24);

    for (unsigned int x = 0; x < chunkSize; x++)
    {
        for (unsigned int y = 0; y < chunkSize; y++)
        {
            for (unsigned int z = 0; z < chunkSize; z++)
            {

                if (!isBlockFilled(x, y, z, blockVal))
                    continue;

                glm::vec3 worldPos = glm::vec3(x * 0.5f, -((float)y) * 0.5f, z * 0.5f);
                int textureIndex = (blockVal[gridIndex(x, y, z)] == cobblestoneVal) ? 1 : 0;

                for (const FaceInfo &face : faces)
                {
                    if (!isBlockFilled(x + face.dx, y + face.dy, z + face.dz, blockVal))
                    {
                        for (int i = face.offset; i < face.offset + 30; i += 5)
                        {
                            meshVertices.push_back(prototype.vertices[i + 0] + worldPos.x);
                            meshVertices.push_back(prototype.vertices[i + 1] + worldPos.y);
                            meshVertices.push_back(prototype.vertices[i + 2] + worldPos.z);
                            meshVertices.push_back(prototype.vertices[i + 3]);
                            meshVertices.push_back(prototype.vertices[i + 4]);
                            meshVertices.push_back((float)textureIndex);
                        }
                    }
                }
            }
        }
    }
}

inline size_t Chunk::gridIndex(unsigned int x, unsigned int y, unsigned int z)
{
    return (size_t)x * chunkSize * chunkSize + (size_t)z * chunkSize + y;
}

// heightmap index is the same as grid index since heightmap is 2D and only depends on x and z
inline size_t Chunk::heightMapIndex(unsigned int x, unsigned int z)
{
    return (size_t)x * chunkSize + z;
}

inline bool Chunk::isBlockFilled(int x, int y, int z, const std::vector<unsigned char> &blockVal)
{
    if (x < 0 || y < 0 || z < 0 || x >= (int)chunkSize || y >= (int)chunkSize || z >= (int)chunkSize)
        return false;
    unsigned char value = blockVal[gridIndex((unsigned int)x, (unsigned int)y, (unsigned int)z)];
    return value == grassVal || value == cobblestoneVal;
}

unsigned char Chunk::checkBlock(glm::vec3 pos, float stepSize, const std::vector<unsigned char> &blockVal)
{
    int x = static_cast<int>(pos.x * stepSize);
    int y = static_cast<int>(pos.y * stepSize);
    int z = static_cast<int>(pos.z * stepSize);
    if (x >= 0 && y >= 0 && z >= 0 && x < (int)chunkSize && y < (int)chunkSize && z < (int)chunkSize)
    {
        return blockVal[gridIndex((unsigned int)x, (unsigned int)y, (unsigned int)z)];
    }
    return 'a';
}

void Chunk::update(Shader &shader, unsigned int grassTexture, unsigned int cobbleTexture)
{
    shader.setMat4("model", glm::value_ptr(model));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grassTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, cobbleTexture);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(chunkVertices.size() / 6));
}