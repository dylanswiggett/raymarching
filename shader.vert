#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;

out vec2 screenp;

void main() {
     screenp = vertexPosition_modelspace.xy;
     gl_Position = vec4(vertexPosition_modelspace, 1.0f);
}

