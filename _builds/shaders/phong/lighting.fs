#version 330

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

uniform sampler2D texture0;

uniform float ambientAttenuation = 10.0;

uniform vec3  sunLocation;
uniform vec4  sunColor;

uniform float specularExponent = 32.0;

out vec4 finalColor;

uniform vec3 viewPos;

void main() {
    vec4 baseColor = texture(texture0, fragTexCoord);
    finalColor = baseColor / ambientAttenuation;

    vec3 sunDirection = normalize(sunLocation - fragPosition);
    float diffuseAmount = max(dot(normalize(fragNormal), sunDirection), 0.0);
    finalColor += (sunColor * baseColor) * diffuseAmount;

    if (diffuseAmount > 0.0 && specularExponent > 0.0){
        vec3 viewDirection = normalize(viewPos - fragPosition);
        vec3 lightDirection = normalize(sunLocation - fragPosition);
        float specularCoefficient = pow(max(0.0, dot(viewDirection, reflect(-(lightDirection), normalize(fragNormal)))), specularExponent);

        finalColor += vec4(vec3(specularCoefficient), 1.0) * sunColor;
    }

    finalColor = min(finalColor, 1.0);
    finalColor.w = 1.0; //Ensure no transparent things
}