varying vec3 N;
varying vec3 v;
varying float d;

uniform float time;

uniform int lighting_enabled;

void main (void) {
    if (lighting_enabled == 1) {
        vec3 light0 = normalize(gl_LightSource[0].position.xyz - v.xyz);
        vec3 viewer = normalize(-v);
        vec3 reflection1 = normalize(-reflect(light0,N));
        float light1_attenuation = 1.0/(gl_LightSource[0].constantAttenuation+
                                      gl_LightSource[0].linearAttenuation*d+
                                      gl_LightSource[0].quadraticAttenuation*pow(d,2));
        vec4 Iamb = gl_LightModel.ambient*gl_FrontMaterial.ambient+gl_LightSource[0].ambient*gl_FrontMaterial.ambient;
        vec4 Idiff = gl_FrontMaterial.diffuse*gl_LightSource[0].diffuse*max(dot(N, light0), 0);
        vec4 Ispec = gl_FrontMaterial.specular*gl_LightSource[0].specular*pow(max(dot(viewer, reflection1), 0), gl_FrontMaterial.shininess);

        gl_FragColor = Iamb + light1_attenuation*(Idiff + Ispec);
    } else {
        gl_FragColor = gl_FrontMaterial.diffuse;
    }
    return;
}
