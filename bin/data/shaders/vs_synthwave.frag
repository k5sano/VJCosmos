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

    vec3 col = vec3(0.0);
    float t = u_time;

    if(uv.y < 0.1){
        float py = uv.y - 0.1;
        float depth = 1.0 / (-py + 0.001);
        float px = uv.x * depth + t * 2.0;

        float gx = abs(sin(px * 3.14159 * 0.5));
        float gz = abs(sin((depth*0.3 - t*0.8) * 3.14159));
        float grid = max(
            smoothstep(0.95, 1.0, gx),
            smoothstep(0.95, 1.0, gz)
        );

        float fftVal = texture2DRect(u_fft, vec2(fract(abs(px)*0.05) * 256.0, 0.5)).r;
        float hue = fract(depth*0.03 + t*0.04 + u_colorShift);
        col = hsv2rgb(hue, 0.8, 1.0) * grid * (0.4 + fftVal*0.6 + u_beat*0.4) * u_glow;

        float mountainX = px * 0.02;
        float mountainH = sin(mountainX*3.0)*0.03 + sin(mountainX*7.0)*0.015 + sin(mountainX*13.0)*0.008;
        mountainH += texture2DRect(u_fft, vec2(fract(mountainX*0.1) * 256.0, 0.5)).r * 0.02;
        float mountain = smoothstep(mountainH, mountainH + 0.005, py + 0.1);
        vec3 mountainCol = hsv2rgb(fract(0.7+u_colorShift), 0.6, 0.3);
        col = mix(mountainCol * (0.5+u_beat*0.3), col, mountain);

        float fog = exp(-depth*0.04);
        col *= fog;
    }

    vec2 sunPos = vec2(0.0, 0.18);
    float sunDist = length(uv - sunPos);
    float sun = smoothstep(0.14, 0.135, sunDist);
    float sunLines = step(0.5, fract(uv.y*25.0));
    sun *= mix(1.0, 0.2, sunLines * step(uv.y, sunPos.y));
    col += hsv2rgb(fract(0.05+u_colorShift), 0.9, 1.0) * sun;

    col += hsv2rgb(fract(0.08+u_colorShift), 0.7, 1.0) * 0.04/(sunDist+0.04) * u_glow * 0.5;

    if(uv.y >= 0.1 && sun < 0.01){
        float sky = max(uv.y - 0.1, 0.0);
        col = max(col, hsv2rgb(fract(0.75+u_colorShift), 0.5, 0.03+sky*0.04));

        vec2 starUv = uv * 20.0;
        vec2 starId = floor(starUv);
        float starHash = fract(sin(dot(starId, vec2(12.9898, 78.233)))*43758.5453);
        float starBright = step(0.97, starHash) * fract(sin(starHash*100.0+t)*10.0);
        col += vec3(starBright * 0.3);
    }

    float horizon = smoothstep(0.03, 0.0, abs(uv.y - 0.1));
    col += hsv2rgb(fract(0.05+u_colorShift), 0.6, 1.0) * horizon * 0.4 * u_glow;

    col *= 1.0 - dot(uv*0.5, uv*0.5);
    col = col / (1.0+col);
    col = pow(col, vec3(0.85));

    gl_FragColor = vec4(col, 1.0);
}
