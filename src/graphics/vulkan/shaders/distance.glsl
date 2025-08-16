// positive: outside, negative: inside
float signedDistanceFromUv(vec2 uv, uint shapeType, vec3 shapeParams) {
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

    return signedDistance;
}