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

vec2 hash2(vec2 p){
    p = vec2(dot(p,vec2(127.1,311.7)), dot(p,vec2(269.5,183.3)));
    return fract(sin(p)*43758.5453);
}

void main(){
    vec2 uv = (gl_FragCoord.xy - u_resolution*0.5) / u_resolution.y;
    uv /= u_zoom;
    uv *= 4.0 + u_distortion * 3.0;

    vec2 ip = floor(uv);
    vec2 fp = fract(uv);

    float minDist = 1.0;
    float minDist2 = 1.0;
    vec2 minPoint = vec2(0.0);

    for(int j = -1; j <= 1; j++){
        for(int i = -1; i <= 1; i++){
            vec2 neighbor = vec2(float(i), float(j));
            vec2 point = hash2(ip + neighbor);
            float fftVal = texture2DRect(u_fft, vec2(fract(point.x*0.5) * 256.0, 0.5)).r;
            point = 0.5 + 0.5*sin(u_time*0.5 + 6.2831*point + fftVal*3.0);
            vec2 diff = neighbor + point - fp;
            float d = length(diff);
            if(d < minDist){
                minDist2 = minDist;
                minDist = d;
                minPoint = point;
            } else if(d < minDist2){
                minDist2 = d;
            }
        }
    }

    float edge = minDist2 - minDist;
    float hue = fract(minPoint.x + minPoint.y + u_time*0.1 + u_colorShift);
    vec3 col = hsv2rgb(hue, 0.6, 0.7) * (1.0 - minDist*0.8);

    float edgeGlow = smoothstep(0.08, 0.0, edge);
    col += hsv2rgb(fract(hue+0.5+u_colorShift), 0.8, 1.0) * edgeGlow * u_glow * 0.6;

    col *= 1.0 + u_beat * 0.4;

    vec2 vuv = (gl_FragCoord.xy - u_resolution*0.5) / u_resolution.y;
    col *= 1.0 - dot(vuv*0.7, vuv*0.7);
    col = col / (1.0+col);
    col = pow(col, vec3(0.85));

    gl_FragColor = vec4(col, 1.0);
}
