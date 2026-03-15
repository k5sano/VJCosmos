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

vec3 hsv2rgb(float h, float s, float v){
    vec3 c = clamp(abs(mod(h*6.0+vec3(0,4,2),6.0)-3.0)-1.0,0.0,1.0);
    return v * mix(vec3(1.0), c, s);
}

mat2 rot(float a){
    return mat2(cos(a),-sin(a),sin(a),cos(a));
}

float terrain(vec2 p){
    float fftVal = texture2DRect(u_fft, vec2(fract(p.x * 0.1 + u_time * 0.05) * 256.0, 0.5)).r;
    float fftVal2 = texture2DRect(u_fft, vec2(fract(p.x * 0.05 + 0.3) * 256.0, 0.5)).r;

    float h = 0.0;
    h += sin(p.x * 0.5 + u_time) * 0.3;
    h += sin(p.x * 1.3 - u_time * 0.7) * 0.15;
    h += sin(p.x * 3.7 + p.y * 0.5) * 0.08 * (1.0 + u_distortion * 3.0);
    h += fftVal * 1.5 * (0.5 + u_beat * 0.5);
    h += fftVal2 * 0.8;

    return h - p.y;
}

void main(){
    vec2 uv = (gl_FragCoord.xy - u_resolution * 0.5) / u_resolution.y;
    uv /= u_zoom;

    float camHeight = 3.0 + sin(u_time * 0.3) * 0.5 - u_beat * 0.5;
    vec3 ro = vec3(u_time * 2.0, camHeight, -3.0);
    vec3 rd = normalize(vec3(uv.x, uv.y - 0.2, 1.5));
    rd.xz *= rot(sin(u_time * 0.2) * 0.2);

    vec3 col = vec3(0.0);
    float t = 0.0;
    bool hit = false;
    vec3 hitPos;

    for(int i = 0; i < 100; i++){
        vec3 p = ro + rd * t;
        float h = terrain(p.xz);
        float d = p.y - h;

        if(d < 0.01){
            hit = true;
            hitPos = p;
            break;
        }
        t += d * 0.4 + 0.02;
        if(t > 30.0) break;
    }

    if(hit){
        vec2 e = vec2(0.05, 0.0);
        float hc = terrain(hitPos.xz);
        float hx = terrain(hitPos.xz + e.xy);
        float hz = terrain(hitPos.xz + e.yx);
        vec3 n = normalize(vec3(hc - hx, e.x, hc - hz));

        vec3 lightDir = normalize(vec3(sin(u_time*0.5), 1.5, cos(u_time*0.3)));
        float diff = max(dot(n, lightDir), 0.0);
        float spec = pow(max(dot(reflect(-lightDir, n), -rd), 0.0), 16.0);

        float fftColor = texture2DRect(u_fft, vec2(fract(hitPos.x * 0.05) * 256.0, 0.5)).r;
        float hue = fract(fftColor * 0.5 + hitPos.y * 0.1 + u_time * 0.05 + u_colorShift);
        vec3 baseCol = hsv2rgb(hue, 0.6 + u_mid * 0.4, 0.8 + u_high * 0.2);

        col = baseCol * (diff * 0.6 + 0.25);
        col += vec3(1.0, 0.9, 0.7) * spec * 0.5;

        vec2 grid = abs(fract(hitPos.xz * 0.5) - 0.5);
        float gridLine = smoothstep(0.02, 0.04, min(grid.x, grid.y));
        vec3 gridCol = hsv2rgb(fract(u_time * 0.1 + u_colorShift), 0.8, 1.0);
        col = mix(col + gridCol * 0.3 * u_glow, col, gridLine);

        float fog = exp(-t * 0.06);
        vec3 fogCol = hsv2rgb(fract(u_time * 0.03 + u_colorShift), 0.3, 0.1 + u_beat * 0.05);
        col = mix(fogCol, col, fog);
    } else {
        float sky = uv.y * 0.5 + 0.5;
        col = hsv2rgb(fract(u_time * 0.02 + sky * 0.3 + u_colorShift), 0.4, 0.08);

        float aurora = 0.0;
        for(int i = 0; i < 3; i++){
            float fi = float(i);
            float wave = sin(uv.x * (3.0 + fi) + u_time * (0.5 + fi * 0.2)) * 0.1;
            float band = smoothstep(0.02, 0.0, abs(uv.y - 0.2 - fi * 0.08 - wave));
            aurora += band;
        }
        float auroraFFT = texture2DRect(u_fft, vec2(0.1 * 256.0, 0.5)).r;
        col += hsv2rgb(fract(u_time * 0.1 + uv.x * 0.2 + u_colorShift), 0.7, 1.0) * aurora * (0.3 + auroraFFT * 0.5) * u_glow;
    }

    col *= 1.0 - dot(uv * 0.6, uv * 0.6);
    col = col / (1.0 + col);
    col = pow(col, vec3(0.9));

    gl_FragColor = vec4(col, 1.0);
}
