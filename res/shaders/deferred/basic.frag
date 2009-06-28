varying vec3 normal;
varying vec2 texCoords;

uniform sampler2D colorMap;
uniform sampler2D normalMap;
 
void main(void)
{
    //calculate tangent
	vec3 tangent = cross(normal, vec3(1.0, 0.0, 0.0));
	//check for linear dependency
	float epsilon = 0.003;
	if (dot(tangent, tangent) < epsilon)
	{
		tangent = cross(normal, vec3(0.0, 1.0, 0.0));
	}
	tangent = normalize(tangent);	

	vec3 binormal = cross(tangent, normal);

	vec3 normalM = normalize((texture2D(normalMap, texCoords).xyz - 0.5) * 2);
	vec3 worldNormal;
	vec3 col0 = normalize(vec3(binormal.x, tangent.x, normal.x));
	vec3 col1 = normalize(vec3(binormal.y, tangent.y, normal.y));
	vec3 col2 = normalize(vec3(binormal.z, tangent.z, normal.z));

	/* worldNormal.y = dot(normalM, tangent); */
	/* worldNormal.x = dot(normalM, binormal); */
	/* worldNormal.z = dot(normalM, normal); */
	worldNormal.y = dot(normalM, col0);
	worldNormal.x = dot(normalM, col1);
	worldNormal.z = dot(normalM, col2);
	worldNormal = normalize(worldNormal);
//	worldNormal = -worldNormal;



	//color info
    gl_FragData[0].xyz = texture2D(colorMap, texCoords).xyz;
	//depth
	gl_FragData[0].w = gl_FragCoord.z;

	//normals
    gl_FragData[1].yxz = worldNormal * 0.5 + 0.5;
//	gl_FragData[1].xyz = normalM * 0.5 + 0.5;
//	gl_FragData[1].xyz = normal * 0.5 + 0.5;
	gl_FragData[1].w = 1.0;
}
