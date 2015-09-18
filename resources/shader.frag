//primitive color
varying vec4 fragColor;
varying vec4 fragCoord;
varying vec2 fragUv;
varying float lightIntensity;

uniform bool do_texture;

uniform float time;
uniform float aspect;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D texture;


void main(void) {

    gl_FragColor = fragColor;
}

