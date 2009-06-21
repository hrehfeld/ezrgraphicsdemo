varying vec3 normal;
varying vec3 v;
varying vec3 lightvec;
 
void main(void)
{
    vec3 Eye             = normalize(-v);
 
    vec3 Reflected       = normalize( reflect( -lightvec, normal )); 
 //hat den selben effekt wie
 //vec3 Reflected       = normalize( 2.0 * dot(normal, lightvec) *  normal - lightvec);
 
    vec4 IAmbient        = gl_LightSource[0].ambient;
	vec4 IDiffuse        = gl_LightSource[0].diffuse * max(dot(normal, lightvec), 0.0);
 	vec4 ISpecular       = gl_LightSource[0].specular * pow(max(dot(Reflected, Eye), 0.0), gl_FrontMaterial.shininess);
// 	gl_FragColor         = gl_FrontLightModelProduct.sceneColor + IAmbient + IDiffuse + ISpecular;

	gl_FragColor = vec4(gl_FragCoord.w, gl_FragCoord.w, gl_FragCoord.w, 1);
}