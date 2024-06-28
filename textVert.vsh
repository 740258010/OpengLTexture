#version 450 core
layout(location = 0) in vec3 aPos; // <vec2 pos, vec2 tex>
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoords;
uniform mat4 worldToView;

//uniform int flag = 1;

void main(void)
{
    gl_Position = worldToView * vec4(aPos,1.0f);
    TexCoords = vec2(aTexCoord.x, aTexCoord.y);
}
