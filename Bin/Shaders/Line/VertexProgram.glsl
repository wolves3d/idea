attribute	vec3 	vPos;
attribute	vec4	vColor;

uniform		mat4	mMVP;

void main()
{
	gl_Position		= mMVP * vec4( vPos, 1);
	gl_FrontColor	= vColor;
}