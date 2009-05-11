uniform float time;

void main()
{  

	vec4 vertex = vec4(gl_Vertex);
	vertex.z = sin(4.0 * vertex.y + time) * 0.45;
	gl_Position = gl_ModelViewProjectionMatrix * vertex;
}
