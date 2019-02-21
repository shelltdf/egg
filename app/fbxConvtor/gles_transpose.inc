
//因为gles2 没有transpose函数支持

"float transpose(float m) {\n"
"  return m;\n"
"}\n"

"mat2 transpose(mat2 m) {\n"
"  return mat2(m[0][0], m[1][0],\n"
"              m[0][1], m[1][1]);\n"
"}\n"

"mat3 transpose(mat3 m) {\n"
"  return mat3(m[0][0], m[1][0], m[2][0],\n"
"              m[0][1], m[1][1], m[2][1],\n"
"              m[0][2], m[1][2], m[2][2]);\n"
"}\n"

"mat4 transpose(mat4 m) {\n"
"  return mat4(m[0][0], m[1][0], m[2][0], m[3][0],\n"
"              m[0][1], m[1][1], m[2][1], m[3][1],\n"
"              m[0][2], m[1][2], m[2][2], m[3][2],\n"
"              m[0][3], m[1][3], m[2][3], m[3][3]);\n"
"}\n"
"\n"