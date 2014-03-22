#version 400 core
layout(vertices = 4) out;

in vec2 vPosition[];
out vec2 tcPosition[];

//uniform float TessLevelInner;
//uniform float TessLevelOuter;
const float TessLevelInner = 16.0;
const float TessLevelOuter = 16.0;

#define ID gl_InvocationID

void main()
{
    tcPosition[ID] = vPosition[ID];
    if (ID == 0) {
        gl_TessLevelInner[0] = TessLevelInner;
        gl_TessLevelInner[1] = TessLevelInner;

        gl_TessLevelOuter[0] = TessLevelOuter;
        gl_TessLevelOuter[1] = TessLevelOuter;
        gl_TessLevelOuter[2] = TessLevelOuter;
        gl_TessLevelOuter[3] = TessLevelOuter;
    }
}
