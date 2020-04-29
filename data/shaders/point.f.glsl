varying vec2 v_center;
varying float v_radius;

void main()
{
	vec2 p = gl_FragCoord.xy - v_center;
	float a = 1.0;
	float d = length(p) - v_radius + 1.0;
	if (d > 0.0) a = exp(-d*d);
	gl_FragColor = vec4(vec3(0.0), a);

	//gl_FragColor = vec4(0,0,0,1);
}
