//varying vec2 v_res;
varying float v_ab, v_t, v_alpha;
varying vec2 v_A, v_T, v_O;

void main()
{
	// Get screen coordinates
	vec2 pos = gl_FragCoord.xy - v_A;

	// Convert fragment position to uv coordinates
	vec2 uv = vec2(dot(pos, v_T), dot(pos, v_O));
	

	// Determine signed distance of fragment from line
	float d = 0;
	// Check if A endcap
	if (uv.x < 0)
	{
		d = length(pos) - v_t;
	}
	// Check if B endcap
	else if (uv.x > v_ab)
	{
		d = distance(v_ab * v_T, pos) - v_t;
	}
	// Line segment body
	else
	{
		d = abs(uv.y) - v_t;
	}

	// Fragment inside line body
	if (d < 0)
	{
		gl_FragColor = vec4(255,0,0,v_alpha);
	}
	// Fragment in antialiasing region
	else
	{
		gl_FragColor = vec4(255,0,0,v_alpha * exp(-d*d));
	}
}
