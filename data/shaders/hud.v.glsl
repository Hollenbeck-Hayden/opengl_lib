attribute vec2 position;

uniform mat4 vp_matrix;

void main() {
	gl_Position = vp_matrix * vec4(position, 0.0, 1.0);
}
