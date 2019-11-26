#version 120

uniform sampler2D fbo_texture;
uniform float offset;
varying vec2 f_texCoord;

void main(void) {
    vec2 texCoord = f_texCoord;
    texCoord.x += sin(texCoord.y * 4*2*3.14159 + offset) / 100;
    gl_FragColor = texture2D(fbo_texture, texCoord);
}