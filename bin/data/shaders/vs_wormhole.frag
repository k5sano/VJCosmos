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

    float angle = atan(uv.y, uv.x);
    float radius = length(uv);

    float twist = sin(radius*8.0 - u_time*3.0)*(0.5+u_distortion*1.5);
    angle += twist;
    angle += u_time * 0.5;

    float fftVal = texture2DRect(u_fft, vec2(radius*0.4 * 256.0, 0.5)).r;
    float fftVal2 = texture2DRect(u_fft, vec2(fract(angle/6.2831) * 256.0, 0.5)).r;

    float arms = sin(angle*5.0 + radius*12.0 - u_time*4.0 + fftVal*4.0)*0.5+0.5;
    float rings = sin(radius*25.0 - u_time*6.0 + fftVal2*5.0)*0.5+0.5;

    float pattern = arms*0.6 + rings*0.4;

    float hue = fract(pattern*0.3 + angle/6.2831*0.5 + u_time*0.05 + u_colorShift);
    vec3 col = hsv2rgb(hue, 0.7, 0.7) * pattern;

    float vortex = 0.04 / (radius + 0.04);
    col += hsv2rgb(fract(u_time*0.12+u_colorShift), 0.5, 1.0) * vortex * (1.0+u_beat*1.5) * u_glow;

    float spiralLine = smoothstep(0.02, 0.0, abs(fract(angle/6.2831*6.0 + radius*2.0 - u_time*0.5) - 0.5) - 0.45);
    col += hsv2rgb(fract(hue+0.5), 0.8, 1.0) * spiralLine * fftVal * 0.4 * u_glow;

    col *= 1.0 + u_beat * 0.3;
    col *= 1.0 - dot(uv*0.5, uv*0.5);
    col = col / (1.0+col);
    col = pow(col, vec3(0.85));

    gl_FragColor = vec4(col, 1.0);
}
