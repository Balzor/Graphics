#version 150

uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 position;
in vec2 texCoord;

out Vertex {
    vec2 texCoord;
} OUT;

void main(void) {
    mat4 mvp = projMatrix;
    gl_Position = mvp * vec4(position, 1.0);
    OUT.texCoord = texCoord;
}