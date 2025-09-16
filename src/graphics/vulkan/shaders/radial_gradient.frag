#version 450

#include "push_constants.h"
#include "distance.glsl"

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec2 uv;
layout(location = 1) in vec2 pixelPos;

layout(set = 0, binding = 0) uniform sampler2D gradientLut;

void main() {
    float signedDistance = signedDistanceFromUv(uv, push.shapeType, push.shapeParams);

    if (signedDistance > 0.0) {
        discard;
    }

    vec2 center = push.color.xy;
    vec2 offset = push.color.zw;
    vec2 radius = push.global.xy;

    vec2 d = (pixelPos - center - offset) / radius;
    vec2 o = -offset / radius;

    float a = dot(o, o) - 1.0;
    float b = -2.0 * dot(o, d);
    float c = dot(d, d);

    float t = -1.0;

    if (abs(a) < 0.0001) {
        if (abs(b) < 0.0001) {
            t = 0.0;
        } else {
            t = -c / b;
        }
    } else {
        float dis = b * b - 4.0 * a * c;
        if (dis < 0.0) {
            discard;// No intersection
        }

        t = (-b - sqrt(dis)) / (2.0 * a);
    }

    if (t >= 0.0) {
        outColor = texture(gradientLut, vec2(t, 0.5));
    } else {
        discard;
    }
}
