#version 130

precision highp float;
attribute  vec4 position;
uniform mat4 projectionMatrix;

varying lowp vec4 vPos;

void main()
{
    gl_Position = projectionMatrix * position;
    vPos = gl_Position;
}
