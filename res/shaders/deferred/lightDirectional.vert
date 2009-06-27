varying vec2 screenPos;
varying vec3 light;

uniform vec3 lightdir;

void main(void)
{
//	light = vec3(gl_ModelViewMatrix * vec4(lightdir, 1));
	light = lightdir;
//	light = vec3(1, 0, 0);
	
	screenPos = gl_MultiTexCoord0.xy;
    gl_Position = ftransform();
}
