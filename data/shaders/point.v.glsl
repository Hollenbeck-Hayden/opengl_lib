attribute vec2 position;
uniform float radius;

varying vec2 v_center;
varying float v_radius;

uniform mat4 vp_matrix;

void main()
{
	gl_Position = vp_matrix * vec4(position, 0.0, 1.0);
	gl_PointSize = 2.0 + ceil(2.0*radius);

	v_radius = radius;
	v_center = position;

}
