// #define OPTION_HALF_COLOR

attribute	vec3 	vPos;
attribute	vec2 	vTex0;
attribute	vec4 	vSkin;

varying		vec2	texCoord;

uniform		mat4	pBoneMatrices[ 20 ];

void main()
{
	texCoord		= vTex0;	
	
	vec4 vPos1		= vSkin.y * pBoneMatrices[ int( vSkin.x ) ] * vec4( vPos, 1 );
	vec4 vPos2		= vSkin.w * pBoneMatrices[ int( vSkin.z ) ] * vec4( vPos, 1 );
	
	gl_Position		= gl_ModelViewProjectionMatrix * ( vPos1 + vPos2 );
	
	#ifdef OPTION_HALF_COLOR
		gl_FrontColor	= vec4( 0.5, 0.5, 0.5, 1 );
	#else
		gl_FrontColor	= vec4( 1, 1, 1, 1 );
	#endif // #ifdef OPTION_HALF_COLOR
}