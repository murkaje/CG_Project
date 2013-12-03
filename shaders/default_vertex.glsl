const int NO_TEXTURE = 666;

varying vec3 N;
varying vec4 v;
varying vec4 v1;

varying vec2 texCoord;

uniform float time;

uniform int texId;

uniform bool light[8];

void main(void) {
    v = gl_ModelViewMatrix * gl_Vertex;
    N = normalize(gl_NormalMatrix * gl_Normal);
    v1 = gl_Vertex;

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    if (texId != NO_TEXTURE) {
        texCoord = (gl_Vertex.xy + 1.0) / 2.0;
    }
}
