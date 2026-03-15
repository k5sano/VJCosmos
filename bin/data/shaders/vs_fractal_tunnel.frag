#version 120
uniform float u_time;
uniform vec2 u_resolution;
uniform float u_beat;
uniform float u_bass;
uniform float u_mid;
uniform float u_high;
uniform float u_volume;
uniform float u_colorShift;
uniform float u_zoom;
uniform float u_glow;
uniform float u_distortion;
uniform sampler2DRect u_fft;

mat2 rot(float a){
    float s = sin(a), c = cos(a);
    return mat2(c, -s, s, c);
}

vec3 hsv2rgb(float h, float s, float v){
    vec3 c = clamp(abs(mod(h*6.0+vec3(0,4,2),6.0)-3.0)-1.0,0.0,1.0);
    return v * mix(vec3(1.0), c, s);
}

float sdBox(vec3 p, vec3 b){
    vec3 d = abs(p) - b;
    return length(max(d, 0.0)) + min(max(d.x, max(d.y, d.z)), 0.0);
}

float map(vec3 p){
    float t = u_time * 0.4;
    p.z = mod(p.z + t * 3.0, 4.0) - 2.0;
    p.xy *= rot(t * 0.3 + u_beat * 0.5);
    p.xz *= rot(t * 0.2);
    p += vec3(sin(p.y*3.0+t), cos(p.z*2.0+t), sin(p.x*2.5+t)) * u_distortion * 0.3;

    float fft1 = texture2DRect(u_fft, vec2(0.05 * 256.0, 0.5)).r;
    float fft2 = texture2DRect(u_fft, vec2(0.2 * 256.0, 0.5)).r;

    float scale = 1.0;
    float d = sdBox(p, vec3(1.0 + u_bass * 0.3));
    for(int i = 0; i < 4; i++){
        vec3 a = mod(p * scale, 2.0) - 1.0;
        scale *= 3.0;
        vec3 r = abs(1.0 - 3.0 * abs(a));
        float da = max(r.x, r.y);
        float db = max(r.y, r.z);
        float dc = max(r.z, r.x);
        float c = (min(da, min(db, dc)) - 1.0) / scale;
        d = max(d, c);
    }

    vec3 sp = p;
    sp.xy *= rot(t * 1.5);
    float orb = length(sp - vec3(1.5 * sin(t), 1.5 * cos(t), 0.0)) - 0.1 - fft2 * 0.2;
    return min(d, orb);
}

vec3 calcNormal(vec3 p){
    vec2 e = vec2(0.001, 0.0);
    return normalize(vec3(
        map(p+e.xyy)-map(p-e.xyy),
        map(p+e.yxy)-map(p-e.yxy),
        map(p+e.yyx)-map(p-e.yyx)
    ));
}

void main(){
    vec2 uv = (gl_FragCoord.xy - u_resolution*0.5) / u_resolution.y;
    uv /= u_zoom;

    float camZ = u_time * 3.0;
    vec3 ro = vec3(sin(u_time*0.3)*0.5, cos(u_time*0.4)*0.5, camZ);
    vec3 rd = normalize(vec3(uv, 1.5 + u_beat * 0.4));
    rd.xy *= rot(sin(u_time*0.2)*0.3);
    rd.xz *= rot(u_time*0.15);

    float t = 0.0;
    float glowAcc = 0.0;
    bool hit = false;

    for(int i = 0; i < 80; i++){
        vec3 p = ro + rd * t;
        p.z = mod(p.z, 4.0) - 2.0;
        float d = map(p);
        glowAcc += 0.015 / (1.0 + d*d*40.0);
        if(d < 0.001){ hit = true; break; }
        if(t > 15.0) break;
        t += d * 0.8;
    }

    vec3 col = vec3(0.0);

    if(hit){
        vec3 p = ro + rd * t;
        p.z = mod(p.z, 4.0) - 2.0;
        vec3 n = calcNormal(p);
        vec3 lightDir = normalize(vec3(sin(u_time), 1.0, cos(u_time)));
        float diff = max(dot(n, lightDir), 0.0);
        float spec = pow(max(dot(reflect(-lightDir,n),-rd),0.0), 32.0);

        float hue = fract(t * 0.05 + u_time * 0.1 + u_bass * 0.3 + u_colorShift);
        vec3 baseColor = hsv2rgb(hue, 0.7 + u_mid * 0.3, 1.0);
        col = baseColor * (diff * 0.7 + 0.2) + vec3(spec * 0.5);

        float fresnel = pow(1.0 - max(dot(n, -rd), 0.0), 3.0);
        col += hsv2rgb(fract(hue + 0.3), 0.8, 1.0) * fresnel * 0.5;

        float fog = exp(-t * 0.15);
        col *= fog;
    }

    float hueShift = texture2DRect(u_fft, vec2(abs(uv.x)*0.5 * 256.0, 0.5)).r;
    col += glowAcc * hsv2rgb(fract(u_time*0.08 + hueShift + u_colorShift), 0.7, 1.0) * (0.3 + u_beat * 0.4) * u_glow;

    if(!hit){
        float bg = length(uv) * 0.5;
        col = hsv2rgb(fract(u_time*0.03 + bg*0.2 + u_colorShift), 0.4, 0.05 + u_beat*0.04);
    }

    col *= 1.0 - dot(uv*0.7, uv*0.7);
    col = col / (1.0 + col);
    col = pow(col, vec3(0.85));

    gl_FragColor = vec4(col, 1.0);
}
