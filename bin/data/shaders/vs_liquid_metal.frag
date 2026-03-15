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

void main(){
    vec2 uv = (gl_FragCoord.xy - u_resolution*0.5) / u_resolution.y;
    uv /= u_zoom;

    float v = 0.0;
    for(int i = 0; i < 8; i++){
        float fi = float(i);
        float fftVal = texture2DRect(u_fft, vec2((fi*0.1+0.03) * 256.0, 0.5)).r;
        vec2 center = vec2(
            sin(u_time*(0.3+fi*0.08) + fi*2.0),
            cos(u_time*(0.2+fi*0.12) + fi*1.5)
        ) * (0.35 + fftVal*0.25 + u_distortion*0.2);
        float d = length(uv - center);
        v += (0.08 + fftVal*0.04) / d;
    }

    float edge = smoothstep(2.0, 2.8, v);
    float hue = fract(v*0.04 + u_time*0.08 + u_colorShift);
    vec3 col = hsv2rgb(hue, 0.45, 0.75) * edge;

    float highlight = pow(fract(v*0.15 + u_time*0.25), 4.0);
    col += vec3(1.0, 0.95, 0.9) * highlight * 0.6;

    float edgeGlow = smoothstep(2.8, 2.0, v) * smoothstep(1.5, 2.0, v);
    col += hsv2rgb(fract(u_time*0.06 + u_colorShift + 0.3), 0.8, 1.0) * edgeGlow * u_glow * 0.8;

    col *= 1.0 + u_beat * 0.4;

    col *= 1.0 - dot(uv*0.7, uv*0.7);
    col = col / (1.0+col);
    col = pow(col, vec3(0.85));

    gl_FragColor = vec4(col, 1.0);
}
