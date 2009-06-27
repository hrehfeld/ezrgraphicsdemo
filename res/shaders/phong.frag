//light dir in ts
varying vec3 light;
varying float lightDistSqr;
//eye pos in ts
varying vec3 eye;
varying vec2 texCoords;

uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform float invRadius;

void main (void)
{
	float attenuation = clamp(1.0 - invRadius * lightDistSqr, 0.0, 1.0);
	//normlight = light / sqrt(lightDistSq)
	vec3 normLight = light * inversesqrt(lightDistSqr);

	//read normal and convert from [0,1] to [-1,1]
	vec3 normal = normalize(texture2D(normalMap, texCoords).xyz * 2.0 - 1.0);
	//debug
//	vec3 normal = vec3(0.0, 0.0, 1.0);


	vec4 ambientColor = gl_LightSource[0].ambient
//		* gl_FrontMaterial.ambient;
		;

	float lightAmount = max(dot(normLight, normal), 0.0 );
	vec4 lightColor = gl_LightSource[0].diffuse
//		* gl_FrontMaterial.diffuse
		* lightAmount;

	vec3 normEye = normalize(eye);
	float specular = clamp(dot(reflect(-normEye, normal), normLight), 0.0, 1.0);
	specular = pow(specular, gl_FrontMaterial.shininess);
	
	vec4 specularColor = gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;
	
	vec4 diffuseColor = texture2D(colorMap, texCoords);
	gl_FragColor = (ambientColor * diffuseColor + lightColor * diffuseColor + specularColor) * attenuation;
	//debug
	//gl_FragColor = vec4(specular, specular, specular, 1.0);
//	gl_FragColor = vec4(normLight, 1.0);
//	gl_FragColor = vec4(specularColor);
}
