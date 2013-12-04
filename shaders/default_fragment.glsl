const int NO_TEXTURE = 666;

varying vec3 N;
varying vec4 v;
varying vec4 v1;

varying vec2 texCoord;

uniform bool light[8];

uniform float time;

uniform int lighting_enabled, shadows_enabled, texId, receive_shadows;

uniform sampler2D texture;

uniform mat4 shadowMapMatrix[3];

uniform sampler2D shadowMapTexture0;
uniform sampler2D shadowMapTexture1;
uniform sampler2D shadowMapTexture2;

vec4 srgb(vec4 color) {
    return pow(color, vec4(1.0/2.2));
}

vec4 blinn_light(gl_LightSourceParameters light) {
    vec3 light_dir = normalize(light.position.xyz - v.xyz);
    vec3 viewer_dir = normalize(-v.xyz);

    vec3 h = normalize((light_dir + viewer_dir)/2.0);

    float d = distance(light.position, v);

    float intensity = 1.0;

    vec4 lamb = gl_LightModel.ambient*gl_FrontMaterial.ambient+light.ambient*gl_FrontMaterial.ambient;
    float attenuation = intensity/(0.0+
                                   0.0+
                                   1.0*pow(d,2));
    vec4 ldiff;
    if (texId != NO_TEXTURE) {
        vec4 texDiff = texture2D(texture, texCoord);
        ldiff = texDiff*light.diffuse*max(dot(N, light_dir), 0);
    } else {
        ldiff = gl_FrontMaterial.diffuse*light.diffuse*max(dot(N, light_dir), 0);
    }
    vec4 lspec = gl_FrontMaterial.specular*light.specular*pow(max(dot(N, h), 0), gl_FrontMaterial.shininess);

    return lamb+attenuation*(ldiff + lspec);
}

vec4 lights() {
    vec4 c = vec4(0,0,0,1);
    for (int i = 0; i < 8; i++) {
        if (light[i]) c += blinn_light(gl_LightSource[i]);
    }
    return c;
}

vec4 applyShadowMap(int i, sampler2D tex) {
    vec4 c = vec4(0);
    float offset = 0.0001;

    vec4 lamb = gl_LightModel.ambient*gl_FrontMaterial.ambient;
    c = lamb;
    vec4 sc = shadowMapMatrix[i]*v;
    sc /= sc.w;
    sc.xyz /= 2.0;
    sc.xyz += 0.5;
    if (sc.z > 1.0 || sc.x < 0.0 || sc.x > 1.0) {
        //c += lights();
    //} else if (sc.z < 0.0 || sc.y < 0.0 || sc.y > 1.0) {
        //c += lights();
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
        if (shadows_enabled == 1 && receive_shadows == 1) {
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
