struct clippingPlanes {
	float nearSize;
	float nearDistance;
};

uniform vec3 light;

uniform sampler2D color3_depth1;
uniform sampler2D normal2;

uniform clippingPlanes planes;

uniform ivec2 screenSize;

uniform float attenuation;

/* uniform mat4 modelViewMatrix; */
/* uniform mat4 modelViewMatrixInverse; */
/* uniform mat3 normalMatrix; */
/* uniform mat3 normalMatrixInverse; */
/* uniform mat4 projectionMatrix; */
/* uniform mat4 projectionMatrixInverse; */

void main (void)
{
    //pixel position on the screen in [0, 1] range (for texture lookups)
	vec2 screenPos = gl_FragCoord.xy / screenSize;

	vec2 fragmentScreen = (screenPos - 0.5) * 2.0;
	vec3 fragmentView = vec3(fragmentScreen * planes.nearSize, -(planes.nearDistance));

	vec2 current = screenPos;

	//read color
	vec4 colorDepthSample = texture2D(color3_depth1, current);
	vec4 diffuseColor = vec4(colorDepthSample.xyz, 1.0);
	//read depth
	float depth = colorDepthSample.w;
	
	

	//read normal
	vec2 normalSample = texture2D(normal2, current).xy;
	vec2 sample = (normalSample.xy - 0.5) * 2.0;
	//reconstruct z component of normal (using the assumption that lenght(normal) == 1)
	//killzone2: Normal.z = sqrt(1.0 - pow(Normal.x, 2) - pow(Normal.y, 2))
	float normalZ = sqrt(1.0 - sample.x * sample.x - sample.y * sample.y);
//    vec3 normalView = vec3(sample, normalZ);
	//read normal without clamping
	vec3 normalView = normalize(((texture2D(normal2, current).xyz) - 0.5) * 2.0);
	//@todo check clamping - workaround necessary?
	//vec3 normalView = normalize(texture2D(normal2, current).xyz);


	vec4 ambientColor = gl_LightSource[0].ambient;

	vec3 eyeRayView = fragmentView / planes.nearDistance;
	vec3 hitView = depth * eyeRayView;
	vec3 lightHit = light - hitView;
	vec3 lightDir = normalize(lightHit);

	float lightAmount = max(dot(lightDir, normalView), 0.0);
	vec4 lightColor = gl_LightSource[0].diffuse * lightAmount;

	float att = clamp(1.0 - attenuation * dot(lightHit, lightHit), 0.0, 1.0);


	float specular = clamp(dot(reflect(normalize(eyeRayView), normalView), lightDir), 0.0, 1.0);
	specular = pow(specular, gl_FrontMaterial.shininess);
	vec4 specularColor = gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;

	gl_FragData[0] = (ambientColor * diffuseColor + lightColor * diffuseColor + specularColor) * att;
	
//debug stuff
//	vec2 test = texture2D(normal2, current).zw;
//	gl_FragData[0] = vec4(-light, 1);
//	gl_FragData[0] = vec4(vec2(test.x) + 10.0, vec2(test.y) + 10.0);
//	gl_FragData[0] = diffuseColor;
//	gl_FragData[0] = vec4(lightAmount);
//	gl_FragData[0] = vec4(att);
//	gl_FragData[0] = vec4(-fragmentView, 1);
//	gl_FragData[0] = modelViewMatrixInverse * vec4(light, 1) - vec4(normalMatrixInverse * normalView, 1);	
//	gl_FragData[0] = (modelViewMatrixInverse * vec4(light, 1)
//					  - modelViewMatrixInverse * vec4(hitView, 1)) * 0.3;	
//	gl_FragData[0] = -(modelViewMatrixInverse * vec4(light, 1)) * 0.4;
//	gl_FragData[0] = -modelViewMatrixInverse * vec4(hitView, 1);
//	gl_FragData[0] = vec4(att);
//	gl_FragData[0] = vec4(hitView - hitViewConstructed, 1);	
//	gl_FragData[0] = vec4((hitViewConstructed - hitView).y);	


//	gl_FragData[0] = vec4(length(light) * 0.5);
//	gl_FragData[0] = vec4(normalView, 1);
//	gl_FragData[0] = vec4(normalMatrixInverse * normalView, 1);
//	gl_FragData[0] = vec4(gl_FragCoord.xy, 0, 1);
//	gl_FragData[0] = vec4(normalView - hitView, 1);
//	gl_FragData[0] = vec4(light - normalView, 1);
//	gl_FragData[0] = vec4(reflect(eyeRayView, normalView), 1);
//	gl_FragData[0] = colorDepthSample;
//	gl_FragData[0] = vec4(fragmentScreen, 0.0, 1.0);
//	gl_FragData[0] = vec4(fragmentView.x, fragmentView.y, fragmentView.z, 1.0);
//	gl_FragData[0] = vec4(-eyeRayView, 1);
//	gl_FragData[0] = vec4(hitView, 1);
//	gl_FragData[0] = vec4(lightHit, 1);
//	vec3 lightDirWorld = normalMatrixInverse * lightDir;
//	gl_FragData[0] = vec4(-lightDirWorld, 1);
//	gl_FragData[0] = vec4(specular, specular, specular, 1.0);
//	gl_FragData[0] = vec4(normalView.xy, normalView.z - normalView2.z, 1);
//	gl_FragData[0] = vec4(vec3(depth) * 0.1, 1.0);	
//	gl_FragData[0] = vec4(z, z, z, 1.0);	
}

