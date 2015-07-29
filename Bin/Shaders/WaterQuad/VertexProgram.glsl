#version 150

in vec2 vPos;
in vec2 vTex0;

out vec2 fragTexCoord;

void main()
{
	fragTexCoord = vTex0;
	gl_Position = vec4( vPos.x, vPos.y, 0, 1 );
}