varying vec2 myTexCoord;

void main() {
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	myTexCoord = vec2(gl_MultiTexCoord0);
}