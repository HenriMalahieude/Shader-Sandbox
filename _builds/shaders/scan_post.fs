#version 330

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;

vec2 windowSize;
uniform sampler2D texture0;
uniform float scanGrid = 128;

uniform float scan_size = 0.02;
uniform float position = 0.5;
uniform vec4 scan_fac = vec4(0.1, 0.1, 1, 1);

vec2 gridify(vec2 perc, vec2 sizes, float grid_size) {
	return floor(vec2(perc.x * sizes.x, perc.y * sizes.y) / grid_size);
}

out vec4 finalColor;
void main() {
    windowSize = textureSize(texture0, 0);
    vec4 baseColor = texture(texture0, fragTexCoord);

    vec2 gridpos = gridify(fragTexCoord, windowSize.xy, windowSize.x/scanGrid);

    float distx = abs(position*scanGrid - gridpos.x) / scanGrid;
    if (distx <= (scan_size)) {
        float xfac = (distx / (scan_size)); //center is strongest
        //vec4 delta = scan_col*xfac; delta.w = 1;
        baseColor *= (1-scan_fac)*(xfac);
        baseColor.w = 1;
    }
       // Output to screen
    finalColor = baseColor;
}