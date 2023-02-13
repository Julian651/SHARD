#version 330
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexColor;
out vec3 fragColor;
uniform mat4 m_transform;
void main()
{
    gl_Position = m_transform * vec4(vertexPos.x, vertexPos.y, vertexPos.z, 1.0);
    fragColor = vertexColor;
}