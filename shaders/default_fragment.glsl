varying vec3 N;
varying vec3 v;
varying float d;

uniform float time;

uniform int lighting_enabled;

void main (void) {
    if (lighting_enabled == 1) {
        vec3 light_dir = normalize(gl_LightSource[0].position.xyz - v.xyz);
        vec3 viewer_dir = normalize(-v);

        vec3 h = normalize((light_dir + viewer_dir)/2.0);

        float attenuation = 1.0/(gl_LightSource[0].constantAttenuation+
                                gl_LightSource[0].linearAttenuation*d+
                                gl_LightSource[0].quadraticAttenuation*pow(d,2));

        vec4 Iamb = gl_LightModel.ambient*gl_FrontMaterial.ambient+gl_LightSource[0].ambient*gl_FrontMaterial.ambient;
        vec4 Idiff = gl_FrontMaterial.diffuse*gl_LightSource[0].diffuse*max(dot(N, light_dir), 0);
        vec4 Ispec = gl_FrontMaterial.specular*gl_LightSource[0].specular*pow(max(dot(N, h), 0), gl_FrontMaterial.shininess);

        gl_FragColor = Iamb + attenuation*(Idiff + Ispec);
    } else {
        gl_FragColor = gl_FrontMaterial.diffuse;
    }
    return;
}
