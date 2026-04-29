#version 410 core
out vec4 FragColor;

flat in int TexIndex;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 Position;

uniform sampler2D grassTexture;
uniform sampler2D cobbleTexture;
uniform vec3 viewPos;

float specularStrength = 0.15;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(100.0, 200.0, 150.0) - Position + viewPos);
    vec3 lightDir2 = normalize(vec3(-100.0, 200.0, -150.0) - Position + viewPos);
    //vec3 lightUp = normalize(vec3(0.0, -1.0, 0.0));

    float diff = max(dot(norm, lightDir), dot(norm, lightDir2));
    // diff = max(diff, dot(norm, lightUp) * 0.25); // Add some ambient light from below
    vec3 diffuse = diff * vec3(1.0); // Assuming white light for simplicity

    vec3 specLightDir = normalize(vec3(100.0, 50.0, 150.0) - Position + viewPos);
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-specLightDir, norm);
    // // vec3 reflectDir2 = reflect(-lightDir2, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.25), 16);
    // float spec = pow(max(dot(viewDir, reflectDir), dot(viewDir, reflectDir2)), 32);
    //vec3 specular = specularStrength * spec * vec3(1.0); // Assuming white specular highlights

    vec4 grassColor = texture(grassTexture, TexCoord);
    vec4 cobbleColor = texture(cobbleTexture, TexCoord);
    FragColor = ((TexIndex == 1) ? cobbleColor : grassColor) * vec4(diffuse * 1.35 + spec/2, 1.0);
}