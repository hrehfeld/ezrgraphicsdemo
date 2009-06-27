varying vec3 normal;
varying vec2 texCoords;

uniform sampler2D colorMap;
uniform sampler2D normalMap;
 
void main(void)
{
	//color info
    gl_FragData[0].xyz = texture2D(colorMap, texCoords).xyz;
	//depth
	gl_FragData[0].w = gl_FragCoord.z;

	//normals
    gl_FragData[1].xy = normal.xy;
	gl_FragData[1].zw = vec2(1.0);
}
