#version 450

#include "push_constants.h"
#include "distance.glsl"

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec2 uv;
layout(location = 1) in vec2 pixelPos;

layout(set = 0, binding = 0) uniform sampler1D gradientLut;

void main() {
    float signedDistance = signedDistanceFromUv(uv, push.shapeType, push.shapeParams);

    if (signedDistance > 0.0) {
        discard;
    }

    vec2 center = push.color.xy;
    vec2 offset = push.color.zw;
    vec2 radius = push.global.xy;

    float d = length((pixelPos - center) / radius);

    outColor = texture(gradientLut, d);
}
