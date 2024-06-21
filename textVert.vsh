#version 450 core
layout(location = 0) in vec3 aPos;

out vec4 fragColor;
//uniform mat4 worldToView;
out vec2 TexCoord;

void main(void)
{
    gl_Position =  vec4(aPos.x,aPos.y,aPos.z,1.0f);
//    gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0f);
//    fragColor  =  vec4(aColor,1.0f);
    TexCoord = vec2(TexCoord.x, TexCoord.y);;
}
