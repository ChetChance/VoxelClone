#version 410 core
out vec4 FragColor;

flat in int TexIndex;
in vec2 TexCoord;

uniform sampler2D grassTexture;
uniform sampler2D cobbleTexture;

void main()
{
    vec4 grassColor = texture(grassTexture, TexCoord);
    vec4 cobbleColor = texture(cobbleTexture, TexCoord);
    FragColor = (TexIndex == 1) ? cobbleColor : grassColor;
}