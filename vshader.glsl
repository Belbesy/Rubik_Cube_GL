#version 130
attribute vec4 vPosition;
attribute vec4 vColor;
out vec4 color_out;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void
main()
{
    gl_Position =  Projection*View*Model*vPosition;
    color_out = vColor;
}
