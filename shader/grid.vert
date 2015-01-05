#version 400
layout (location = 0) in vec3 vp;
layout (location = 1) in vec3 vn;
out vec3 color;
uniform mat4 uViewMatrix, uProjectionMatrix;
uniform float uTime;
void main() {
    color = vec3(1.0, 1.0, 1.0);
    vec3 pt = vp * 2.0;
    gl_PointSize = abs(sin(pt.x * pt.y * uTime * 5.0)  * 5.0);
    gl_Position = vec4(pt, 1.0);
}