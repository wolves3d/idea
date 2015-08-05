#version 150

uniform	mat4 invViewProj;

uniform vec4 setup;
uniform vec4 waveDirs[4];
uniform vec4 waveParams[4];

in vec2 vPos;
in vec2 vTex0;

out vec4 fragWaveSetup;
out vec4 fragWaveDirs[4];
out vec4 fragWaveParams[4];

out vec2 fragTexCoord;
out mat4 fragIVP;

void main()
{
	fragWaveSetup = setup;
	fragWaveDirs = waveDirs;
	fragWaveParams = waveParams;
	
	fragTexCoord = vTex0;
	fragIVP = invViewProj;
	gl_Position = vec4( vPos.x, vPos.y, 0, 1 );
}