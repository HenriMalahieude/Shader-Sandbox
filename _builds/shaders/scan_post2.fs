#version 330

in vec3 fragPosition;
in vec3 fragLocalPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;

//settings
uniform sampler2D texture0;
uniform float scan_size = 2.0; //in grid units
uniform float scanGrid = 128;
uniform vec4 scan_factor = vec4(0.1, 0.1, 1, 1);

//dynamic values
uniform vec3 camPos;
uniform float completion = 0.5;

vec2 gridify(vec2 perc, vec2 sizes, float grid_size) {
    return floor(vec2(perc.x * sizes.x, perc.y * sizes.y) / grid_size);
}

out vec4 finalColor;
void main() {
    vec4 baseColor = texture(texture0, fragTexCoord);
    
    float scanPos = mix(-1.0*(scanGrid/2.0), (scanGrid/2.0), completion);
    
    // Create a smooth band around the scan position
    float distToScan = abs(fragLocalPosition.x - scanPos);
    float scanInfluence = smoothstep(scan_size, 0.0, distToScan);
    
    // Apply blue shift based on scan influence
    vec3 blueShift = vec3(0.1, 0.2, 1.0); // Blue color
    vec3 scannedColor = baseColor.rgb * blueShift * scanInfluence;
    
    finalColor = vec4(scannedColor, baseColor.a);
}