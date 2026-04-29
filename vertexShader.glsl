#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in float aTextureIndex;
layout (location = 4) in float shadowFactor;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

flat out int TexIndex;
out vec2 TexCoord;
out vec3 Normal;
out vec3 Position;
out vec3 FragPos;

void main()
{
    FragPos = aPos;
    Position = vec3(model * vec4(aPos, 1.0f));
    TexCoord = aTexCoord;
    TexIndex = int(aTextureIndex + 0.5);
    //FragPos = aPos; // Pass the fragment position to the fragment shader for lighting calculations
    Normal = aNormal; // Transform the normal vector to world space
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}