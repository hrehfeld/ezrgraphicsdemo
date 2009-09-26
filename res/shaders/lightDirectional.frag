struct clippingPlanes {
	float nearSize;
	float nearDistance;
};

//pixel position on the screen in [0, 1] range (for texture lookups)
varying vec2 screenPos;
varying vec2 fragmentScreen;
varying vec3 fragmentView;

//eye pos
uniform vec3 eye;
varying vec3 light;
uniform vec3 lightdir;
uniform sampler2D color;
uniform sampler2D normal;
uniform clippingPlanes planes;

void main (void)
{
	vec2 current = screenPos;

	//read color
	//vec4 colorDepthSample = texture2D(color, current);
    vec3 colorSample = texture2D(color, current);
	vec4 diffuseColor = vec4(colorSample.xyz, 1.0);
	//read depth
	//float depth = colorDepthSample.w;
	

	//read normal
    vec2 sample = (texture2D(normal, current).xy - 0.5) * 2.0;
	//reconstruct z component of normal (using the assumption that lenght(normal) == 1)
	//killzone2: Normal.z = sqrt(1.0 - pow(Normal.x, 2) - pow(Normal.y, 2))
    float normalZ = sqrt(1.0 - sample.x * sample.x - sample.y * sample.y);
    vec3 normalView = vec3(sample, normalZ);
	
    //read normal without clamping
	//vec3 normalView = normalize(((texture2D(normal, current).xyz) - 0.5) * 2.0);
	//@todo check clamping - workaround necessary?
	//vec3 normalView = normalize(texture2D(normal2, current).xyz);

    float depth = texture2D(normal, current).z;

	vec4 ambientColor = gl_LightSource[0].ambient;

	float lightAmount = max(dot(-light, normalView), 0.0 );
	vec4 lightColor = gl_LightSource[0].diffuse * lightAmount;

	vec3 eyeRayView = normalize(fragmentView);
	vec3 hitView = depth * eyeRayView;

	float specular = clamp(dot(reflect(eyeRayView, normalView), -light), 0.0, 1.0);
	specular = pow(specular, gl_FrontMaterial.shininess);
	vec4 specularColor = gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;

//	specularColor = vec4(0);

	
	
	gl_FragData[0] = (ambientColor * diffuseColor + lightColor * diffuseColor + specularColor);
//debug stuff
//	gl_FragData[0] = diffuseColor;
//	gl_FragData[0] = vec4(lightAmount);
//	gl_FragData[0] = vec4(dot(light, normalView), dot(light, normalView), dot(light, normalView), 1);

//	gl_FragData[0] = vec4(length(light) * 0.5);
//	gl_FragData[0] = vec4(normalView, 1);
//	gl_FragData[0] = modelViewMatrixInverse * vec4(normalView, 1);
//	gl_FragData[0] = vec4(gl_FragCoord.xy, 0, 1);
//	gl_FragData[0] = vec4(normalMatrixInverse * normalView.xyz, 1);
//	gl_FragData[0] = vec4(light - normalView, 1);
//	gl_FragData[0] = vec4(reflect(eyeRayView, normalView), 1);
//	gl_FragData[0] = colorDepthSample;
//	gl_FragData[0] = vec4(fragmentScreen, 0.0, 1.0);
//	gl_FragData[0] = vec4(fragmentView.x, fragmentView.y, fragmentView.z, 1.0);
//	gl_FragData[0] = vec4(-eyeRayView, 1);
//	gl_FragData[0] = vec4(hitView, 1);
//	gl_FragData[0] = vec4(specular, specular, specular, 1.0);
//	gl_FragData[0] = vec4(vec3(depth) * 0.1, 1.0);	
//	gl_FragData[0] = vec4(z, z, z, 1.0);	
}

