#version 450

#include "push_constants.h"

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec2 uv;
layout(location = 1) in vec2 pixelPos;

// image: image, gradient: gradientLut
layout(set = 0, binding = 0) uniform sampler2D tex;

// positive: outside, negative: inside
float signedDistanceFromUv(vec2 uv, uint shapeType, vec2 shapeParams) {
    float signedDistance;

    if (shapeType == 1) { // Ellipse(normalized -> circle)
        signedDistance = length(uv) - 1.0;
    } else if (shapeType == 2) { // Rounded Rectangle
        vec2 cornerPos = abs(uv) - vec2(1.0, 1.0) + shapeParams;
        signedDistance = min(cornerPos.x, cornerPos.y) < 0.0
        ? -1.0
        : length(cornerPos / shapeParams) - 1.0;
    } else {
        signedDistance = -1.0;
    }

    return signedDistance;
}

float linear_gradient_t() {
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

    return t;
}

float radial_gradient_t() {
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

    return t;
}

vec4 image_color() {
    vec4 color;

    float uvMode = push.global.x;
    if (uvMode == 0.0) {
        color = texture(tex, uv);
    } else {
        vec2 offset = push.color.xy;
        vec2 scale = push.color.zw;

        vec2 windowUv = pixelPos / scale + offset;
        windowUv = (windowUv + 1.0) * 0.5;

        color = texture(tex, windowUv);
    }

    return color;
}

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
