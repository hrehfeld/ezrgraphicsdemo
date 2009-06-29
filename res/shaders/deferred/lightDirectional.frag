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

uniform sampler2D color3_depth1;
uniform sampler2D normal2;

uniform clippingPlanes planes;

uniform mat4 modelViewMatrix;
uniform mat4 modelViewMatrixInverse;
uniform mat3 normalMatrix;
uniform mat3 normalMatrixInverse;
uniform mat4 projectionMatrix;
uniform mat4 projectionMatrixInverse;

//currently not used, check if it's right and what it's doing
float DepthToZPosition(in float depth) {
	return gl_DepthRange.near
		/ (gl_DepthRange.far - depth * (gl_DepthRange.far - gl_DepthRange.near))
		* gl_DepthRange.far;
}

void main (void)
{
	vec2 current = screenPos;

	//read color
	vec4 colorDepthSample = texture2D(color3_depth1, current);
	vec4 diffuseColor = vec4(colorDepthSample.xyz, 1.0);
	//read depth
	float depth = colorDepthSample.w;
	

	//read normal
    //vec2 sample = texture2D(normal2, current).xy;
	//reconstruct z component of normal (using the assumption that lenght(normal) == 1)
	//killzone2: Normal.z = sqrt(1.0 - pow(Normal.x, 2) - pow(Normal.y, 2))
    //float normalZ = sqrt(1.0 - (dot(sample, sample)));
    //vec3 normal = vec3(sample, normalZ);
	//read normal without clamping
	vec3 normalView = normalize(((texture2D(normal2, current).xyz) - 0.5) * 2.0);

	vec4 ambientColor = gl_LightSource[0].ambient;

	float lightAmount = max(dot(normalize(-light), normalView), 0.0 );
	vec4 lightColor = gl_LightSource[0].diffuse * lightAmount;

	vec3 eyeDirView = normalize(fragmentView);
	vec3 hitView = depth * eyeDirView;

	float specular = clamp(dot(reflect(eyeDirView, normalView), light), 0.0, 1.0);
	specular = pow(specular, gl_FrontMaterial.shininess);
	vec4 specularColor = gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;

	specularColor = vec4(0);

	
	
	gl_FragColor = (ambientColor * diffuseColor + lightColor * diffuseColor + specularColor);
//debug stuff
//	gl_FragColor = diffuseColor;
//	gl_FragColor = vec4(lightAmount);
//	gl_FragColor = vec4(dot(light, normalView), dot(light, normalView), dot(light, normalView), 1);

//	gl_FragColor = vec4(length(light) * 0.5);
//	gl_FragColor = vec4(normalView, 1);
//	gl_FragColor = modelViewMatrixInverse * vec4(normalView, 1);
//	gl_FragColor = vec4(gl_FragCoord.xy, 0, 1);
//	gl_FragColor = vec4(normalView.xyz, 1);
//	gl_FragColor = vec4(light - normalView, 1);
//	gl_FragColor = vec4(reflect(normalize(eyeDir), normalView), 1);
//	gl_FragColor = colorDepthSample;
//	gl_FragColor = vec4(fragmentScreen, 0.0, 1.0);
//	gl_FragColor = vec4(fragmentView.x, fragmentView.y, fragmentView.z, 1.0);
//	gl_FragColor = vec4(eyeDir, 1);
//	gl_FragColor = vec4(hitView, 1);
//	gl_FragColor = vec4(specular, specular, specular, 1.0);
//	gl_FragColor = vec4(depth, depth, depth, 1.0);	
//	gl_FragColor = vec4(z, z, z, 1.0);	
}

