#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
//in vec4 fragColor;
in vec3 fragNormal;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables
uniform vec3 sun_pos;
uniform vec4 sun_color;

uniform float specular_coef = 16.0;
uniform float ambient_intensity = 0.1;
uniform vec3 viewPos;

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor = texture(texture0, fragTexCoord);
    vec3 lightDot = vec3(0.0);
    vec3 normal = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 specular = vec3(0.0);
    vec4 ambient = vec4(ambient_intensity, ambient_intensity, ambient_intensity, 1.0);

    // NOTE: Implement here your fragment shader code

    vec3 lightDir = normalize(sun_pos - fragPosition);

    float NdotL = max(dot(normal, lightDir), 0.0);
    lightDot = sun_color.rgb * NdotL;

    float specCo = 0.0;
    if (NdotL > 0.0 && specular_coef > 0) specCo = pow(max(0.0, dot(viewDir, reflect(-(lightDir), normal))), specular_coef); // 16 refers to shine
    specular += specCo;

    finalColor = (texelColor * ((colDiffuse + vec4(specular, 1.0)) * vec4(lightDot, 1.0)));
    finalColor += texelColor * ambient * colDiffuse;

    // Gamma correction
    finalColor = pow(finalColor, vec4(1.0/2.2));
}