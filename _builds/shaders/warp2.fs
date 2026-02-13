#version 330

in vec2 fragTexCoord;

uniform sampler2D texture0;
uniform sampler2D positionTex; 
uniform vec2 warpLocation = vec2(0.5, 0.6); //0 -> 1
uniform float warpSize = 0.01;
uniform float warpRadius = 0.05;
uniform float yxratio = 1.0;

out vec4 finalColor;

void main(){
    float frontFactor = texture(positionTex, fragTexCoord).r;
    vec4 baseColor = texture(texture0, fragTexCoord);

    vec2 lclDist = vec2(fragTexCoord.x, fragTexCoord.y*yxratio);
    vec2 lclSorc = vec2(warpLocation.x, warpLocation.y*yxratio);
    float dist = length(lclSorc - lclDist);
    
    vec4 warpColor = vec4(0);
    vec2 dir = warpLocation - fragTexCoord;
    float intensity = 1 - ((dist - warpRadius) / warpSize);
    intensity *= (1-frontFactor); // reduce intensity for fragments that are in front

    warpColor = texture(texture0, fragTexCoord + dir * intensity);
    warpColor.w = min(1-intensity + 0.35, 1);
    
    if (dist <= warpRadius*1.1) { 
        if (frontFactor > 0.5) baseColor = warpColor;
        else baseColor = vec4(vec3(0.0), 1.0);
    } else if (dist < warpRadius + warpSize) {
        baseColor = warpColor;
    }
    
    //vec4 baseColor = vec4(fragPosition, 1.0);
    finalColor = baseColor;
}