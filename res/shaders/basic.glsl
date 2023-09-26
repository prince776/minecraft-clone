#shader vertex
#version 330 core

layout(location = 0)in vec4 position;
layout(location = 1)in vec2 texCoord;
layout(location = 3)in vec4 color;

out vec2 v_TexCoord;
out vec4 v_Color;
uniform mat4 u_MVP;

void main() {
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
    v_Color = color;
}

#shader fragment
#version 330 core

layout(location = 0)out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
uniform sampler2D u_TextureSlot;

void main() {
    vec4 texColor = texture(u_TextureSlot, v_TexCoord);
	color = v_Color * texColor;
}
