varying vec3 N;
varying vec3 v;

uniform bool light0;
uniform bool light1;
uniform bool light2;
uniform bool light3;
uniform bool light4;
uniform bool light5;
uniform bool light6;
uniform bool light7;

uniform float time;

uniform int lighting_enabled;

vec4 blinn_light(gl_LightSourceParameters light) {
    vec3 light_dir = normalize(light.position.xyz - v.xyz);
    vec3 viewer_dir = normalize(-v);

    vec3 h = normalize((light_dir + viewer_dir)/2.0);

    float d = distance(light.position.xyz, v);

    float intensity = 5.0;

    vec4 lamb = gl_LightModel.ambient*gl_FrontMaterial.ambient+light.ambient*gl_FrontMaterial.ambient;
    float attenuation = intensity/(0.0+
                                   0.0+
                                   1.0*pow(d,2));
    vec4 ldiff = gl_FrontMaterial.diffuse*light.diffuse*max(dot(N, light_dir), 0);
    vec4 lspec = gl_FrontMaterial.specular*light.specular*pow(max(dot(N, h), 0), gl_FrontMaterial.shininess);

    return lamb+attenuation*(ldiff + lspec);
}

void main (void) {
    if (lighting_enabled == 1) {
        vec4 lamb = gl_LightModel.ambient*gl_FrontMaterial.ambient;
        gl_FragColor = lamb;
        if (light0) gl_FragColor += blinn_light(gl_LightSource[0]);
        if (light1) gl_FragColor += blinn_light(gl_LightSource[1]);
        if (light2) gl_FragColor += blinn_light(gl_LightSource[2]);
        if (light3) gl_FragColor += blinn_light(gl_LightSource[3]);
        if (light4) gl_FragColor += blinn_light(gl_LightSource[4]);
        if (light5) gl_FragColor += blinn_light(gl_LightSource[5]);
        if (light6) gl_FragColor += blinn_light(gl_LightSource[6]);
        if (light7) gl_FragColor += blinn_light(gl_LightSource[7]);
    } else {
        gl_FragColor = gl_FrontMaterial.diffuse;
    }
    return;
}
