varying vec3 normalVertexP;
varying vec2 texCoords;
varying float zView;

void main(void)
{
	zView = (gl_ModelViewMatrix * gl_Vertex).z;
	texCoords = gl_MultiTexCoord0.xy;
    normalVertexP = gl_NormalMatrix * normalize(gl_Normal);
    gl_Position = ftransform();
}
