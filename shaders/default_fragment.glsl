varying vec3 N;
varying vec4 v;
varying vec4 v1;

uniform bool light0;
uniform bool light1;
uniform bool light2;
uniform bool light3;
uniform bool light4;
uniform bool light5;
uniform bool light6;
uniform bool light7;

int shadows = 0;

uniform float time;

uniform int lighting_enabled;

uniform mat4 shadowMapMatrix[3];

uniform sampler2D shadowMapTexture0;
uniform sampler2D shadowMapTexture1;
uniform sampler2D shadowMapTexture2;

vec4 srgb(vec4 color) {
    return pow(color, vec4(1/2.2));
}

vec4 blinn_light(gl_LightSourceParameters light) {
    vec3 light_dir = normalize(light.position.xyz - v.xyz);
    vec3 viewer_dir = normalize(-v.xyz);

    vec3 h = normalize((light_dir + viewer_dir)/2.0);

    float d = distance(light.position, v);

    float intensity = 5.0;

    vec4 lamb = gl_LightModel.ambient*gl_FrontMaterial.ambient+light.ambient*gl_FrontMaterial.ambient;
    float attenuation = intensity/(0.0+
                                   0.0+
                                   1.0*pow(d,2));
    vec4 ldiff = gl_FrontMaterial.diffuse*light.diffuse*max(dot(N, light_dir), 0);
    vec4 lspec = gl_FrontMaterial.specular*light.specular*pow(max(dot(N, h), 0), gl_FrontMaterial.shininess);

    return lamb+attenuation*(ldiff + lspec);
}

vec4 lights() {
    vec4 c = vec4(0);
    if (light0) c += blinn_light(gl_LightSource[0]);
    if (light1) c += blinn_light(gl_LightSource[1]);
    if (light2) c += blinn_light(gl_LightSource[2]);
    if (light3) c += blinn_light(gl_LightSource[3]);
    if (light4) c += blinn_light(gl_LightSource[4]);
    if (light5) c += blinn_light(gl_LightSource[5]);
    if (light6) c += blinn_light(gl_LightSource[6]);
    if (light7) c += blinn_light(gl_LightSource[7]);
    return c;
}

vec4 applyShadowMap(int i, sampler2D tex) {
    vec4 c = vec4(0);
    float offset = 0.0001;

    vec4 lamb = gl_LightModel.ambient*gl_FrontMaterial.ambient;
    c = lamb;
    vec4 sc = shadowMapMatrix[i]*v;
    sc /= sc.w;
    sc.xyz /= 2;
    sc.xyz += 0.5;
    if (sc.z > 1 || sc.x < 0 || sc.x > 1 ) {
        //c += lights();
    } else if (sc.z < 0 || sc.y < 0 || sc.y > 1 ) {
        c += lights();
    } else {
        if (texture2D(tex, sc.xy).x + offset >= sc.z) {
            c += lights();
        }
    }
    return c;
}

void main (void) {
    if (lighting_enabled == 1) {
        gl_FragColor = vec4(0);
        vec4 lamb = gl_LightModel.ambient*gl_FrontMaterial.ambient;
        vec4 c = lamb;
        if (shadows == 1) {
            c += applyShadowMap(0, shadowMapTexture0);
            c += applyShadowMap(1, shadowMapTexture1);
            c += applyShadowMap(2, shadowMapTexture2);
        } else {
            c += lights();
        }
        gl_FragColor = srgb(c);
    } else {
        gl_FragColor = srgb(gl_FrontMaterial.diffuse);
    }
    return;
}
