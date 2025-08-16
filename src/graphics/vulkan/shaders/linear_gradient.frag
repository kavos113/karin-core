#version 450

#include "push_constants.h"
#include "distance.glsl"

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec2 uv;
layout(location = 1) in vec2 pixelPos;

layout(set = 0, binding = 0) uniform sampler1D gradientLut;

void main() {
    // positive: outside, negative: inside
    float signedDistance = signedDistanceFromUv(uv, push.shapeType, push.shapeParams);

    if (signedDistance > 0.0) {
        discard;
    }

    vec2 aspect = vec2(push.global.x, 1.0);
    vec2 startPoint = push.color.xy * aspect;
    vec2 endPoint = push.color.zw * aspect;
    vec2 direction = endPoint - startPoint;
    vec2 pixel = pixelPos * aspect - startPoint;

    float lengthSq = dot(direction, direction);

    float t;
    if (lengthSq < 0.0001) {
        t = 0.0;// startPoint and endPoint are the same
    } else {
        t = dot(pixel, direction) / lengthSq;
    }

    outColor = texture(gradientLut, t);
}
