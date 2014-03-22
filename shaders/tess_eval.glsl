#version 400 core
layout(quads) in;
in vec2 tcPosition[];

out vec2 tePosition;

#define ID gl_PrimitiveID

void main() {
    //vec2 p0 = gl_TessCoord.x * tcPosition[0];
    //vec2 p1 = gl_TessCoord.y * tcPosition[1];
    //vec3 p2 = gl_TessCoord.z * tcPosition[2];
    //vec2 tePosition = p0 + p1;


    float u = gl_TessCoord.x, v = gl_TessCoord.y;

    vec2 a = mix(tcPosition[0], tcPosition[1], u);
    vec2 b = mix(tcPosition[2], tcPosition[3], u);

    //vec2 a = mix(gl_in[0].gl_Position.xy, gl_in[1].gl_Position.xy, u);
    //vec2 b = mix(gl_in[2].gl_Position.xy, gl_in[3].gl_Position.xy, u);

    tePosition = mix(a, b, v);
    tePosition.y += sin(u * 3.1416 * 2) * 0.1;
    //vec2 tePosition = tcPosition[ID];

    gl_Position = vec4(tePosition, 0.0, 1.0);
}
