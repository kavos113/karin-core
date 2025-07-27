# version 450

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec2 uv;

layout(push_constant) uniform PushConstants {
    vec4 color;
    vec3 shapeParams;
    uint shapeType;
} push;

void main()
{
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

    outColor = push.color;
}