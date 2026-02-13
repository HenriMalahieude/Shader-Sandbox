#version 330

in vec3 fragPosition;
in vec2 fragTexCoord;

uniform vec3 warpSource;
uniform float sourceSize;
uniform vec3 camPos;

out vec4 finalColor;

void main(){
    vec3 camToWrp = normalize(warpSource - camPos);
    vec3 wrpToFrag = normalize(fragPosition - warpSource);
    
    float dist = length(fragPosition - warpSource);
    
    float frontFactor = ((dot(wrpToFrag, camToWrp)*-1)+1.0)/2.0;
    finalColor = vec4(frontFactor, 0, 0, 1.0);
    //if (dist < sourceSize*1.5) finalColor.x *= (dist/(sourceSize*1.5));
}
