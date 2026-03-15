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

float shape(vec2 p, float r, int n){
    float a = atan(p.y, p.x);
    float sides = float(n);
    float angle = 3.14159 * 2.0 / sides;
    float d = cos(floor(0.5 + a / angle) * angle - a) * length(p) - r;
    return d;
}

void main(){
    vec2 uv = (gl_FragCoord.xy - u_resolution * 0.5) / u_resolution.y;
    uv /= u_zoom;
    float t = u_time;

    float segments = 6.0 + floor(u_bass * 4.0) * 2.0 + floor(u_distortion * 6.0) * 2.0;
    float angle = atan(uv.y, uv.x);
    float radius = length(uv);
    angle = mod(angle, 3.14159 * 2.0 / segments);
    angle = abs(angle - 3.14159 / segments);
    vec2 kUv = vec2(cos(angle), sin(angle)) * radius;

    kUv *= 1.0 + u_beat * 0.3;
    kUv *= rot(t * 0.3);

    vec3 col = vec3(0.0);

    for(int i = 0; i < 5; i++){
        float fi = float(i);
        float r = 0.15 + fi * 0.15;
        float fftVal = texture2DRect(u_fft, vec2((fi * 0.1 + 0.05) * 256.0, 0.5)).r;
        float ring = smoothstep(0.02, 0.0, abs(length(kUv) - r - fftVal * 0.1));
        float hue = fract(fi * 0.2 + t * 0.1 + fftVal + u_colorShift);
        col += hsv2rgb(hue, 0.7, 1.0) * ring * (0.5 + u_beat * 0.5) * u_glow;
    }

    for(int i = 0; i < 3; i++){
        float fi = float(i);
        vec2 sp = kUv;
        sp *= rot(t * (0.5 + fi * 0.3));
        sp += vec2(sin(t + fi), cos(t * 0.7 + fi)) * 0.2;
        float fftVal = texture2DRect(u_fft, vec2((0.3 + fi * 0.15) * 256.0, 0.5)).r;
        float s = shape(sp, 0.05 + fftVal * 0.08, 3 + i);
        float glowVal = 0.005 / (abs(s) + 0.005);
        float hue = fract(t * 0.15 + fi * 0.33 + radius * 0.5 + u_colorShift);
        col += hsv2rgb(hue, 0.8, 1.0) * glowVal * 0.15 * u_glow;
    }

    float spiral = sin(radius * 20.0 - t * 4.0 + angle * segments) * 0.5 + 0.5;
    float fftSpiral = texture2DRect(u_fft, vec2(radius * 0.5 * 256.0, 0.5)).r;
    spiral *= fftSpiral;
    float spiralHue = fract(radius * 0.5 + t * 0.08 + u_colorShift);
    col += hsv2rgb(spiralHue, 0.6, 0.8) * spiral * 0.2;

    float centerGlow = 0.03 / (radius + 0.03);
    float centerFFT = texture2DRect(u_fft, vec2(0.02 * 256.0, 0.5)).r;
    col += hsv2rgb(fract(t * 0.12 + u_colorShift), 0.5, 1.0) * centerGlow * (0.3 + centerFFT * 0.5 + u_beat * 0.3) * u_glow;

    if(u_beat > 0.8){
        col += vec3(1.0) * (u_beat - 0.8) * 0.5;
    }

    col *= 1.0 - dot(uv * 0.5, uv * 0.5);
    col = col / (1.0 + col);
    col = pow(col, vec3(0.85));

    gl_FragColor = vec4(col, 1.0);
}
