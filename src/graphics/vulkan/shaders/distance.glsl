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