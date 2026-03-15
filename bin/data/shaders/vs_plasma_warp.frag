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

vec2 warp(vec2 p, float t){
    float fft1 = texture2DRect(u_fft, vec2(0.1 * 256.0, 0.5)).r;
    float fft2 = texture2DRect(u_fft, vec2(0.3 * 256.0, 0.5)).r;
    float dist = 0.3 + u_distortion * 0.5;
    p += vec2(sin(p.y * 3.0 + t + fft1 * 2.0), cos(p.x * 3.0 + t * 1.3)) * dist;
    p += vec2(sin(p.y * 5.0 - t * 0.7 + fft2), cos(p.x * 4.0 + t * 0.5)) * dist * 0.5;
    return p;
}

float plasma(vec2 p, float t){
    float v = 0.0;
    v += sin(p.x * 3.0 + t);
    v += sin(p.y * 4.0 - t * 1.3);
    v += sin((p.x + p.y) * 2.0 + t * 0.7);
    v += sin(length(p) * 5.0 - t * 2.0);
    return v * 0.25;
}

void main(){
    vec2 uv = (gl_FragCoord.xy - u_resolution * 0.5) / u_resolution.y;
    uv /= u_zoom;
    float t = u_time;

    vec2 p = uv * (2.0 + u_beat * 0.5);
    p *= rot(t * 0.1 + u_bass * 0.5);

    vec2 w1 = warp(p, t);
    vec2 w2 = warp(w1 * 1.5, t * 0.7 + 100.0);

    float p1 = plasma(w1, t);
    float p2 = plasma(w2, t * 1.3);
    float p3 = plasma(p * 2.0, t * 0.5);

    float fftLow = texture2DRect(u_fft, vec2(0.05 * 256.0, 0.5)).r;
    float fftMid = texture2DRect(u_fft, vec2(0.25 * 256.0, 0.5)).r;
    float fftHigh = texture2DRect(u_fft, vec2(0.6 * 256.0, 0.5)).r;

    float combined = p1 * (1.0 + fftLow) + p2 * (0.5 + fftMid) + p3 * fftHigh;

    vec3 col = vec3(0.0);

    float hue1 = fract(combined * 0.3 + t * 0.05 + u_colorShift);
    float hue2 = fract(combined * 0.2 + t * 0.08 + 0.33 + u_colorShift);
    float hue3 = fract(combined * 0.15 + t * 0.03 + 0.66 + u_colorShift);

    col += hsv2rgb(hue1, 0.7, 0.8) * (0.5 + p1 * 0.5);
    col += hsv2rgb(hue2, 0.6, 0.6) * (0.3 + p2 * 0.3);
    col += hsv2rgb(hue3, 0.8, 0.5) * abs(p3);

    float arc = 0.0;
    for(int i = 0; i < 4; i++){
        float fi = float(i);
        vec2 ap = uv * rot(t * 0.3 + fi * 1.57);
        float fftArc = texture2DRect(u_fft, vec2((fi * 0.15 + 0.1) * 256.0, 0.5)).r;
        float wave = sin(ap.x * 10.0 + t * 3.0 + fi * 2.0 + fftArc * 5.0) * (0.05 + fftArc * 0.1);
        float line = smoothstep(0.015, 0.0, abs(ap.y - wave));
        arc += line;
    }
    col += hsv2rgb(fract(t * 0.15 + u_colorShift), 0.3, 1.0) * arc * (0.4 + u_beat * 0.6) * u_glow;

    float radius = length(uv);
    float burst = 0.05 / (radius + 0.05);
    float burstPulse = 1.0 + u_beat * 2.0;
    float burstFFT = texture2DRect(u_fft, vec2(0.02 * 256.0, 0.5)).r;
    col += hsv2rgb(fract(t * 0.2 + radius + u_colorShift), 0.5, 1.0) * burst * burstPulse * (0.15 + burstFFT * 0.3) * u_glow;

    float scan = sin(gl_FragCoord.y * 1.5 + t * 10.0) * 0.03;
    col += scan;

    col += vec3(0.8, 0.9, 1.0) * max(u_beat - 0.85, 0.0) * 2.0;

    col *= 1.0 - dot(uv * 0.6, uv * 0.6);
    col = max(col, 0.0);
    col = col / (1.0 + col);
    col = pow(col, vec3(0.85));

    gl_FragColor = vec4(col, 1.0);
}
