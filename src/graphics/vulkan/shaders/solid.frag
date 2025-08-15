#version 450
#include "push_constants.h"

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec2 uv;

void main()
{
    // positive: outside, negative: inside
    float signedDistance;

    if (pushConstants.shapeType == 1) { // Ellipse(normalized -> circle)
        signedDistance = length(uv) - 1.0;
    } else if (pushConstants.shapeType == 2) { // Rounded Rectangle
        vec2 cornerPos = abs(uv) - vec2(1.0, 1.0) + pushConstants.shapeParams.xy;
        signedDistance = min(cornerPos.x, cornerPos.y) < 0.0
        ? -1.0
        : length(cornerPos / pushConstants.shapeParams.xy) - 1.0;
    } else {
        signedDistance = -1.0;
    }

    if (signedDistance > 0.0) {
        discard;
    }

    outColor = pushConstants.color;
}