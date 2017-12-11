varying vec2 myTexCoord;
uniform sampler2D diffuse;
uniform bool useColor = true;

void main() {
	if (true == useColor)
	{
		gl_FragColor = gl_Color;
	}
	else
	{
		gl_FragColor = texture2D(diffuse, myTexCoord) * gl_Color;
	}
}
