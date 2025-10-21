#version 450

#define VERTEX_SHADER
#include "push_constants.h"

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 inUv;
layout(location = 0) out vec2 uv;
layout(location = 1) out vec2 pixelPos;

layout(set = 0, binding = 0) uniform Matrices {
    mat4 projection;
} matrices;

void main() {
    gl_Position = matrices.projection * vec4(pos, 0.0, 1.0);
    uv = inUv;
    pixelPos = pos;
}
