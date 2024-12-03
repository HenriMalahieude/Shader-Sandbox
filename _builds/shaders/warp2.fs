#version 330

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;

uniform sampler2D texture0;
uniform vec2 warpLocation = vec2(0.5, 0.6); //0 -> 1
uniform float warpSize = 0.01;
uniform float warpRadius = 0.05;
uniform float yxratio = 1.0;

//Stolen from Stack Overflow
float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

out vec4 finalColor;
void main(){
    vec4 baseColor = texture(texture0, fragTexCoord);

    vec2 lclDist = vec2(fragTexCoord.x, fragTexCoord.y*yxratio);
    vec2 lclSorc = vec2(warpLocation.x, warpLocation.y*yxratio);
    float dist = length(lclSorc - lclDist);

    if (dist < warpRadius) baseColor = vec4(0);
    else if (dist < (warpRadius+warpSize)) { //this is where we get all on a dir
        vec4 warpColor = vec4(0);
        vec2 dir = warpLocation - fragTexCoord;
        float intensity = 1 - ((dist - warpRadius) / warpSize);

        warpColor = texture(texture0, fragTexCoord + dir * intensity);
        warpColor.w = min(1-intensity + 0.35, 1);

        baseColor = warpColor;
    }

    finalColor = baseColor;
}