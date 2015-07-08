uniform sampler2D DiffuseMap;

void main(void)
{
	gl_FragColor = gl_Color * texture2D( DiffuseMap, gl_TexCoord[0].st );
}