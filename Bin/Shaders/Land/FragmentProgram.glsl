uniform sampler2D DiffuseMap;

varying vec2 texCoord;

void main(void)
{
	gl_FragColor = texture2D( DiffuseMap, texCoord );
	
	// color = ( tex2D( dif_1 ) * inf_1 ) + ( tex2D( dif_2 ) * inf_2 );
}