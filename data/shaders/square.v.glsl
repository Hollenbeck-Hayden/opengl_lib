attribute vec2 position;

uniform mat4 vp_matrix;

varying vec2 v_position;

void main() {
	v_position = position;
	gl_Position = vp_matrix * vec4(position, 0.0, 1.0);
}
