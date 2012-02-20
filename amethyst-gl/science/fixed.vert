//uniform Transformation {
//    mat4 projection_matrix;
//    mat4 modelview_matrix;
//};
 
//in vec3 vertex;
#version 110 

void main() {
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = ftransform();
}
