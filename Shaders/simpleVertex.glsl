#version 120

attribute vec2 v_Coord;
uniform sampler2D fbo_texture;
varying vec2 f_texCoord;

void main(void) {
    gl_Position = vec4(v_Coord, 0.0, 1.0);
    f_texCoord = (v_Coord + 1.0) / 2.0;
}