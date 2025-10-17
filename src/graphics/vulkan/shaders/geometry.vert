#version 450

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 inUv;
layout(location = 0) out vec2 uv;
layout(location = 1) out vec2 pixelPos;

layout(set = 2, binding = 0) uniform Matrices {
    mat4 projection;
} matrices;

void main() {
    gl_Position = vec4(pos, 0.0, 1.0);
    uv = inUv;
    pixelPos = pos;
}
