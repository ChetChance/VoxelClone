#ifndef CHUNK_H
#define CHUNK_H

#include "cube.h"

inline unsigned char airVal = 0;
inline unsigned char grassVal = 1;
inline unsigned char cobblestoneVal = 2;

class Chunk
{
    public:
        std::vector<float> chunkVertices;
        unsigned int VBO, VAO;

        Chunk(unsigned int chunkSize, Shader& shader, glm::vec3 chunkPosition);
        inline size_t gridIndex(unsigned int x, unsigned int y, unsigned int z);
        inline size_t heightMapIndex(unsigned int x, unsigned int z);
        void bufferizeChunkMesh(std::vector<unsigned char> blockVal, Shader& shader);
        void update(Shader &shader, unsigned int grassTexture, unsigned int cobbleTexture);
    private:
        inline bool isBlockFilled(int x, int y, int z, const std::vector<unsigned char> &blockVal);
        unsigned char checkBlock(glm::vec3 pos, float stepSize, const std::vector<unsigned char> &blockVal);
        void buildChunkMesh(const std::vector<unsigned char> &blockVal, std::vector<float>& meshVertices);
        unsigned int chunkSize;
        std::vector<unsigned char> blockVal;
        glm::vec3 chunkPosition;
        glm::mat4 model = glm::mat4(1.0f);
        const float shadows[6] = { 1.0f, 1.0f, 0.8f, 0.8f, 0.6f, 1.1f };
        
};

#endif