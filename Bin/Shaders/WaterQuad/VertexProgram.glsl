#version 150

uniform	mat4 invViewProj;
uniform	vec4 waveParams;

in vec2 vPos;
in vec2 vTex0;

out vec2 fragTexCoord;
out vec4 fragWaveParams;
out mat4 fragIVP;

void main()
{
	fragWaveParams = waveParams;
	fragTexCoord = vTex0;
	fragIVP = invViewProj;
	gl_Position = vec4( vPos.x, vPos.y, 0, 1 );
}