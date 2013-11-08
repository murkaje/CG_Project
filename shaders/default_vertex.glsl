varying vec3 N;
varying vec3 v;
varying float d;

uniform float time;

uniform int lighting_enabled;

void main(void) {
    v = vec3(gl_ModelViewMatrix * gl_Vertex);
    N = normalize(gl_NormalMatrix * gl_Normal);
    d = distance(gl_LightSource[0].position, gl_Vertex);

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
