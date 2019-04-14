#version 330 core
uniform sampler2D egg_texture0;
in vec4 v_color0;
in vec2 v_uv0;
layout(location = 0) out vec4 FragColor;
void main()
{
    vec4 color = texture2D(egg_texture0, v_uv0);
    FragColor = v_color0 * color;
}

