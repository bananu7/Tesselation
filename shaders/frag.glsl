#version 400 core
out vec4 color;
in vec2 tePosition;
void main() {
    color = vec4((tePosition + 1.0) * 0.5, 0.0, 1.0);
}
