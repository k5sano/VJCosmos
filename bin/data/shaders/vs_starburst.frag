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

    vec3 col = vec3(0.0);

    for(int i = 0; i < 16; i++){
        float fi = float(i);
        float rayAngle = fi/16.0*6.2831 + u_time*0.3 + sin(u_time*0.5+fi)*u_distortion*0.5;
        float fftVal = texture2DRect(u_fft, vec2(fi/16.0 * 256.0, 0.5)).r;

        float angleDiff = abs(mod(angle - rayAngle + 3.14159, 6.2831) - 3.14159);
        float rayWidth = 0.025 + fftVal*0.04 + u_beat*0.015;
        float ray = smoothstep(rayWidth, 0.0, angleDiff);
        ray *= exp(-radius*(1.8 - fftVal*1.2));

        float hue = fract(fi/16.0 + u_time*0.04 + u_colorShift);
        col += hsv2rgb(hue, 0.7, 1.0) * ray * 0.4 * u_glow;
    }

    for(int i = 0; i < 5; i++){
        float fi = float(i);
        float ringR = fract(fi*0.2 + u_time*0.25) * 1.5;
        float ringDist = abs(radius - ringR);
        float ring = smoothstep(0.015, 0.0, ringDist);
        float alpha = 1.0 - ringR/1.5;
        float fftVal = texture2DRect(u_fft, vec2((fi*0.15+0.05) * 256.0, 0.5)).r;
        float hue = fract(ringR + u_time*0.08 + u_colorShift);
        col += hsv2rgb(hue, 0.6, 1.0) * ring * alpha * (0.4 + fftVal*0.4);
    }

    for(int i = 0; i < 8; i++){
        float fi = float(i);
        float a = fi/8.0*6.2831 + u_time*0.7 + fi;
        float r = 0.3 + sin(u_time*1.5+fi*2.0)*0.15;
        vec2 sparkPos = vec2(cos(a), sin(a)) * r;
        float sparkDist = length(uv - sparkPos);
        float fftVal = texture2DRect(u_fft, vec2((fi*0.1+0.1) * 256.0, 0.5)).r;
        float spark = 0.003 / (sparkDist + 0.003);
        float hue = fract(fi*0.125 + u_time*0.1 + u_colorShift);
        col += hsv2rgb(hue, 0.5, 1.0) * spark * (0.1 + fftVal*0.15) * u_glow;
    }

    float star = 0.035 / (radius + 0.035);
    float starFFT = texture2DRect(u_fft, vec2(0.02 * 256.0, 0.5)).r;
    col += hsv2rgb(fract(u_time*0.1+u_colorShift), 0.3, 1.0) * star * (0.5+starFFT+u_beat) * u_glow;

    col *= 1.0 + u_beat * 0.25;
    col *= 1.0 - dot(uv*0.5, uv*0.5);
    col = col / (1.0+col);
    col = pow(col, vec3(0.85));

    gl_FragColor = vec4(col, 1.0);
}
