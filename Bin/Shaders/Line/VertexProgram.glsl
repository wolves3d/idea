#version 330

uniform		mat4	mMVP;

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec4 vColor;

/*
attribute	vec3 	vPos;
attribute	vec4	vColor;
*/


void main()
{
	gl_Position		= mMVP * vec4( vPos, 1);
	gl_FrontColor	= vColor;
}