#version 450

#include "push_constants.h"
#include "distance.glsl"

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec2 uv;
layout(location = 1) in vec2 pixelPos;

layout(set = 0, binding = 0) uniform sampler2D image;

void main() {
    float signedDistance = signedDistanceFromUv(uv, push.shapeType, push.shapeParams);

    if (signedDistance > 0.0) {
        discard;
    }

    float uvMode = push.global.x;
    if (uvMode == 0.0) {
        outColor = texture(image, uv);
    } else {
        vec2 offset = push.color.xy;
        vec2 scale = push.color.zw;

        vec2 windowUv = pixelPos * scale + offset;
        windowUv = (windowUv + 1.0) * 0.5;

        outColor = texture(image, windowUv);
    }
}
