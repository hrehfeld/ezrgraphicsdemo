//light dir in ts
varying vec3 light;
//eye pos in ts
varying vec3 eye;
varying vec2 texCoords;

uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform float invRadius;

void main (void)
{
	float lightDistSqr = dot(light, light);
	float att = clamp(1.0 - invRadius * sqrt(lightDistSqr), 0.0, 1.0);
	vec3 lVec = light * inversesqrt(lightDistSqr);

	vec3 vVec = normalize(eye);
	
	vec4 base = texture2D(colorMap, texCoords);
	
	vec3 bump = normalize( texture2D(normalMap, texCoords).xyz * 2.0 - 1.0);

	vec4 vAmbient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

	float diffuse = max( dot(lVec, bump), 0.0 );
	
	vec4 vDiffuse = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * 
					diffuse;	

	float specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0), 
	                 gl_FrontMaterial.shininess );
	
	vec4 vSpecular = gl_LightSource[0].specular * gl_FrontMaterial.specular * 
					 specular;	
	
	gl_FragColor = (vAmbient * base + vDiffuse * base + vSpecular) * att;
}
