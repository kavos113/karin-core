#version 450

#include "push_constants.h"

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec2 uv;
layout(location = 1) in vec2 pixelPos;

layout(set = 0, binding = 0) uniform sampler1D gradientLut;

void main() {
    // positive: outside, negative: inside
    float signedDistance;

    if (push.shapeType == 1) { // Ellipse(normalized -> circle)
        signedDistance = length(uv) - 1.0;
    } else if (push.shapeType == 2) { // Rounded Rectangle
        vec2 cornerPos = abs(uv) - vec2(1.0, 1.0) + push.shapeParams.xy;
        signedDistance = min(cornerPos.x, cornerPos.y) < 0.0
        ? -1.0
        : length(cornerPos / push.shapeParams.xy) - 1.0;
    } else {
        signedDistance = -1.0;
    }

    if (signedDistance > 0.0) {
        discard;
    }

    vec2 aspect = vec2(push.aspect, 1.0);
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
