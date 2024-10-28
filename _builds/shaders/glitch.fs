#version 330

//Input Vertex Info
in vec2 fragTexCoord; //from vertexTexCoord

uniform sampler2D texture0;

out vec4 finalColor;

//Custom
ivec2 windowSize;
//const float offset = 0.01;
uniform float min_row_perc = 0.05;
uniform float time = 0.0;
uniform float rad_chance = 0.05;
uniform float dead_chance = 0.01;
uniform float dead_size = 6;
uniform int row_size = 7;

//Stolen from Stack Overflow
float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

vec4 rgbbChooser(float rng, float r, float g, float b) {
	if (rng < r) return vec4(1, 0, 0, 1);
	if (rng >= r && rng < g) return vec4(0, 1, 0, 1);
	if (rng >= g && rng < b) return vec4(0, 0, 1, 1);
	return vec4(0, 0, 0, 1);
}

vec2 gridify(vec2 perc, vec2 sizes, float grid_size) {
	return floor(vec2(perc.x * sizes.x, perc.y * sizes.y) / grid_size);
}

void main() {
	windowSize = textureSize(texture0, 0);

	//Dead Pixel or Not
	vec2 grid_dead = gridify(fragTexCoord, windowSize, dead_size);

	float lcl_chnce = rand(grid_dead);
	if (lcl_chnce >= dead_chance) {
		float row = floor((fragTexCoord.y * windowSize.x) / row_size);

		float grider_rng = rand(vec2(row, time)); //is row irradiated?

		float size_rng = rand(vec2(time, grider_rng)); //where is center of "radiation"
		size_rng = (size_rng + min_row_perc) / (1 + min_row_perc);
		size_rng /= 2;

		float center_rng = rand(vec2(row, grider_rng)); //how far does radiation affect?
		if (grider_rng < rad_chance && abs(center_rng - fragTexCoord.x) < size_rng) {
			float type = grider_rng / rad_chance;

			vec2 grid_rad = gridify(fragTexCoord, windowSize, row_size/2.0);

			float r = rand(vec2(type, 1) * grid_rad);
			float g = rand(vec2(time, 1) * grid_rad);
			float b = rand(vec2(time, grid_rad.x));

			finalColor = vec4(r, g, b, 1);
			//finalColor.w = (size_rng + 0.1) / 1.1;
		} else {
			finalColor = vec4(0, 0, 0, 0);
		} // */
	} else {
		//Dead Pixels
		float type = rand(fragTexCoord);
		finalColor = rgbbChooser(type, 0.1, 0.3, 0.45);
	}
}