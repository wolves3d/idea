uniform sampler2D DiffuseMap;

in vec2 fragTexCoord;

void main(void)
{
	gl_FragColor = texture2D( DiffuseMap, fragTexCoord.st );
}