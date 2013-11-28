const int NO_TEXTURE = 666;

varying vec3 N;
varying vec4 v;
varying vec4 v1;

uniform float time;

uniform int texId;

uniform bool light[8];

uniform int lighting_enabled;

void main(void) {
    v = gl_ModelViewMatrix * gl_Vertex;
    N = normalize(gl_NormalMatrix * gl_Normal);
    v1 = gl_Vertex;

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    if (texId != NO_TEXTURE) {
        gl_TexCoord[texId].xy = (gl_Vertex.xy + 1) / 2.0;
    }
}
