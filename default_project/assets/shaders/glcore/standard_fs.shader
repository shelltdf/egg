#version 330 core
//out vec4 color;
in vec4 v_color0;
layout(location = 0) out vec4 FragColor;
void main()
{
    //gl_FragColor = vec4(0.0, 0.5, 0.5, 1.0);
    FragColor = v_color0;
}

