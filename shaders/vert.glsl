#version 400 core
layout(location = 0) in vec2 position;
out vec2 vPosition;

void main() {
    vPosition = position;
    gl_Position = vec4(position, 0.0, 1.0);
}
