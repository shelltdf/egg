#version 330 core
in vec4 egg_va_position;
in vec3 egg_va_normal;
in vec2 egg_va_uv0;
in vec4 egg_va_color0;
uniform mat4 egg_ModelViewMatrix;
uniform mat4 egg_ProjectionMatrix;
out vec4 v_color0;
void main()
{
    gl_Position = egg_ProjectionMatrix * egg_ModelViewMatrix * egg_va_position;
    //gl_Position = egg_va_position;
    
    
    v_color0 = egg_va_color0;
}

