#version 330 core
in vec2 egg_va_position2;
in vec2 egg_va_uv0;
in vec4 egg_va_color0;
// uniform mat4 egg_ModelViewMatrix;
// uniform mat4 egg_ProjectionMatrix;
uniform vec4 egg_ViewPort;
out vec4 v_color0;
out vec2 v_uv0;
void main()
{
    gl_Position = vec4(
        egg_va_position2.x/(egg_ViewPort.y-egg_ViewPort.x)*2 - 1
        ,-egg_va_position2.y/(egg_ViewPort.w-egg_ViewPort.z)*2 + 1
        ,0,1);

    v_color0 = egg_va_color0;
    v_uv0 = egg_va_uv0;
}

