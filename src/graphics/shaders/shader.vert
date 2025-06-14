# version 450

layout(location = 0) in vec2 pos;
layout(location = 0) out vec2 normalizedCoords;

layout(push_constant) uniform PushConstants {
    mat4 normalizeMatrix;
} push;

void main()
{
    gl_Position = vec4(pos, 0.0, 1.0);
    normalizedCoords = (push.normalizeMatrix * vec4(pos, 0.0, 1.0)).xy;
}