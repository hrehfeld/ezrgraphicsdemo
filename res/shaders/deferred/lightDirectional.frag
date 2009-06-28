varying vec2 screenPos;
//eye pos
uniform vec3 eye;
varying vec3 light;

uniform vec3 lightdir;

uniform sampler2D color3_depth1;
uniform sampler2D normal2;

void main (void)
{
//	vec2 current = gl_FragCoord.xy;
	vec2 current = screenPos;

//	vec2 sample = texture2D(normal2, current).xy;
	// reconstruct z component of normal (using the assumption that lenght(normal) == 1)
//	float normalZ = sqrt(1.0 - (dot(sample, sample)));
//	vec3 normal = vec3(sample, normalZ);
	vec3 normal = (texture2D(normal2, current).xyz - 0.5) * 2.0;
//	vec3 normal = texture2D(normal2, current).xyz;
	
	vec4 ambientColor = gl_LightSource[0].ambient
//		* gl_FrontMaterial.ambient;
		;

	float lightAmount = max(dot(normalize(
//									gl_LightSource[0].position.xyz
									light
									), normal), 0.0 );
	vec4 lightColor = gl_LightSource[0].diffuse
//		* gl_FrontMaterial.diffuse
		* lightAmount;

	/* vec3 normEye = normalize(eye); */
	/* float specular = clamp(dot(reflect(-normEye, normal), normLight), 0.0, 1.0); */
	/* specular = pow(specular, gl_FrontMaterial.shininess); */
	/* vec4 specularColor = gl_LightSource[0].specular * gl_FrontMaterial.specular * specular; */

	vec4 specularColor = vec4(0);

	
	vec4 colorDepthSample = texture2D(color3_depth1, current);
	vec4 diffuseColor = vec4(colorDepthSample.xyz, 1.0);
//	vec4 diffuseColor = vec4(1);
	float depth = colorDepthSample.w;
	
	gl_FragColor = (ambientColor * diffuseColor + lightColor * diffuseColor + specularColor);
//	gl_FragColor = diffuseColor;
//	gl_FragColor = vec4(lightAmount);
//	gl_FragColor = vec4(lightdir, 1);
//	gl_FragColor = vec4(gl_FragCoord.xy, 0, 1);
//	gl_FragColor = vec4(normal, 1);
//	gl_FragColor = colorDepthSample;
}
