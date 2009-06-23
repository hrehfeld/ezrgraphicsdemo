//light direction in tangentspace
varying vec3 light;
//eye pos in tangentspace
varying vec3 eye;
varying vec2 texCoords;
//attribute vec3 vTangent;

void main ()
{
    //calculate tangent
	vec3 c1 = cross(gl_Normal, vec3(0.0, 0.0, 1.0)); 
	vec3 c2 = cross(gl_Normal, vec3(0.0, 1.0, 0.0)); 
	
	vec3 tangent; 
	if (dot(c1, c1) > dot(c2,c2))
	{
		tangent = c1;	
	}
	else
	{
		tangent = c2;	
	}
	tangent = normalize(tangent);	


	//phong with bumpmapping from here
	texCoords = gl_MultiTexCoord0.xy;
	
	vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
//	vec3 tangent = normalize(gl_NormalMatrix * vTangent);
	vec3 binormal = cross(normal, tangent);

	gl_Position = gl_ModelViewMatrix * gl_Vertex;

	vec3 transformedVertex = vec3(gl_Position);
	vec3 lightDir = gl_LightSource[0].position.xyz - transformedVertex;

	light.x = dot(lightDir, tangent);
	light.y = dot(lightDir, binormal);
	light.z = dot(lightDir, normal);

	vec3 iTransformedVertex = -transformedVertex;
	eye.x = dot(iTransformedVertex, tangent);
	eye.y = dot(iTransformedVertex, binormal);
	eye.z = dot(iTransformedVertex, normal);

	gl_Position = ftransform();
}
