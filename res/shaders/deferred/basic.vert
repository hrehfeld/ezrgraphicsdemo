varying vec3 normal;
varying vec2 texCoords;

void main(void)
{
	texCoords = gl_MultiTexCoord0.xy;
    normal          = gl_NormalMatrix * gl_Normal;
    gl_Position     = ftransform();
}