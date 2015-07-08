attribute	vec3 	vPos;
attribute	vec2 	vTex0;

varying		vec2	texCoord;

uniform		mat4	mMVP;

void main()
{
	texCoord		= vTex0;	
	gl_Position		= mMVP * vec4( vPos, 1);
	gl_FrontColor	= vec4( 1, 1, 1, 1 );
}