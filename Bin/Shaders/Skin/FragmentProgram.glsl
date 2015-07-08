uniform sampler2D DiffuseMap;

varying vec2 texCoord;

void main(void)
{
	gl_FragColor = texture2D( DiffuseMap, texCoord );
}