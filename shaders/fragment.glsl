#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D spriteTexture;

void main()
{
    vec4 texColor = texture(spriteTexture, TexCoord);
    if(texColor.a < 0.1)
        discard; // Transparência para evitar o fundo preto

    FragColor = texColor;
}