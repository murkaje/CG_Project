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
    //need to add UV map VBO to mesh and make the shader use it
        if(gl_Normal.x > 0.999 || gl_Normal.x < -0.999) {
            texCoord = vec2(gl_Vertex.z, gl_Vertex.y)+0.5;
        } else if(gl_Normal.y > 0.999 || gl_Normal.y < -0.999) {
            texCoord = vec2(gl_Vertex.x, gl_Vertex.z)+0.5;
        } else {
            texCoord = vec2(gl_Vertex.x, gl_Vertex.y)+0.5;
        }
        //texCoord = vec2(gl_Vertex.x,  min(gl_Vertex.y,gl_Vertex.z) )+0.5;
    }
}
