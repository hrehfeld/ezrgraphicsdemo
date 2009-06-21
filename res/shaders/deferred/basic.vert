varying vec3 normal;
varying vec3 v;
varying vec3 lightvec;
void main(void)
{
    normal          = normalize(gl_NormalMatrix * gl_Normal);
    v               = vec3(gl_ModelViewMatrix * gl_Vertex);
    lightvec        = normalize(gl_LightSource[0].position.xyz - v);
    gl_Position     = ftransform();
}