#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

const vec2 size = vec2(800, 450);   // Framebuffer size
const float samples = 100.0;          // Pixels per axis; higher = bigger glow, worse performance
const float quality = 0.1;          // Defines size factor: Lower = smaller glow, better quality

//Extreme MaxStars=200% S=100;Q=0.1
//Normal MaxStars=100% S=50;Q=0.2
//Average MaxStars=75% S=10;Q=1.5
//Reduced MaxStars=50% S= 5;Q=3.5

void main()
{
    vec4 sum = vec4(0);
    vec2 sizeFactor = vec2(1)/size*quality;

    // Texel color fetching from texture sampler
    vec4 source = texture(texture0, fragTexCoord);

    const int range = int((samples - 1.0)/2.0);

    for (int x = -range; x <= range; x++)
    {
        for (int y = -range; y <= range; y++)
        {
            sum += texture(texture0, fragTexCoord + vec2(x, y)*sizeFactor);
        }
    }

    // Calculate final fragment color
    finalColor = ((sum/(samples*samples)) + source)*colDiffuse;
}