#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

//Custom Vars
uniform vec2 windowSize;

uniform float viewFovy;
uniform vec3 viewPos;
uniform vec3 viewDir;

uniform vec3 sunPos;

void main() {

}