#version 450
#include "push_constants.h"
#include "distance.glsl"

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec2 uv;

void main()
{
    // positive: outside, negative: inside
    float signedDistance = signedDistanceFromUv(uv, push.shapeType, push.shapeParams);

    if (signedDistance > 0.0) {
        discard;
    }

    outColor = push.color;
}