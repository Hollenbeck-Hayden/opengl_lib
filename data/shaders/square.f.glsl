float distance(vec2 P, vec2 center, float radius)
{
	return length(P-center) - radius;
}

varying vec2 v_position;

void main()
{
	const float epsilon = 0.005;

	float d = distance(v_position, vec2(0.0), 0.5);
	if (d < -epsilon)
		gl_FragColor = vec4(1.0-abs(d), 0.0, 0.0, 1.0);
	else if (d > epsilon)
		gl_FragColor = vec4(0.0, 0.0, 1.0-abs(d), 1.0);
	else
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
