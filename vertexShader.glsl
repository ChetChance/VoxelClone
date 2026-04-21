#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aTextureIndex;
layout (location = 3) in float shadowFactor;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

flat out int TexIndex;
out vec2 TexCoord;
out float shadows;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    TexCoord = aTexCoord;
    shadows = shadowFactor;
    TexIndex = int(aTextureIndex + 0.5);
}