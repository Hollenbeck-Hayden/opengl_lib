attribute vec3 position;

uniform mat4 vp_matrix;

void main() {
	gl_Position = vp_matrix * vec4(position, 1.0);
}
