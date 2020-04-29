attribute vec2 pA, pB, uv;
attribute float thickness;
attribute vec3 color;

uniform vec2 resolution;
uniform mat4 vp_matrix;

//varying vec2 v_res;
varying float v_ab, v_t, v_alpha;
varying vec2 v_T, v_O, v_A;

vec2 ndc_to_screen(vec2 ndc)
{
	return (ndc + vec2(1,1)) * resolution / 2;
}

vec2 screen_to_ndc(vec2 scr)
{
	return (scr * 2 / resolution) - vec2(1,1);
}



void main()
{
	float width = abs(thickness);
	if (width < 1)
	{
		v_alpha = width;
		width = 1;
	}
	else
	{
		v_alpha = 1;
	}

	float filter = 2;

	// Map endpoints to NDC coords and project to screen
	vec2 posA = ndc_to_screen((vp_matrix * vec4(pA, 0.0, 1.0)).xy);
	vec2 posB = ndc_to_screen((vp_matrix * vec4(pB, 0.0, 1.0)).xy);

	// Calculate vector T along line segment and vector O orthogonal to segment
	float ab = length(posB - posA);
	vec2 T = (posB - posA) / ab;
	vec2 O = vec2(-T.y,T.x);

	// Convert screen thickness to NDC thickness
	float t = filter + width / 2;

	// Set vertex position
	vec2 vertex_pos = posA + (ab * uv.x * T) + (2 * uv.x - 1) * t * (T + (1 - 2 * uv.y) * O);
	gl_Position = vec4(screen_to_ndc(vertex_pos), 0, 1);

	// Pass data to fragment shader
	//v_res = resolution;
	v_ab = ab;
	v_A = posA;
	v_T = T;
	v_O = O;
	v_t = width/2;
}
