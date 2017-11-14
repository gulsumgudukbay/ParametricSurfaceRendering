#version 130

precision highp float;
varying lowp vec4 vPos;

void main()
{
    gl_FragColor = vec4(0,0,0,1.0);
    float dist = distance(vPos.xyz, vec3(0.0, 0.3, -1.5));
    float intensity = 500.0/((dist+3.0)*(dist+3.0)*(dist+3.0)*(dist+3.0));
    gl_FragColor.r = intensity;
    gl_FragColor.g = intensity - 0.3;
    gl_FragColor.b = intensity - 0.1;
}
