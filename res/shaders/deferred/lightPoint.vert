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

uniform vec3 eye;
uniform vec3 light;

/* uniform mat4 modelViewMatrix; */
/* uniform mat4 modelViewMatrixInverse; */
/* uniform mat3 normalMatrix; */
/* uniform mat3 normalMatrixInverse; */
/* uniform mat4 projectionMatrix; */
/* uniform mat4 projectionMatrixInverse; */

uniform clippingPlanes planes;

void main(void)
{
    gl_Position = ftransform();
}
