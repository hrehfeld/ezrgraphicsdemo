varying vec3 normalVertexP;
varying vec2 texCoords;

void main(void)
{
	texCoords = gl_MultiTexCoord0.xy;
    normalVertexP = gl_NormalMatrix * normalize(gl_Normal);
    gl_Position = ftransform();
}
