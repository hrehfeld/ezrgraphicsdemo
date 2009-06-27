//light direction in tangentspace
varying vec3 light;
varying float lightDistSqr;
//eye pos in tangentspace
varying vec3 eye;
varying vec2 texCoords;
//attribute vec3 vTangent;

//debug only
varying vec3 normal;
varying vec3 tangent;

void main ()
{
	normal = normalize(gl_NormalMatrix * gl_Normal);

    //calculate tangent
	tangent = cross(normal, vec3(1.0, 0.0, 0.0));
	//check for linear dependency
	float epsilon = 0.03;
	if (dot(tangent, tangent) < epsilon)
	{
		tangent = cross(normal, vec3(0.0, 1.0, 0.0));
	}
	tangent = normalize(tangent);	

	vec3 binormal = cross(tangent, normal);


	texCoords = gl_MultiTexCoord0.xy;

	vec3 vertexView = vec3(gl_ModelViewMatrix * gl_Vertex);
	
	//light pos is automatically in view coords
	vec3 lightDir = gl_LightSource[0].position.xyz - vertexView;
	lightDistSqr = dot(lightDir, lightDir);
	//we don't need to do this, as it must be done in the frag again
	//lightDir = normalize(lightDir);

	//rotate to tangent space
	light.x = dot(lightDir, tangent);
	light.y = dot(lightDir, binormal);
	light.z = dot(lightDir, normal);

	//vertexView = vertex - view <=> -vertexView = (-vertex) - view = view - vertex = eyedirection
	vec3 invVertexView = -vertexView;
	eye.x = dot(invVertexView, tangent);
	eye.y = dot(invVertexView, binormal);
	eye.z = dot(invVertexView, normal);

	gl_Position = ftransform();
}
