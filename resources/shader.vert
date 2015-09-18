attribute vec4 vertColor;
attribute vec4 vertPos;
attribute vec4 vertNormal;
attribute vec2 vertUv;

uniform float time;
uniform float aspect;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D texture;

varying vec4 fragColor;
varying vec4 fragCoord;
varying vec2 fragUv;
varying float lightIntensity;

uniform bool do_texture;

vec4 LightPosition = vec4(0.0, 0.0, -1.0, 1.0);
const float SpecContribution = 0.1;
const float DiffContribution = 1.0 - SpecContribution;

const vec4 ambient = vec4(1.0, 1.0, 1.0, 1.0);

void main(void) {
    vec4 ecPosition = projection * view * model * vertPos;
    
    fragColor = vertColor;
    fragUv = vertUv;
    gl_Position = ecPosition;
}

