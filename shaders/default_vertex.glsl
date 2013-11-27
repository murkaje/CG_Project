varying vec3 N;
varying vec4 v;
varying vec4 v1;

uniform float time;

uniform bool light0;
uniform bool light1;
uniform bool light2;
uniform bool light3;
uniform bool light4;
uniform bool light5;
uniform bool light6;
uniform bool light7;

uniform int lighting_enabled;

void main(void) {
    v = gl_ModelViewMatrix * gl_Vertex;
    N = normalize(gl_NormalMatrix * gl_Normal);
    v1 = gl_Vertex;

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
