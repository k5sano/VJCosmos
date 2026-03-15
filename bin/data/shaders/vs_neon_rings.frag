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

mat2 rot(float a){ return mat2(cos(a),-sin(a),sin(a),cos(a)); }

void main(){
    vec2 uv = (gl_FragCoord.xy - u_resolution*0.5) / u_resolution.y;
    uv /= u_zoom;

    vec3 col = vec3(0.0);

    for(int i = 0; i < 10; i++){
        float fi = float(i);
        float fftVal = texture2DRect(u_fft, vec2((fi*0.08+0.03) * 256.0, 0.5)).r;

        vec2 center = vec2(
            sin(u_time*0.4 + fi*0.8) * 0.12,
            cos(u_time*0.3 + fi*1.2) * 0.12
        ) * (1.0 + u_distortion);

        float radius = 0.08 + fi*0.07 + fftVal*0.12 + u_beat*0.03;

        float angle = atan(uv.y - center.y, uv.x - center.x);
        float wobble = sin(angle*3.0 + u_time*2.0 + fi)*0.01*(1.0+u_distortion*3.0);
        float dist = abs(length(uv - center) - radius - wobble);

        float neon = 0.003 / (dist + 0.003);
        float hue = fract(fi*0.1 + u_time*0.04 + u_colorShift);
        col += hsv2rgb(hue, 0.7, 1.0) * neon * 0.12 * u_glow;
    }

    float cd = length(uv);
    col += hsv2rgb(fract(u_time*0.08+u_colorShift), 0.4, 1.0) * 0.02/(cd+0.02) * u_beat * u_glow;

    col += vec3(0.9,0.95,1.0) * max(u_beat - 0.85, 0.0) * 1.5;

    col *= 1.0 - dot(uv*0.6, uv*0.6);
    col = col / (1.0+col);
    col = pow(col, vec3(0.85));

    gl_FragColor = vec4(col, 1.0);
}
