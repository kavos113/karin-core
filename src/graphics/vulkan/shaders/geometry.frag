#version 450

#define FRAGMENT_SHADER
#include "push_constants.h"

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec2 uv;
layout(location = 1) in vec2 pixelPos;

// image: image, gradient: gradientLut
layout(set = 1, binding = 0) uniform sampler2D tex;

#include "common.glsl"

void main() {
    float signedDistance = signedDistanceFromUv(uv, push.shapeType, push.shapeParams);

    if (signedDistance > 0.0) {
        discard;
    }

    if (push.patternType == 0) { // solid
        outColor = push.color;
    } else if (push.patternType == 1) { // linear gradient
        float t = linear_gradient_t();
        outColor = texture(tex, vec2(t, 0.5));
    } else if (push.patternType == 2) { // radial gradient
        float t = radial_gradient_t();
        if (t >= 0.0) {
            outColor = texture(tex, vec2(t, 0.5));
        } else {
            discard;
        }
    } else if (push.patternType == 3) { // image
        outColor = image_color();
    } else {
        discard;
    }
}
