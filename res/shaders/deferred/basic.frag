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
	//@todo check if we need to normalize, i think we don't because normal and vec are normalised
	//tangent = normalize(tangent);	

	vec3 binormal = cross(tangent, normal);

	vec3 normalM = (texture2D(normalMap, texCoords).xyz - 0.5) * 2;
	//rotate normal from image (in tangent space) to world space
	//(binormal tangent normal) * normalM 
	vec3 col0 = vec3(tangent.x, binormal.x, normal.x);
	vec3 col1 = vec3(tangent.y, binormal.y, normal.y);
	vec3 col2 = vec3(tangent.z, binormal.z, normal.z);

	//rotates in the other direction
	/* worldNormal.y = dot(normalM, tangent); */
	/* worldNormal.x = dot(normalM, binormal); */
	/* worldNormal.z = dot(normalM, normal); */
	vec3 worldNormal = vec3(dot(normalM, col0),
							dot(normalM, col1),
							dot(normalM, col2));
	//should be normalized
    //worldNormal = normalize(worldNormal);


	//color info
    gl_FragData[0].xyz = texture2D(colorMap, texCoords).xyz;
	//depth
	gl_FragData[0].w = gl_FragCoord.z;

	//normals
    gl_FragData[1].xyz = worldNormal * 0.5 + 0.5;
//	gl_FragData[1].xyz = normalM * 0.5 + 0.5;
//	gl_FragData[1].xyz = normal * 0.5 + 0.5;
	gl_FragData[1].w = 1.0;
}
