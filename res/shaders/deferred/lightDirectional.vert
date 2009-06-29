struct clippingPlanes {
	/**
	 * smallest distance from center of the plane to the sides
	 */
	float nearSize;
	/**
	 * distance of the nearplane from eye
	 */
	float nearDistance;
};

varying vec2 screenPos;
varying vec2 fragmentScreen;
varying vec3 fragmentView;
varying vec3 light;

uniform vec3 eye;
uniform vec3 lightdir;

uniform mat4 modelViewMatrix;
uniform mat4 modelViewMatrixInverse;
uniform mat3 normalMatrix;
uniform mat3 normalMatrixInverse;
uniform mat4 projectionMatrix;
uniform mat4 projectionMatrixInverse;

uniform clippingPlanes planes;

void main(void)
{
	light = lightdir;
	
	screenPos = gl_MultiTexCoord0.xy;

	fragmentScreen = (screenPos - 0.5) * 2.0;
	fragmentView = vec3(fragmentScreen * planes.nearSize, planes.nearDistance);

    gl_Position = ftransform();
}
